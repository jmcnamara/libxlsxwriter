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

    lxw_workbook  *workbook  = workbook_new("test_rich_string09.xlsx");
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

    lxw_rich_string_tuple *rich_strings1[] = {&fragment11, &fragment12, &fragment13, NULL};
    worksheet_write_rich_string(worksheet, CELL("A3"), rich_strings1, NULL);

    /* The following use cases fail validation and should be ignored. */

    /* NULL string. */
    lxw_rich_string_tuple fragment21 = {.format = NULL, .string = NULL};
    lxw_rich_string_tuple fragment22 = {.format = bold, .string = "bc"};
    lxw_rich_string_tuple fragment23 = {.format = NULL, .string = "defg"};

    lxw_rich_string_tuple *rich_strings2[] = {&fragment21, &fragment22, &fragment23, NULL};
    worksheet_write_rich_string(worksheet, CELL("A3"), rich_strings2, NULL);

    /* Empty string. */
    lxw_rich_string_tuple fragment31 = {.format = NULL, .string = ""};
    lxw_rich_string_tuple fragment32 = {.format = bold, .string = "bc"};
    lxw_rich_string_tuple fragment33 = {.format = NULL, .string = "defg"};

    lxw_rich_string_tuple *rich_strings3[] = {&fragment31, &fragment32, &fragment33, NULL};
    worksheet_write_rich_string(worksheet, CELL("A3"), rich_strings3, NULL);

    /* Empty array. */
    lxw_rich_string_tuple *rich_strings4[] = {NULL};
    worksheet_write_rich_string(worksheet, CELL("A3"), rich_strings4, NULL);

    /* Single string. */
    lxw_rich_string_tuple fragment51 = {.format = bold, .string = "foo"};
    lxw_rich_string_tuple *rich_strings5[] = {&fragment51, NULL};
    worksheet_write_rich_string(worksheet, CELL("A3"), rich_strings5, NULL);


    return workbook_close(workbook);
}
