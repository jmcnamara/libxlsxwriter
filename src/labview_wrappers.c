/*
 * labview_wrappers.c - LabVIEW wrapper functions with ANSI to UTF-8 conversion
 *
 * These functions wrap the standard libxlsxwriter functions to convert
 * ANSI-encoded strings (from LabVIEW) to UTF-8 before passing to the library.
 *
 * LabVIEW uses unsigned long (32-bit) to represent opaque pointer handles.
 * These wrapper functions cast between the handle representation and actual pointers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 */

#include "xlsxwriter.h"

#ifdef _WIN32
#include <windows.h>
#include <stdlib.h>
#include <string.h>

/* Handle type for LabVIEW compatibility (32-bit on x86, 64-bit handles need uintptr_t) */
typedef uintptr_t lxw_handle;

/*
 * Convert ANSI string to UTF-8.
 * Returns newly allocated UTF-8 string (caller must free) or NULL on failure.
 */
static char *
ansi_to_utf8(const char *ansi_str)
{
    if (!ansi_str || !*ansi_str)
        return NULL;

    /* First convert ANSI to UTF-16 */
    int wide_len = MultiByteToWideChar(CP_ACP, 0, ansi_str, -1, NULL, 0);
    if (wide_len == 0)
        return NULL;

    wchar_t *wide_str = (wchar_t *) malloc(wide_len * sizeof(wchar_t));
    if (!wide_str)
        return NULL;

    if (MultiByteToWideChar(CP_ACP, 0, ansi_str, -1, wide_str, wide_len) == 0) {
        free(wide_str);
        return NULL;
    }

    /* Then convert UTF-16 to UTF-8 */
    int utf8_len =
        WideCharToMultiByte(CP_UTF8, 0, wide_str, -1, NULL, 0, NULL, NULL);
    if (utf8_len == 0) {
        free(wide_str);
        return NULL;
    }

    char *utf8_str = (char *) malloc(utf8_len);
    if (!utf8_str) {
        free(wide_str);
        return NULL;
    }

    if (WideCharToMultiByte
        (CP_UTF8, 0, wide_str, -1, utf8_str, utf8_len, NULL, NULL) == 0) {
        free(wide_str);
        free(utf8_str);
        return NULL;
    }

    free(wide_str);
    return utf8_str;
}

#else
/* On non-Windows, assume strings are already UTF-8 */
#include <stdlib.h>
#include <string.h>

static char *
ansi_to_utf8(const char *str)
{
    if (!str)
        return NULL;
    return strdup(str);
}
#endif

/* ============================================================================
 * Worksheet write functions
 * ============================================================================ */

lxw_error
worksheet_write_string_lv(lxw_worksheet *worksheet, lxw_row_t row,
                          lxw_col_t col, const char *string,
                          lxw_format *format)
{
    char *utf8 = ansi_to_utf8(string);
    lxw_error err =
        worksheet_write_string(worksheet, row, col, utf8 ? utf8 : string,
                               format);
    free(utf8);
    return err;
}

lxw_error
worksheet_write_formula_lv(lxw_worksheet *worksheet, lxw_row_t row,
                           lxw_col_t col, const char *formula,
                           lxw_format *format)
{
    char *utf8 = ansi_to_utf8(formula);
    lxw_error err =
        worksheet_write_formula(worksheet, row, col, utf8 ? utf8 : formula,
                                format);
    free(utf8);
    return err;
}

lxw_error
worksheet_write_url_lv(lxw_worksheet *worksheet, lxw_row_t row, lxw_col_t col,
                       const char *url, lxw_format *format)
{
    char *utf8 = ansi_to_utf8(url);
    lxw_error err =
        worksheet_write_url(worksheet, row, col, utf8 ? utf8 : url, format);
    free(utf8);
    return err;
}

lxw_error
worksheet_write_comment_lv(lxw_worksheet *worksheet, lxw_row_t row,
                           lxw_col_t col, const char *string)
{
    char *utf8 = ansi_to_utf8(string);
    lxw_error err =
        worksheet_write_comment(worksheet, row, col, utf8 ? utf8 : string);
    free(utf8);
    return err;
}

lxw_error
worksheet_set_header_lv(lxw_worksheet *worksheet, const char *header)
{
    char *utf8 = ansi_to_utf8(header);
    lxw_error err = worksheet_set_header(worksheet, utf8 ? utf8 : header);
    free(utf8);
    return err;
}

lxw_error
worksheet_set_footer_lv(lxw_worksheet *worksheet, const char *footer)
{
    char *utf8 = ansi_to_utf8(footer);
    lxw_error err = worksheet_set_footer(worksheet, utf8 ? utf8 : footer);
    free(utf8);
    return err;
}

lxw_error
worksheet_merge_range_lv(lxw_worksheet *worksheet, lxw_row_t first_row,
                         lxw_col_t first_col, lxw_row_t last_row,
                         lxw_col_t last_col, const char *string,
                         lxw_format *format)
{
    char *utf8 = ansi_to_utf8(string);
    lxw_error err =
        worksheet_merge_range(worksheet, first_row, first_col, last_row,
                              last_col,
                              utf8 ? utf8 : string, format);
    free(utf8);
    return err;
}

void
worksheet_set_comments_author_lv(lxw_worksheet *worksheet, const char *author)
{
    char *utf8 = ansi_to_utf8(author);
    worksheet_set_comments_author(worksheet, utf8 ? utf8 : author);
    free(utf8);
}

lxw_error
worksheet_insert_textbox_lv(lxw_worksheet *worksheet, lxw_row_t row,
                            lxw_col_t col, const char *text)
{
    char *utf8 = ansi_to_utf8(text);
    lxw_error err =
        worksheet_insert_textbox(worksheet, row, col, utf8 ? utf8 : text);
    free(utf8);
    return err;
}

lxw_error
worksheet_insert_textbox_opt_lv(lxw_worksheet *worksheet, lxw_row_t row,
                                lxw_col_t col, const char *text,
                                lxw_textbox_options *options)
{
    char *utf8 = ansi_to_utf8(text);
    lxw_error err =
        worksheet_insert_textbox_opt(worksheet, row, col, utf8 ? utf8 : text,
                                     options);
    free(utf8);
    return err;
}

/* ============================================================================
 * Chart functions
 * ============================================================================ */

lxw_chart_series *
chart_add_series_lv(lxw_chart *chart, const char *categories,
                    const char *values, uint8_t y2_axis)
{
    char *utf8_cat = ansi_to_utf8(categories);
    char *utf8_val = ansi_to_utf8(values);
    lxw_chart_series *series = chart_add_series_impl(chart,
                                                     utf8_cat ? utf8_cat :
                                                     categories,
                                                     utf8_val ? utf8_val :
                                                     values,
                                                     y2_axis);
    free(utf8_cat);
    free(utf8_val);
    return series;
}

void
chart_series_set_name_lv(lxw_chart_series *series, const char *name)
{
    char *utf8 = ansi_to_utf8(name);
    chart_series_set_name(series, utf8 ? utf8 : name);
    free(utf8);
}

void
chart_axis_set_name_lv(lxw_chart_axis *axis, const char *name)
{
    char *utf8 = ansi_to_utf8(name);
    chart_axis_set_name(axis, utf8 ? utf8 : name);
    free(utf8);
}

void
chart_title_set_name_lv(lxw_chart *chart, const char *name)
{
    char *utf8 = ansi_to_utf8(name);
    chart_title_set_name(chart, utf8 ? utf8 : name);
    free(utf8);
}

void
chart_series_set_trendline_name_lv(lxw_chart_series *series, const char *name)
{
    char *utf8 = ansi_to_utf8(name);
    chart_series_set_trendline_name(series, utf8 ? utf8 : name);
    free(utf8);
}

void
chart_axis_set_num_format_lv(lxw_chart_axis *axis, const char *num_format)
{
    char *utf8 = ansi_to_utf8(num_format);
    chart_axis_set_num_format(axis, utf8 ? utf8 : num_format);
    free(utf8);
}

void
chart_series_set_labels_num_format_lv(lxw_chart_series *series,
                                      const char *num_format)
{
    char *utf8 = ansi_to_utf8(num_format);
    chart_series_set_labels_num_format(series, utf8 ? utf8 : num_format);
    free(utf8);
}

void
chart_series_set_categories_lv(lxw_chart_series *series,
                               const char *sheetname, lxw_row_t first_row,
                               lxw_col_t first_col, lxw_row_t last_row,
                               lxw_col_t last_col)
{
    char *utf8 = ansi_to_utf8(sheetname);
    chart_series_set_categories(series, utf8 ? utf8 : sheetname,
                                first_row, first_col, last_row, last_col);
    free(utf8);
}

void
chart_series_set_values_lv(lxw_chart_series *series, const char *sheetname,
                           lxw_row_t first_row, lxw_col_t first_col,
                           lxw_row_t last_row, lxw_col_t last_col)
{
    char *utf8 = ansi_to_utf8(sheetname);
    chart_series_set_values(series, utf8 ? utf8 : sheetname,
                            first_row, first_col, last_row, last_col);
    free(utf8);
}

void
chart_series_set_name_range_lv(lxw_chart_series *series,
                               const char *sheetname, lxw_row_t row,
                               lxw_col_t col)
{
    char *utf8 = ansi_to_utf8(sheetname);
    chart_series_set_name_range(series, utf8 ? utf8 : sheetname, row, col);
    free(utf8);
}

void
chart_axis_set_name_range_lv(lxw_chart_axis *axis, const char *sheetname,
                             lxw_row_t row, lxw_col_t col)
{
    char *utf8 = ansi_to_utf8(sheetname);
    chart_axis_set_name_range(axis, utf8 ? utf8 : sheetname, row, col);
    free(utf8);
}

void
chart_title_set_name_range_lv(lxw_chart *chart, const char *sheetname,
                              lxw_row_t row, lxw_col_t col)
{
    char *utf8 = ansi_to_utf8(sheetname);
    chart_title_set_name_range(chart, utf8 ? utf8 : sheetname, row, col);
    free(utf8);
}

/* ============================================================================
 * Format functions
 * ============================================================================ */

void
format_set_font_name_lv(lxw_format *format, const char *font_name)
{
    char *utf8 = ansi_to_utf8(font_name);
    format_set_font_name(format, utf8 ? utf8 : font_name);
    free(utf8);
}

void
format_set_num_format_lv(lxw_format *format, const char *num_format)
{
    char *utf8 = ansi_to_utf8(num_format);
    format_set_num_format(format, utf8 ? utf8 : num_format);
    free(utf8);
}

/* ============================================================================
 * Workbook functions
 * ============================================================================ */

lxw_worksheet *
workbook_add_worksheet_lv(lxw_workbook *workbook, const char *sheetname)
{
    /* Pass NULL to get default Sheet1, Sheet2, etc. names */
    if (!sheetname || !*sheetname) {
        return workbook_add_worksheet(workbook, NULL);
    }
    char *utf8 = ansi_to_utf8(sheetname);
    lxw_worksheet *ws =
        workbook_add_worksheet(workbook, utf8 ? utf8 : sheetname);
    free(utf8);
    return ws;
}

lxw_chartsheet *
workbook_add_chartsheet_lv(lxw_workbook *workbook, const char *sheetname)
{
    /* Pass NULL to get default Chart1, Chart2, etc. names */
    if (!sheetname || !*sheetname) {
        return workbook_add_chartsheet(workbook, NULL);
    }
    char *utf8 = ansi_to_utf8(sheetname);
    lxw_chartsheet *cs =
        workbook_add_chartsheet(workbook, utf8 ? utf8 : sheetname);
    free(utf8);
    return cs;
}

lxw_error
workbook_define_name_lv(lxw_workbook *workbook, const char *name,
                        const char *formula)
{
    char *utf8_name = ansi_to_utf8(name);
    char *utf8_formula = ansi_to_utf8(formula);
    lxw_error err = workbook_define_name(workbook,
                                         utf8_name ? utf8_name : name,
                                         utf8_formula ? utf8_formula :
                                         formula);
    free(utf8_name);
    free(utf8_formula);
    return err;
}

lxw_worksheet *
workbook_get_worksheet_by_name_lv(lxw_workbook *workbook, const char *name)
{
    char *utf8 = ansi_to_utf8(name);
    lxw_worksheet *ws =
        workbook_get_worksheet_by_name(workbook, utf8 ? utf8 : name);
    free(utf8);
    return ws;
}

lxw_chartsheet *
workbook_get_chartsheet_by_name_lv(lxw_workbook *workbook, const char *name)
{
    char *utf8 = ansi_to_utf8(name);
    lxw_chartsheet *cs =
        workbook_get_chartsheet_by_name(workbook, utf8 ? utf8 : name);
    free(utf8);
    return cs;
}

lxw_error
workbook_validate_sheet_name_lv(lxw_workbook *workbook, const char *sheetname)
{
    char *utf8 = ansi_to_utf8(sheetname);
    lxw_error err =
        workbook_validate_sheet_name(workbook, utf8 ? utf8 : sheetname);
    free(utf8);
    return err;
}

lxw_error
workbook_set_custom_property_string_lv(lxw_workbook *workbook,
                                       const char *name, const char *value)
{
    char *utf8_name = ansi_to_utf8(name);
    char *utf8_value = ansi_to_utf8(value);
    lxw_error err = workbook_set_custom_property_string(workbook,
                                                        utf8_name ? utf8_name
                                                        : name,
                                                        utf8_value ?
                                                        utf8_value : value);
    free(utf8_name);
    free(utf8_value);
    return err;
}

/* ============================================================================
 * Chartsheet functions
 * ============================================================================ */

lxw_error
chartsheet_set_header_lv(lxw_chartsheet *chartsheet, const char *header)
{
    char *utf8 = ansi_to_utf8(header);
    lxw_error err = chartsheet_set_header(chartsheet, utf8 ? utf8 : header);
    free(utf8);
    return err;
}

lxw_error
chartsheet_set_footer_lv(lxw_chartsheet *chartsheet, const char *footer)
{
    char *utf8 = ansi_to_utf8(footer);
    lxw_error err = chartsheet_set_footer(chartsheet, utf8 ? utf8 : footer);
    free(utf8);
    return err;
}

/* ============================================================================
 * File path functions (ANSI to UTF-8 conversion for lxw_fopen compatibility)
 * ============================================================================ */

lxw_workbook *
workbook_new_lv(const char *filename)
{
    char *utf8 = ansi_to_utf8(filename);
    lxw_workbook *wb = workbook_new(utf8 ? utf8 : filename);
    free(utf8);
    return wb;
}

lxw_workbook *
workbook_new_opt_lv(const char *filename, lxw_workbook_options *options)
{
    char *utf8 = ansi_to_utf8(filename);
    lxw_workbook *wb = workbook_new_opt(utf8 ? utf8 : filename, options);
    free(utf8);
    return wb;
}

lxw_error
worksheet_insert_image_lv(lxw_worksheet *worksheet, lxw_row_t row,
                          lxw_col_t col, const char *filename)
{
    char *utf8 = ansi_to_utf8(filename);
    lxw_error err =
        worksheet_insert_image(worksheet, row, col, utf8 ? utf8 : filename);
    free(utf8);
    return err;
}

lxw_error
worksheet_insert_image_opt_lv(lxw_worksheet *worksheet, lxw_row_t row,
                              lxw_col_t col, const char *filename,
                              lxw_image_options *options)
{
    char *utf8 = ansi_to_utf8(filename);
    lxw_error err = worksheet_insert_image_opt(worksheet, row, col,
                                               utf8 ? utf8 : filename,
                                               options);
    free(utf8);
    return err;
}

lxw_error
worksheet_embed_image_lv(lxw_worksheet *worksheet, lxw_row_t row,
                         lxw_col_t col, const char *filename)
{
    char *utf8 = ansi_to_utf8(filename);
    lxw_error err =
        worksheet_embed_image(worksheet, row, col, utf8 ? utf8 : filename);
    free(utf8);
    return err;
}

lxw_error
worksheet_embed_image_opt_lv(lxw_worksheet *worksheet, lxw_row_t row,
                             lxw_col_t col, const char *filename,
                             lxw_image_options *options)
{
    char *utf8 = ansi_to_utf8(filename);
    lxw_error err =
        worksheet_embed_image_opt(worksheet, row, col, utf8 ? utf8 : filename,
                                  options);
    free(utf8);
    return err;
}

lxw_error
worksheet_set_background_lv(lxw_worksheet *worksheet, const char *filename)
{
    char *utf8 = ansi_to_utf8(filename);
    lxw_error err =
        worksheet_set_background(worksheet, utf8 ? utf8 : filename);
    free(utf8);
    return err;
}

lxw_error
workbook_add_vba_project_lv(lxw_workbook *workbook, const char *filename)
{
    char *utf8 = ansi_to_utf8(filename);
    lxw_error err =
        workbook_add_vba_project(workbook, utf8 ? utf8 : filename);
    free(utf8);
    return err;
}

lxw_error
workbook_add_signed_vba_project_lv(lxw_workbook *workbook,
                                   const char *vba_project,
                                   const char *signature)
{
    char *utf8_vba = ansi_to_utf8(vba_project);
    char *utf8_sig = ansi_to_utf8(signature);
    lxw_error err = workbook_add_signed_vba_project(workbook,
                                                    utf8_vba ? utf8_vba :
                                                    vba_project,
                                                    utf8_sig ? utf8_sig :
                                                    signature);
    free(utf8_vba);
    free(utf8_sig);
    return err;
}

/* ============================================================================
 * Autofilter functions
 * ============================================================================ */

lxw_error
worksheet_filter_column_lv(lxw_worksheet *worksheet, lxw_col_t col,
                           uint8_t criteria, const char *value_string,
                           double value)
{
    lxw_filter_rule rule = { 0 };
    rule.criteria = criteria;
    rule.value = value;

    if (value_string && *value_string) {
        char *utf8 = ansi_to_utf8(value_string);
        rule.value_string = utf8 ? utf8 : value_string;
        lxw_error err = worksheet_filter_column(worksheet, col, &rule);
        free(utf8);
        return err;
    }
    else {
        rule.value_string = NULL;
        return worksheet_filter_column(worksheet, col, &rule);
    }
}

lxw_error
worksheet_filter_column2_lv(lxw_worksheet *worksheet, lxw_col_t col,
                            uint8_t criteria1, const char *value_string1,
                            double value1, uint8_t criteria2,
                            const char *value_string2, double value2,
                            uint8_t and_or)
{
    lxw_filter_rule rule1 = { 0 };
    lxw_filter_rule rule2 = { 0 };
    char *utf8_1 = NULL;
    char *utf8_2 = NULL;

    rule1.criteria = criteria1;
    rule1.value = value1;
    rule2.criteria = criteria2;
    rule2.value = value2;

    if (value_string1 && *value_string1) {
        utf8_1 = ansi_to_utf8(value_string1);
        rule1.value_string = utf8_1 ? utf8_1 : value_string1;
    }

    if (value_string2 && *value_string2) {
        utf8_2 = ansi_to_utf8(value_string2);
        rule2.value_string = utf8_2 ? utf8_2 : value_string2;
    }

    lxw_error err =
        worksheet_filter_column2(worksheet, col, &rule1, &rule2, and_or);

    free(utf8_1);
    free(utf8_2);
    return err;
}

/* ============================================================================
 * Chart data label functions
 * ============================================================================ */

lxw_error
chart_series_set_labels_custom_lv(lxw_chart_series *series,
                                  uintptr_t *values,
                                  uint8_t *hide_flags, uint16_t count)
{
    lxw_error err = LXW_NO_ERROR;
    uint16_t i;
    lxw_chart_data_label *labels = NULL;
    lxw_chart_data_label **label_ptrs = NULL;
    char **utf8_values = NULL;

    if (!series || count == 0)
        return LXW_ERROR_NULL_PARAMETER_IGNORED;

    /* Allocate arrays */
    labels =
        (lxw_chart_data_label *) calloc(count, sizeof(lxw_chart_data_label));
    label_ptrs =
        (lxw_chart_data_label **) calloc(count + 1,
                                         sizeof(lxw_chart_data_label *));
    utf8_values = (char **) calloc(count, sizeof(char *));

    if (!labels || !label_ptrs || !utf8_values) {
        free(labels);
        free(label_ptrs);
        free(utf8_values);
        return LXW_ERROR_MEMORY_MALLOC_FAILED;
    }

    /* Build the label structs */
    for (i = 0; i < count; i++) {
        const char *str;

        /* Set up pointer array */
        label_ptrs[i] = &labels[i];

        /* Get string pointer from uintptr_t array */
        str = (values && values[i]) ? (const char *) values[i] : NULL;

        /* Convert value string if provided */
        if (str && str[0]) {
            utf8_values[i] = ansi_to_utf8(str);
            labels[i].value = utf8_values[i] ? utf8_values[i] : str;
        }
        else {
            labels[i].value = NULL;
        }

        /* Set hide flag */
        if (hide_flags) {
            labels[i].hide = hide_flags[i];
        }

        /* Other fields remain NULL (font, line, fill, pattern) */
    }

    /* NULL-terminate the pointer array */
    label_ptrs[count] = NULL;

    /* Call the library function */
    err = chart_series_set_labels_custom(series, label_ptrs);

    /* Free allocated memory */
    for (i = 0; i < count; i++) {
        free(utf8_values[i]);
    }
    free(utf8_values);
    free(labels);
    free(label_ptrs);

    return err;
}
