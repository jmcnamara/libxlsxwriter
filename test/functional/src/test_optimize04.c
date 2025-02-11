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

    lxw_workbook_options options = {LXW_TRUE, NULL, LXW_FALSE, NULL, NULL};

    lxw_workbook  *workbook  = workbook_new_opt("test_optimize04.xlsx", &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *bold = workbook_add_format(workbook);
    lxw_format *italic = workbook_add_format(workbook);

    format_set_bold(bold);
    format_set_italic(italic);

    worksheet_write_string(worksheet, CELL("A1"), "Foo", bold);
    worksheet_write_string(worksheet, CELL("A2"), "Bar", italic);

    lxw_rich_string_tuple fragment1 = {.format = NULL, .string = "a"};
    lxw_rich_string_tuple fragment2 = {.format = bold, .string = "bc"};
    lxw_rich_string_tuple fragment3 = {.format = NULL, .string = "defg"};

    lxw_rich_string_tuple *rich_strings[] = {&fragment1, &fragment2, &fragment3, NULL};
    worksheet_write_rich_string(worksheet, CELL("A3"), rich_strings, NULL);

    return workbook_close(workbook);
}
