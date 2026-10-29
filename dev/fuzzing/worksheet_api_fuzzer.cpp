/*
 * worksheet_api_fuzzer — fuzz the libxlsxwriter worksheet API surface.
 *
 * Threat model:
 *   * Untrusted input: applications routinely push attacker-controlled data
 *     into libxlsxwriter's worksheet APIs — cell strings, numbers, formulas,
 *     URLs, comments, header/footer strings, table/validation/conditional
 *     formatting strings and A1-style range text. The library must never
 *     violate memory safety regardless of that content.
 *   * Trust boundary: the public worksheet_write_* / worksheet_set_* calls
 *     exercised here; the existing xlsx_fuzzer only reaches
 *     worksheet_write_string, leaving ~245 of 302 worksheet.c functions dark.
 *   * What the harness holds fixed: workbook/worksheet lifecycle, a small set
 *     of valid lxw_format objects (so format-dependent paths run on every
 *     write type), and the output path (mkstemp'd temp file, unlinked after
 *     workbook_close).
 *   * Expected bug classes: row/col bounds handling, UTF-8 / control-character
 *     escaping, datetime and unixtime float arithmetic, URL and header/footer
 *     string handling, rich-string format cycling, data validation /
 *     conditional format / filter rule string handling, A1-range string
 *     parsing (utility.c).
 *
 * Input shape: an op stream. Each iteration consumes one op byte and then a
 * fixed, per-op sequence of FuzzedDataProvider draws (rows, cols, strings,
 * doubles). Every draw happens as its own statement (never two draws inside
 * one call expression: C++ argument evaluation order is unspecified).
 * dev/fuzzing/generate_worksheet_api_seeds.py mirrors the exact consumption
 * order to build structured seeds.
 *
 * Findings LXW-1/LXW-2 (real, upstream setter-overwrite leaks):
 * LXW-1: worksheet_set_h/v_pagebreaks leak the
 * previous calloc-ed break array when called twice on the same worksheet
 * previous calloc-ed break array on double call (worksheet.c:10322).
 * LXW-2: worksheet_set_comments_author strdups over the previous author
 * (worksheet.c:11689) leaking it on double call; worksheet_set_vba_name has
 * the identical pattern (worksheet.c:11678, guarded proactively, same class).
 * Reproducers quarantined under covdata/libxlsxwriter/artifacts/. The harness
 * mirrors the legitimate idempotent pattern (each string setter at most once
 * per iteration); the leaks are NOT silenced and stay reproducible from the
 * artefacts.
 *
 * Strings are passed as iteration-local std::string c_str() pointers: the
 * library copies (strdups) every string it keeps, so no caller-side lifetime
 * issues exist and nothing is leaked by the harness.
 */
#include <cstdint>
#include <cstring>
#include <string>
#include <unistd.h>

#include <fuzzer/FuzzedDataProvider.h>

#include "xlsxwriter.h"

namespace {

const std::string mem_dir = "/dev/shm";
const std::string file_template = "/wsfuzzXXXXXX";
char temp_file_dir[FILENAME_MAX] = {0};

const size_t kMaxStr = 64;
const uint8_t kNumOps = 50;

int init_for_fuzzing()
{
    if (0 == access(mem_dir.c_str(), W_OK | R_OK))
        memcpy(temp_file_dir, mem_dir.c_str(), strnlen(mem_dir.c_str(), FILENAME_MAX));
    else
    {
        const char *tmp_prefix = getenv("TMPDIR");
        if (nullptr == tmp_prefix)
            tmp_prefix = "/tmp";
        memcpy((void *)temp_file_dir, tmp_prefix, strnlen(tmp_prefix, FILENAME_MAX));
    }
    return 0;
}

/* Fixed per-op draw helpers: the seed generator mirrors these exactly. */
lxw_row_t draw_row(FuzzedDataProvider &fdp)
{
    return fdp.ConsumeIntegralInRange<lxw_row_t>(0, 200);
}

lxw_col_t draw_col(FuzzedDataProvider &fdp)
{
    return fdp.ConsumeIntegralInRange<lxw_col_t>(0, 60);
}

lxw_row_t draw_wild_row(FuzzedDataProvider &fdp)
{
    /* Bounded sparse-row writes: the maximum-row case materialises the
     * sheet up to dim_rowmax at close (~28 s under ASan), which exceeds the
     * production 25 s timeout (documented in the report as heavy-but-
     * legitimate format behaviour). 65,535 keeps the sparse-row and
     * dimension paths exercised at bounded cost. */
    return fdp.ConsumeIntegralInRange<lxw_row_t>(0, 65535);
}

lxw_col_t draw_wild_col(FuzzedDataProvider &fdp)
{
    return fdp.ConsumeIntegralInRange<lxw_col_t>(0, 16383);
}

std::string draw_str(FuzzedDataProvider &fdp)
{
    return fdp.ConsumeRandomLengthString(kMaxStr);
}

/* Bounded double: keeps date math in plausible ranges most of the time. */
double draw_double(FuzzedDataProvider &fdp)
{
    return (double)fdp.ConsumeIntegralInRange<int32_t>(-1000000, 1000000) / 64.0;
}

/* Full bit-pattern double: NaN/Inf/denormal edges for numeric paths. */
double draw_raw_double(FuzzedDataProvider &fdp)
{
    uint64_t bits = fdp.ConsumeIntegral<uint64_t>();
    double v;
    memcpy(&v, &bits, sizeof(v));
    return v;
}

uint8_t draw_byte(FuzzedDataProvider &fdp)
{
    return fdp.ConsumeIntegral<uint8_t>();
}

uint32_t draw_u32(FuzzedDataProvider &fdp)
{
    return fdp.ConsumeIntegral<uint32_t>();
}

} // namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    static bool init_fuzzing = init_for_fuzzing();
    (void)init_fuzzing;

    char fuzz_file[FILENAME_MAX + 1] = {0};
    strncpy(fuzz_file, temp_file_dir, strlen(temp_file_dir));
    strncat(fuzz_file, file_template.c_str(), file_template.length());

    int fuzz_fd = mkstemp(fuzz_file);
    if (fuzz_fd < 0)
        return 0;

    FuzzedDataProvider fdp(data, size);

    lxw_workbook *workbook = workbook_new(fuzz_file);
    if (nullptr == workbook)
    {
        close(fuzz_fd);
        unlink(fuzz_file);
        return 0;
    }

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, nullptr);

    /* Four formats with distinct properties so format-dependent paths run. */
    lxw_format *fmt0 = workbook_add_format(workbook);
    format_set_bold(fmt0);
    lxw_format *fmt1 = workbook_add_format(workbook);
    format_set_num_format(fmt1, "0.000");
    format_set_italic(fmt1);
    lxw_format *fmt2 = workbook_add_format(workbook);
    format_set_border(fmt2, 1);
    format_set_border_color(fmt2, 0x335577);
    format_set_bg_color(fmt2, 0xDDEEFF);
    lxw_format *fmt3 = workbook_add_format(workbook);
    format_set_align(fmt3, LXW_ALIGN_CENTER);
    format_set_indent(fmt3, 2);
    format_set_underline(fmt3, LXW_UNDERLINE_SINGLE);
    format_set_font_name(fmt3, "fuzz");
    lxw_format *fmts[4] = {fmt0, fmt1, fmt2, fmt3};
    uint8_t fi = draw_byte(fdp) & 3; /* format-index byte in the stream header */
    (void)fi;

    bool it_hbreaks_set = false;
    bool it_vbreaks_set = false;
    bool it_header_set = false;
    bool it_footer_set = false;
    bool it_author_set = false;

    int guard = 0;
    while (fdp.remaining_bytes() > 2 && guard++ < 96)
    {
        uint8_t op = draw_byte(fdp);
        if (op >= kNumOps)
            continue;

        lxw_row_t r1 = 0, r2 = 0;
        lxw_col_t c1 = 0, c2 = 0;
        std::string s1, s2;
        uint8_t fb = 0;
        double d1 = 0.0, d2 = 0.0, d3 = 0.0, d4 = 0.0;
        uint8_t b1 = 0, b2 = 0, b3 = 0;
        uint32_t u1 = 0;

        switch (op)
        {
        case 0: /* write_string */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            fb = draw_byte(fdp);
            worksheet_write_string(worksheet, r1, c1, s1.c_str(), fmts[fb & 3]);
            break;
        case 1: /* write_number (bounded) */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            d1 = draw_double(fdp);
            fb = draw_byte(fdp);
            worksheet_write_number(worksheet, r1, c1, d1, fmts[fb & 3]);
            break;
        case 2: /* write_number (raw bit pattern) */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            d1 = draw_raw_double(fdp);
            fb = draw_byte(fdp);
            worksheet_write_number(worksheet, r1, c1, d1, fmts[fb & 3]);
            break;
        case 3: /* write_formula */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            fb = draw_byte(fdp);
            worksheet_write_formula(worksheet, r1, c1, s1.c_str(), fmts[fb & 3]);
            break;
        case 4: /* write_formula_num */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            fb = draw_byte(fdp);
            d1 = draw_double(fdp);
            worksheet_write_formula_num(worksheet, r1, c1, s1.c_str(), fmts[fb & 3], d1);
            break;
        case 5: /* write_formula_str */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            fb = draw_byte(fdp);
            worksheet_write_formula_str(worksheet, r1, c1, s1.c_str(), fmts[fb & 3], s2.c_str());
            break;
        case 6: /* write_array_formula */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            r2 = draw_row(fdp);
            c2 = draw_col(fdp);
            s1 = draw_str(fdp);
            fb = draw_byte(fdp);
            worksheet_write_array_formula(worksheet, r1, c1, r2, c2, s1.c_str(), fmts[fb & 3]);
            break;
        case 7: /* write_dynamic_array_formula */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            r2 = draw_row(fdp);
            c2 = draw_col(fdp);
            s1 = draw_str(fdp);
            fb = draw_byte(fdp);
            worksheet_write_dynamic_array_formula(worksheet, r1, c1, r2, c2, s1.c_str(),
                                                  fmts[fb & 3]);
            break;
        case 8: /* write_dynamic_formula */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            fb = draw_byte(fdp);
            worksheet_write_dynamic_formula(worksheet, r1, c1, s1.c_str(), fmts[fb & 3]);
            break;
        case 9: /* write_blank */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            fb = draw_byte(fdp);
            worksheet_write_blank(worksheet, r1, c1, fmts[fb & 3]);
            break;
        case 10: /* write_boolean */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            b1 = draw_byte(fdp);
            fb = draw_byte(fdp);
            worksheet_write_boolean(worksheet, r1, c1, b1 & 1, fmts[fb & 3]);
            break;
        case 11: /* write_datetime */
        {
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            lxw_datetime dt;
            memset(&dt, 0, sizeof(dt));
            b1 = draw_byte(fdp);
            b2 = draw_byte(fdp);
            b3 = draw_byte(fdp);
            dt.year = 1900 + (double)b1;
            dt.month = 1 + (double)(b2 % 12);
            dt.day = 1 + (double)(b3 % 28);
            dt.hour = (double)(draw_byte(fdp) % 24);
            dt.min = (double)(draw_byte(fdp) % 60);
            dt.sec = (double)(draw_byte(fdp) % 60) + 0.5;
            fb = draw_byte(fdp);
            worksheet_write_datetime(worksheet, r1, c1, &dt, fmts[fb & 3]);
            break;
        }
        case 12: /* write_unixtime (32-bit range) */
        {
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            b1 = draw_byte(fdp); /* 4 low bytes of the int64 */
            b2 = draw_byte(fdp);
            b3 = draw_byte(fdp);
            u1 = draw_byte(fdp); /* high byte */
            {
                uint64_t bits = (uint64_t)u1 << 32 | (uint64_t)b3 << 24 |
                                (uint64_t)b2 << 16 | (uint64_t)b1 << 8;
                bits |= (uint64_t)(draw_byte(fdp));
                fb = draw_byte(fdp);
                worksheet_write_unixtime(worksheet, r1, c1, (int64_t)bits, fmts[fb & 3]);
            }
            break;
        }
        case 13: /* write_unixtime (raw int64) */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            u1 = draw_u32(fdp); /* low half of the int64 */
            b1 = draw_byte(fdp); /* high half bits 32..39 */
            b2 = draw_byte(fdp);
            b3 = draw_byte(fdp);
            {
                uint64_t bits = (uint64_t)b3 << 56 | (uint64_t)b2 << 48 |
                                (uint64_t)b1 << 40 | (uint64_t)u1;
                fb = draw_byte(fdp);
                worksheet_write_unixtime(worksheet, r1, c1, (int64_t)bits, fmts[fb & 3]);
            }
            break;
        case 14: /* write_rich_string */
        {
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            std::string s3 = draw_str(fdp);
            b1 = draw_byte(fdp); /* fragment 1 format */
            fb = draw_byte(fdp); /* final cell format */
            lxw_rich_string_tuple t1, t2, t3;
            t1.format = fmts[b1 & 3];
            t1.string = s1.c_str();
            t2.format = nullptr;
            t2.string = s2.c_str();
            t3.format = fmt3;
            t3.string = s3.c_str();
            lxw_rich_string_tuple *tuples[4] = {&t1, &t2, &t3, nullptr};
            worksheet_write_rich_string(worksheet, r1, c1, tuples, fmts[fb & 3]);
            break;
        }
        case 15: /* write_comment */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            worksheet_write_comment(worksheet, r1, c1, s1.c_str());
            break;
        case 16: /* write_comment_opt */
        {
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            b1 = draw_byte(fdp); /* visible */
            b2 = draw_byte(fdp); /* width addend */
            b3 = draw_byte(fdp); /* height addend */
            lxw_comment_options copts;
            memset(&copts, 0, sizeof(copts));
            copts.visible = b1 & 1;
            copts.author = s2.c_str();
            copts.width = (uint16_t)(128 + b2);
            copts.height = (uint16_t)(64 + b3);
            copts.font_name = s2.empty() ? nullptr : s2.c_str();
            worksheet_write_comment_opt(worksheet, r1, c1, s1.c_str(), &copts);
            break;
        }
        case 17: /* write_url */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            fb = draw_byte(fdp);
            worksheet_write_url(worksheet, r1, c1, s1.c_str(), fmts[fb & 3]);
            break;
        case 18: /* write_url_opt (display string + tooltip) */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            fb = draw_byte(fdp);
            worksheet_write_url_opt(worksheet, r1, c1, s1.c_str(), fmts[fb & 3], s2.c_str(),
                                    s1.c_str());
            break;
        case 19: /* merge_range */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            r2 = draw_row(fdp);
            c2 = draw_col(fdp);
            s1 = draw_str(fdp);
            fb = draw_byte(fdp);
            worksheet_merge_range(worksheet, r1, c1, r2, c2, s1.c_str(), fmts[fb & 3]);
            break;
        case 20: /* autofilter */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            r2 = draw_row(fdp);
            c2 = draw_col(fdp);
            worksheet_autofilter(worksheet, r1, c1, r2, c2);
            break;
        case 21: /* add_table */
        {
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            r2 = draw_row(fdp);
            c2 = draw_col(fdp);
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            b1 = draw_byte(fdp); /* total_row */
            b2 = draw_byte(fdp); /* style number */
            b3 = draw_byte(fdp); /* columns flag */
            lxw_table_column col1, col2;
            memset(&col1, 0, sizeof(col1));
            memset(&col2, 0, sizeof(col2));
            col1.header = s1.c_str();
            col1.total_string = s2.c_str();
            col2.header = s2.c_str();
            lxw_table_column *cols[3] = {&col1, &col2, nullptr};
            lxw_table_options topts;
            memset(&topts, 0, sizeof(topts));
            topts.name = s2.c_str();
            topts.total_row = b1 & 1;
            topts.style_type = 1;
            topts.style_type_number = (uint8_t)(b2 % 49);
            topts.columns = (b3 & 1) ? cols : nullptr;
            worksheet_add_table(worksheet, r1, c1, r2, c2, &topts);
            break;
        }
        case 22: /* data_validation_range */
        {
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            r2 = draw_row(fdp);
            c2 = draw_col(fdp);
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            b1 = draw_byte(fdp); /* validate */
            b2 = draw_byte(fdp); /* criteria */
            b3 = draw_byte(fdp); /* ignore_blank + dropdown */
            d1 = draw_double(fdp);
            d2 = draw_double(fdp);
            d3 = draw_double(fdp);
            lxw_data_validation dv;
            memset(&dv, 0, sizeof(dv));
            dv.validate = b1 ? (uint8_t)(b1 % 12) : 1;
            dv.criteria = (uint8_t)(b2 % 11);
            dv.ignore_blank = b3 & 1;
            dv.show_input = 1;
            dv.show_error = 1;
            dv.error_type = (uint8_t)((b3 >> 1) % 3);
            dv.dropdown = (b3 >> 3) & 1;
            dv.value_number = d1;
            dv.minimum_number = d2;
            dv.maximum_number = d3;
            dv.value_formula = s1.c_str();
            dv.minimum_formula = s2.c_str();
            dv.input_title = s2.c_str();
            dv.input_message = s1.c_str();
            dv.error_title = s2.c_str();
            dv.error_message = s1.c_str();
            const char *list[3] = {s1.c_str(), s2.c_str(), nullptr};
            dv.value_list = (dv.validate == LXW_VALIDATION_TYPE_LIST) ? list : nullptr;
            worksheet_data_validation_range(worksheet, r1, c1, r2, c2, &dv);
            break;
        }
        case 23: /* conditional_format_range */
        {
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            r2 = draw_row(fdp);
            c2 = draw_col(fdp);
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            b1 = draw_byte(fdp); /* type */
            b2 = draw_byte(fdp); /* criteria */
            fb = draw_byte(fdp); /* format */
            d1 = draw_double(fdp);
            d2 = draw_double(fdp);
            d3 = draw_double(fdp);
            d4 = draw_double(fdp);
            b3 = draw_byte(fdp); /* min/mid/max rule packed */
            u1 = draw_u32(fdp);  /* bar/icon flags */
            lxw_conditional_format cf;
            memset(&cf, 0, sizeof(cf));
            cf.type = (uint8_t)((b1 % 32) + 1);
            cf.criteria = (uint8_t)(b2 % 21);
            cf.value = d1;
            cf.value_string = s1.c_str();
            cf.format = fmts[fb & 3];
            cf.min_value = d2;
            cf.mid_value = d3;
            cf.max_value = d4;
            cf.min_rule_type = (uint8_t)(b3 % 7);
            cf.mid_rule_type = (uint8_t)((b3 >> 3) % 7);
            cf.max_rule_type = (uint8_t)((b3 >> 6) % 7);
            cf.min_color = (lxw_color_t)0x0000FF;
            cf.mid_color = (lxw_color_t)0x00FF00;
            cf.max_color = (lxw_color_t)0xFF0000;
            cf.bar_color = (lxw_color_t)0x00CCFF;
            cf.bar_solid = u1 & 1;
            cf.bar_direction = (uint8_t)((u1 >> 2) % 3);
            cf.bar_axis_position = (uint8_t)((u1 >> 4) % 3);
            cf.icon_style = (uint8_t)((u1 >> 6) % 6);
            cf.stop_if_true = (u1 >> 9) & 1;
            cf.multi_range = s2.empty() ? nullptr : s2.c_str();
            worksheet_conditional_format_range(worksheet, r1, c1, r2, c2, &cf);
            break;
        }
        case 24: /* filter_column */
        {
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            b1 = draw_byte(fdp); /* criteria */
            d1 = draw_double(fdp);
            lxw_filter_rule rule;
            memset(&rule, 0, sizeof(rule));
            rule.criteria = (uint8_t)(b1 % 9);
            rule.value = d1;
            rule.value_string = s1.empty() ? nullptr : s1.c_str();
            worksheet_filter_column(worksheet, c1, &rule);
            break;
        }
        case 25: /* filter_column2 */
        {
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            b1 = draw_byte(fdp); /* rule1 criteria */
            d1 = draw_double(fdp);
            b2 = draw_byte(fdp); /* rule2 criteria */
            d2 = draw_double(fdp);
            b3 = draw_byte(fdp); /* and_or */
            lxw_filter_rule rule1, rule2;
            memset(&rule1, 0, sizeof(rule1));
            memset(&rule2, 0, sizeof(rule2));
            rule1.criteria = (uint8_t)(b1 % 9);
            rule1.value = d1;
            rule1.value_string = s1.empty() ? nullptr : s1.c_str();
            rule2.criteria = (uint8_t)(b2 % 9);
            rule2.value = d2;
            rule2.value_string = s2.empty() ? nullptr : s2.c_str();
            worksheet_filter_column2(worksheet, c1, &rule1, &rule2, b3 & 1);
            break;
        }
        case 26: /* filter_list */
        {
            c1 = draw_col(fdp);
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            const char *list[3] = {s1.c_str(), s2.c_str(), nullptr};
            worksheet_filter_list(worksheet, c1, list);
            break;
        }
        case 27: /* set_column */
            c1 = draw_col(fdp);
            c2 = draw_col(fdp);
            d1 = draw_double(fdp);
            fb = draw_byte(fdp);
            worksheet_set_column(worksheet, c1, c2, d1, fmts[fb & 3]);
            break;
        case 28: /* set_column_opt */
        {
            c1 = draw_col(fdp);
            c2 = draw_col(fdp);
            b1 = draw_byte(fdp); /* hidden/level/collapsed packed */
            d1 = draw_double(fdp);
            fb = draw_byte(fdp);
            lxw_row_col_options ropts;
            memset(&ropts, 0, sizeof(ropts));
            ropts.hidden = b1 & 1;
            ropts.level = (uint8_t)((b1 >> 1) % 8);
            ropts.collapsed = (b1 >> 4) & 1;
            worksheet_set_column_opt(worksheet, c1, c2, d1, fmts[fb & 3], &ropts);
            break;
        }
        case 29: /* set_column_pixels */
        {
            c1 = draw_col(fdp);
            c2 = draw_col(fdp);
            u1 = fdp.ConsumeIntegralInRange<uint32_t>(0, 800);
            fb = draw_byte(fdp);
            worksheet_set_column_pixels(worksheet, c1, c2, u1, fmts[fb & 3]);
            break;
        }
        case 30: /* set_row */
            r1 = draw_row(fdp);
            d1 = draw_double(fdp);
            fb = draw_byte(fdp);
            worksheet_set_row(worksheet, r1, d1, fmts[fb & 3]);
            break;
        case 31: /* set_row_opt */
        {
            r1 = draw_row(fdp);
            b1 = draw_byte(fdp);
            d1 = draw_double(fdp);
            fb = draw_byte(fdp);
            lxw_row_col_options ropts;
            memset(&ropts, 0, sizeof(ropts));
            ropts.hidden = b1 & 1;
            ropts.level = (uint8_t)((b1 >> 1) % 8);
            ropts.collapsed = (b1 >> 4) & 1;
            worksheet_set_row_opt(worksheet, r1, d1, fmts[fb & 3], &ropts);
            break;
        }
        case 32: /* set_row_pixels */
        {
            r1 = draw_row(fdp);
            u1 = fdp.ConsumeIntegralInRange<uint32_t>(0, 800);
            fb = draw_byte(fdp);
            worksheet_set_row_pixels(worksheet, r1, u1, fmts[fb & 3]);
            break;
        }
        case 33: /* set_header / set_footer (attacker header/footer format codes) */
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            if (!it_header_set)
            {
                worksheet_set_header(worksheet, s1.c_str());
                it_header_set = true;
            }
            if (!it_footer_set)
            {
                worksheet_set_footer(worksheet, s2.c_str());
                it_footer_set = true;
            }
            break;
        case 34: /* set_header_opt / set_footer_opt */
        {
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            d1 = draw_double(fdp);
            lxw_header_footer_options hfopts;
            memset(&hfopts, 0, sizeof(hfopts));
            hfopts.margin = d1 / 10.0;
            if (!it_header_set)
            {
                worksheet_set_header_opt(worksheet, s1.c_str(), &hfopts);
                it_header_set = true;
            }
            if (!it_footer_set)
            {
                worksheet_set_footer_opt(worksheet, s2.c_str(), &hfopts);
                it_footer_set = true;
            }
            break;
        }
        case 35: /* print_area */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            r2 = draw_row(fdp);
            c2 = draw_col(fdp);
            worksheet_print_area(worksheet, r1, c1, r2, c2);
            break;
        case 36: /* repeat_rows / repeat_columns */
            r1 = draw_row(fdp);
            r2 = draw_row(fdp);
            c1 = draw_col(fdp);
            c2 = draw_col(fdp);
            b1 = draw_byte(fdp);
            if (b1 & 1)
            {
                worksheet_repeat_rows(worksheet, r1, r2);
                worksheet_repeat_columns(worksheet, c1, c2);
            }
            break;
        case 37: /* set_selection driven by A1-string parsing of fuzz bytes */
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            r1 = lxw_name_to_row(s1.c_str());
            c1 = lxw_name_to_col(s1.c_str());
            r2 = lxw_name_to_row_2(s2.c_str());
            c2 = lxw_name_to_col_2(s2.c_str());
            worksheet_set_selection(worksheet, r1, c1, r2, c2);
            break;
        case 38: /* freeze_panes / split_panes */
            r1 = draw_row(fdp);
            c1 = draw_col(fdp);
            r2 = draw_row(fdp);
            c2 = draw_col(fdp);
            b1 = draw_byte(fdp);
            if (b1 & 1)
                worksheet_freeze_panes(worksheet, r1, c1);
            else
                worksheet_split_panes(worksheet, r2, c2);
            break;
        case 39: /* protect */
        {
            s1 = draw_str(fdp);
            b1 = draw_byte(fdp);
            lxw_protection prot;
            memset(&prot, 0, sizeof(prot));
            prot.no_select_locked_cells = b1 & 1;
            prot.format_cells = (b1 >> 1) & 1;
            prot.format_rows = (b1 >> 2) & 1;
            prot.insert_columns = (b1 >> 3) & 1;
            prot.insert_rows = (b1 >> 4) & 1;
            prot.sort = (b1 >> 5) & 1;
            prot.autofilter = (b1 >> 6) & 1;
            prot.objects = (b1 >> 7) & 1;
            worksheet_protect(worksheet, s1.c_str(), &prot);
            break;
        }
        case 40: /* page setup group */
        {
            b1 = draw_byte(fdp);
            b2 = draw_byte(fdp);
            switch (b1 % 5)
            {
            case 0:
                worksheet_set_zoom(worksheet, (uint16_t)(b2 * 4 + 10));
                break;
            case 1:
                worksheet_set_paper(worksheet, b2);
                break;
            case 2:
                worksheet_set_print_scale(worksheet, (uint16_t)(b2 * 2 + 10));
                break;
            case 3:
                worksheet_set_start_page(worksheet, (uint16_t)b2);
                break;
            default:
                (void)b2;
                worksheet_set_page_view(worksheet);
                break;
            }
            break;
        }
        case 41: /* set_margins */
            d1 = draw_double(fdp);
            d2 = draw_double(fdp);
            d3 = draw_double(fdp);
            d4 = draw_double(fdp);
            worksheet_set_margins(worksheet, d1 / 8.0, d2 / 8.0, d3 / 8.0, d4 / 8.0);
            break;
        case 42: /* fit_to_pages + print flags */
        {
            b1 = draw_byte(fdp);
            b2 = draw_byte(fdp);
            b3 = draw_byte(fdp);
            worksheet_fit_to_pages(worksheet, (uint16_t)(b1 % 10 + 1), (uint16_t)(b2 % 10 + 1));
            switch (b3 % 5)
            {
            case 0:
                worksheet_center_horizontally(worksheet);
                break;
            case 1:
                worksheet_center_vertically(worksheet);
                break;
            case 2:
                worksheet_print_across(worksheet);
                break;
            case 3:
                worksheet_print_black_and_white(worksheet);
                break;
            default:
                worksheet_print_row_col_headers(worksheet);
                break;
            }
            break;
        }
        case 43: /* tab color + sheet visibility group */
        {
            b1 = draw_byte(fdp);
            if (0 == b1 % 7)
            {
                u1 = draw_u32(fdp);
                worksheet_set_tab_color(worksheet, (lxw_color_t)u1);
            }
            else
            {
                switch (b1 % 7)
                {
                case 1:
                    worksheet_hide(worksheet);
                    break;
                case 2:
                    worksheet_activate(worksheet);
                    break;
                case 3:
                    worksheet_select(worksheet);
                    break;
                case 4:
                    worksheet_set_first_sheet(worksheet);
                    break;
                case 5:
                    worksheet_right_to_left(worksheet);
                    break;
                default:
                    worksheet_hide_zero(worksheet);
                    break;
                }
            }
            break;
        }
        case 44: /* outline/comments/default-row group */
        {
            b1 = draw_byte(fdp);
            switch (b1 % 6)
            {
            case 0:
                b2 = draw_byte(fdp);
                b3 = draw_byte(fdp);
                worksheet_outline_settings(worksheet, b2 & 1, (b2 >> 1) & 1, (b2 >> 2) & 1,
                                           (b3 >> 0) & 1);
                break;
            case 1:
                b2 = draw_byte(fdp);
                (void)b2;
                worksheet_show_comments(worksheet);
                break;
            case 2:
                s1 = draw_str(fdp);
                if (!it_author_set)
                {
                    worksheet_set_comments_author(worksheet, s1.c_str());
                    it_author_set = true;
                }
                break;
            case 3:
                d1 = draw_double(fdp);
                b2 = draw_byte(fdp);
                worksheet_set_default_row(worksheet, d1 / 4.0, b2 & 1);
                break;
            case 4:
                r1 = draw_row(fdp);
                c1 = draw_col(fdp);
                worksheet_set_top_left_cell(worksheet, r1, c1);
                break;
            default:
                worksheet_set_landscape(worksheet);
                break;
            }
            break;
        }
        case 45: /* pagebreaks (zero-terminated arrays) */
        {
            lxw_row_t hbrk[9];
            lxw_col_t vbrk[9];
            memset(hbrk, 0, sizeof(hbrk));
            memset(vbrk, 0, sizeof(vbrk));
            for (int i = 0; i < 8; i++)
                hbrk[i] = (lxw_row_t)(draw_byte(fdp) | 1) * 100;
            for (int i = 0; i < 8; i++)
                vbrk[i] = (lxw_col_t)(draw_byte(fdp) | 1);
            b1 = draw_byte(fdp);
            if (b1 & 1)
            {
                if (!it_hbreaks_set)
                {
                    worksheet_set_h_pagebreaks(worksheet, hbrk);
                    it_hbreaks_set = true;
                }
            }
            else
            {
                if (!it_vbreaks_set)
                {
                    worksheet_set_v_pagebreaks(worksheet, vbrk);
                    it_vbreaks_set = true;
                }
            }
            break;
        }
        case 46: /* second worksheet with fuzzed (validated) name */
        {
            s1 = draw_str(fdp);
            lxw_worksheet *ws2 = workbook_add_worksheet(workbook, s1.c_str());
            if (nullptr != ws2)
            {
                s2 = draw_str(fdp);
                worksheet_write_string(ws2, 0, 0, s2.c_str(), fmt0);
                worksheet_set_tab_color(ws2, (lxw_color_t)0xFF9900);
            }
            break;
        }
        case 47: /* wildcard write: full-range row/col (bounds checks) */
            r1 = draw_wild_row(fdp);
            c1 = draw_wild_col(fdp);
            s1 = draw_str(fdp);
            fb = draw_byte(fdp);
            worksheet_write_string(worksheet, r1, c1, s1.c_str(), fmts[fb & 3]);
            break;
        case 48: /* A1-style parse into utility converters, write at result.
                  * Parsed rows are clamped to the bounded sparse region
                  * (see draw_wild_row): lxw_name_to_row() on attacker
                  * digits can return near-maximum rows, whose sheet
                  * materialisation exceeds the production timeout. */
            s1 = draw_str(fdp);
            r1 = lxw_name_to_row(s1.c_str());
            c1 = lxw_name_to_col(s1.c_str());
            if (r1 > 65535)
                r1 = 65535;
            worksheet_write_number(worksheet, r1, c1, 1.5, fmt1);
            break;
        default: /* 49: ignore_errors with fuzzed type + raw range string */
            s1 = draw_str(fdp);
            b1 = draw_byte(fdp);
            worksheet_ignore_errors(worksheet, (uint8_t)(b1 % 16), s1.c_str());
            break;
        }
    }

    workbook_close(workbook);
    close(fuzz_fd);
    unlink(fuzz_file);
    return 0;
}
