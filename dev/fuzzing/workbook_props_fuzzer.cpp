/*
 * workbook_props_fuzzer — fuzz the libxlsxwriter workbook-level surface
 * (src/workbook.c 1,132 dark lines + src/custom.c 83 + src/core.c 22 +
 * src/app.c 25).
 *
 * Threat model:
 *   * Untrusted input: document properties (title/author/company/... strings
 *     land escaped in core.xml and app.xml), custom property names/values,
 *     defined-name names and A1-style formula/range strings (parsed by
 *     utility.c converters), sheet names (validated then escaped), VBA
 *     code-names. The workbook/packager writers must not violate memory
 *     safety on any of it.
 *   * Trust boundary: the public workbook_* APIs driven here.
 *   * Held fixed: one main worksheet always present; output path (mkstemp
 *     temp file, unlinked after workbook_close).
 *   * Expected bug classes: string escaping (core/app/custom.xml), range
 *     string parsing bounds, sheet-name validation, custom property type
 *     handling, constant-memory/zip64 packager paths (workbook_new_opt).
 *
 * Workbook-level string setters (set_properties, set_vba_name) are applied at
 * most once per iteration, mirroring the legitimate idempotent pattern — the
 * setter-overwrite leak class is documented as findings LXW-1/LXW-2/LXW-4.
 *
 * The workbook zip64 mode is selected from the last input byte (the first
 * byte FuzzedDataProvider consumes), before workbook_new; constant-memory
 * is excluded (LXW-5/LXW-6 above).
 * Seeds from generate_workbook_props_seeds.py.
 */
#include <cstdint>
#include <cstring>
#include <string>
#include <unistd.h>

#include <fuzzer/FuzzedDataProvider.h>

#include "xlsxwriter.h"

namespace {

const std::string mem_dir = "/dev/shm";
const std::string file_template = "/wpfuzzXXXXXX";
char temp_file_dir[FILENAME_MAX] = {0};

const size_t kMaxStr = 48;
const uint8_t kNumOps = 9;

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

std::string draw_str(FuzzedDataProvider &fdp)
{
    return fdp.ConsumeRandomLengthString(kMaxStr);
}

double draw_double(FuzzedDataProvider &fdp)
{
    return (double)fdp.ConsumeIntegralInRange<int32_t>(-100000, 100000) / 64.0;
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

    /* Workbook mode comes from the tail byte (first FDP draw). */
    uint8_t mode = (size && data) ? data[size - 1] : 0;
    lxw_workbook_options options;
    memset(&options, 0, sizeof(options));
    /* Findings LXW-5/LXW-6 (real, upstream): constant-memory worksheets
     * never close optimize_tmpfile (worksheet.c:215 — one fd leaks per
     * worksheet, workbook_free has no close), so long constant-memory runs
     * exhaust fds (EMFILE); workbook_close then derefs
     * STAILQ_FIRST(self->sheets) unchecked after its own default-worksheet
     * fallback fails (workbook.c:2221-2223) and segfaults. The mode is
     * therefore excluded from the fuzzing loop (use_zip64 kept); units are
     * quarantined and the findings stay reproducible. Not silenced. */
    options.constant_memory = 0;
    options.use_zip64 = (mode >> 1) & 1;

    FuzzedDataProvider fdp(data, size);
    uint8_t mode_draw = draw_byte(fdp); /* consume the mode byte properly */
    (void)mode_draw;

    lxw_workbook *workbook = workbook_new_opt(fuzz_file, &options);
    if (nullptr == workbook)
    {
        close(fuzz_fd);
        unlink(fuzz_file);
        return 0;
    }

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, nullptr);
    /* add_worksheet returns NULL on documented error paths (e.g. tmpfile
     * exhaustion in constant-memory mode under fd pressure) — checked. */
    if (nullptr == worksheet)
    {
        workbook_close(workbook);
        close(fuzz_fd);
        unlink(fuzz_file);
        return 0;
    }
    worksheet_write_string(worksheet, 0, 0, "data", nullptr);

    bool props_set = false;
    bool vba_name_set = false;
    uint8_t ncustom = 0;
    uint8_t nsheets = 0;

    int guard = 0;
    while (fdp.remaining_bytes() > 2 && guard++ < 48)
    {
        uint8_t op = draw_byte(fdp);
        if (op >= kNumOps)
            continue;

        std::string s1, s2, s3, s4;
        uint8_t b1 = 0, b2 = 0;
        double d1 = 0.0;
        uint32_t u1 = 0;

        switch (op)
        {
        case 0: /* extra worksheet with fuzzed (validated) name */
            if (nsheets >= 2)
                break;
            s1 = draw_str(fdp);
            if (nullptr != workbook_add_worksheet(workbook, s1.c_str()))
                nsheets++;
            break;
        case 1: /* document properties (attacker strings) — once */
        {
            if (props_set)
                break;
            props_set = true;
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            s3 = draw_str(fdp);
            s4 = draw_str(fdp);
            b1 = draw_byte(fdp);
            lxw_doc_properties props;
            memset(&props, 0, sizeof(props));
            props.title = s1.c_str();
            props.author = s2.c_str();
            props.company = s3.c_str();
            props.category = s4.c_str();
            props.keywords = s1.empty() ? nullptr : s1.c_str();
            props.comments = s2.c_str();
            props.manager = s3.c_str();
            props.subject = s4.c_str();
            workbook_set_properties(workbook, &props);
            if (b1 & 1)
                workbook_read_only_recommended(workbook);
            break;
        }
        case 2: /* custom properties (5 types) */
        {
            if (ncustom >= 4)
                break;
            ncustom++;
            b1 = draw_byte(fdp);
            s1 = draw_str(fdp);
            d1 = draw_double(fdp);
            u1 = draw_u32(fdp);
            b2 = draw_byte(fdp);
            switch (b1 % 5)
            {
            case 0:
                workbook_set_custom_property_string(workbook, s1.c_str(), s1.c_str());
                break;
            case 1:
                workbook_set_custom_property_number(workbook, s1.c_str(), d1);
                break;
            case 2:
                workbook_set_custom_property_integer(workbook, s1.c_str(), (int32_t)u1);
                break;
            case 3:
                workbook_set_custom_property_boolean(workbook, s1.c_str(), b2 & 1);
                break;
            default:
            {
                lxw_datetime dt;
                memset(&dt, 0, sizeof(dt));
                dt.year = 2020.0;
                dt.month = 1.0 + (double)(b2 % 12);
                dt.day = 1.0 + (double)((b2 >> 2) % 28);
                workbook_set_custom_property_datetime(workbook, s1.c_str(), &dt);
                break;
            }
            }
            break;
        }
        case 3: /* defined names (A1-style formula strings) */
            s1 = draw_str(fdp);
            s2 = draw_str(fdp);
            workbook_define_name(workbook, s1.c_str(), s2.c_str());
            break;
        case 4: /* sheet name validation + lookup */
            s1 = draw_str(fdp);
            workbook_validate_sheet_name(workbook, s1.c_str());
            workbook_get_worksheet_by_name(workbook, s1.c_str());
            break;
        case 5: /* epoch / size / vba name (once) */
        {
            b1 = draw_byte(fdp);
            s1 = draw_str(fdp);
            if (b1 & 1)
                workbook_use_1904_epoch(workbook);
            workbook_set_size(workbook, (uint16_t)(draw_byte(fdp) * 100),
                              (uint16_t)(draw_byte(fdp) * 100));
            if (!vba_name_set && !s1.empty())
            {
                workbook_set_vba_name(workbook, s1.c_str());
                vba_name_set = true;
            }
            break;
        }
        case 6: /* rows of cells (constant-memory ordering exercise) */
            for (lxw_row_t r = 1; r < 4; r++)
                worksheet_write_number(worksheet, r, (lxw_col_t)(r % 8),
                                       (double)r, nullptr);
            break;
        case 7: /* URL cell (hyperlink rels on doc-props workbooks) */
            s1 = draw_str(fdp);
            worksheet_write_url(worksheet, 5, 0, s1.c_str(), nullptr);
            break;
        default: /* datetime write */
        {
            b1 = draw_byte(fdp);
            lxw_datetime dt;
            memset(&dt, 0, sizeof(dt));
            dt.year = 1900 + (double)(b1 % 100);
            dt.month = 1 + (double)((b1 >> 2) % 12);
            dt.day = 1 + (double)((b1 >> 4) % 28);
            worksheet_write_datetime(worksheet, 6, 1, &dt, nullptr);
            break;
        }
        }
    }

    workbook_close(workbook);
    close(fuzz_fd);
    unlink(fuzz_file);
    return 0;
}
