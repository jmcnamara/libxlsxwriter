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

    lxw_workbook  *workbook  = workbook_new("test_rich_string06.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *red_format = workbook_add_format(workbook);

    format_set_font_color(red_format, LXW_COLOR_RED);

    worksheet_write_string(worksheet, CELL("A1"), "Foo", red_format);
    worksheet_write_string(worksheet, CELL("A2"), "Bar", NULL);

    lxw_rich_string_tuple fragment1 = {.format = NULL,       .string = "ab"};
    lxw_rich_string_tuple fragment2 = {.format = red_format, .string = "cde"};
    lxw_rich_string_tuple fragment3 = {.format = NULL,       .string = "fg"};

    lxw_rich_string_tuple *rich_strings[] = {&fragment1, &fragment2, &fragment3, NULL};
    worksheet_write_rich_string(worksheet, CELL("A3"), rich_strings, NULL);

    return workbook_close(workbook);
}
