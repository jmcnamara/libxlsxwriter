/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_rich_string12.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *bold = workbook_add_format(workbook);
    lxw_format *italic = workbook_add_format(workbook);
    lxw_format *wrap = workbook_add_format(workbook);

    format_set_bold(bold);
    format_set_italic(italic);
    format_set_text_wrap(wrap);

    worksheet_set_column(worksheet, 0, 0, 30, NULL);
    worksheet_set_row(worksheet, 2, 60, NULL);

    worksheet_write_string(worksheet, CELL("A1"), "Foo", bold);
    worksheet_write_string(worksheet, CELL("A2"), "Bar", italic);

    lxw_rich_string_tuple fragment1 = {.format = NULL,   .string = "This is\n"};
    lxw_rich_string_tuple fragment2 = {.format = bold,   .string = "bold\n"};
    lxw_rich_string_tuple fragment3 = {.format = NULL,   .string = "and this is\n"};
    lxw_rich_string_tuple fragment4 = {.format = italic, .string = "italic"};

    lxw_rich_string_tuple *rich_strings[] = {&fragment1, &fragment2, &fragment3, &fragment4, NULL};
    worksheet_write_rich_string(worksheet, CELL("A3"), rich_strings, wrap);

    return workbook_close(workbook);
}
