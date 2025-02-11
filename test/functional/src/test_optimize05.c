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

    lxw_workbook  *workbook  = workbook_new_opt("test_optimize05.xlsx", &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *bold = workbook_add_format(workbook);
    lxw_format *italic = workbook_add_format(workbook);

    format_set_bold(bold);
    format_set_italic(italic);

    worksheet_write_string(worksheet, CELL("A1"), "Foo", bold);
    worksheet_write_string(worksheet, CELL("A2"), "Bar", italic);

    lxw_rich_string_tuple fragment11 = {.format = NULL, .string = "a"};
    lxw_rich_string_tuple fragment12 = {.format = bold, .string = "bc"};
    lxw_rich_string_tuple fragment13 = {.format = NULL, .string = "defg"};

    lxw_rich_string_tuple fragment21 = {.format = NULL, .string = "a"};
    lxw_rich_string_tuple fragment22 = {.format = bold, .string = "bcdef"};
    lxw_rich_string_tuple fragment23 = {.format = NULL, .string = "g"};

    lxw_rich_string_tuple fragment31 = {.format = NULL,   .string = "abc"};
    lxw_rich_string_tuple fragment32 = {.format = italic, .string = "de"};
    lxw_rich_string_tuple fragment33 = {.format = NULL,   .string = "fg"};

    lxw_rich_string_tuple fragment41 = {.format = italic, .string = "abcd"};
    lxw_rich_string_tuple fragment42 = {.format = NULL,   .string = "efg"};

    lxw_rich_string_tuple *rich_strings1[] = {&fragment11, &fragment12, &fragment13, NULL};
    lxw_rich_string_tuple *rich_strings2[] = {&fragment21, &fragment22, &fragment23, NULL};
    lxw_rich_string_tuple *rich_strings3[] = {&fragment31, &fragment32, &fragment33, NULL};
    lxw_rich_string_tuple *rich_strings4[] = {&fragment41, &fragment42, NULL};


    worksheet_write_rich_string(worksheet, CELL("A3"), rich_strings1, NULL);
    worksheet_write_rich_string(worksheet, CELL("B4"), rich_strings3, NULL);
    worksheet_write_rich_string(worksheet, CELL("C5"), rich_strings1, NULL);
    worksheet_write_rich_string(worksheet, CELL("D6"), rich_strings3, NULL);
    worksheet_write_rich_string(worksheet, CELL("E7"), rich_strings2, NULL);
    worksheet_write_rich_string(worksheet, CELL("F8"), rich_strings4, NULL);

    return workbook_close(workbook);
}
