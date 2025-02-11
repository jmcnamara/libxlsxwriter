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

    lxw_workbook_options options = {.constant_memory = LXW_TRUE};

    lxw_workbook  *workbook  = workbook_new_opt("test_optimize14.xlsx", &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, CELL("A1"),  "Foo", NULL);
    worksheet_write_string(worksheet, CELL("C7"),  "Bar", NULL);
    worksheet_write_string(worksheet, CELL("G14"), "Baz", NULL);

    worksheet_write_comment(worksheet, CELL("A1"),  "Some text");
    worksheet_write_comment(worksheet, CELL("D1"),  "Some text");
    worksheet_write_comment(worksheet, CELL("C7"),  "Some text");
    worksheet_write_comment(worksheet, CELL("E10"), "Some text");
    worksheet_write_comment(worksheet, CELL("G14"), "Some text");

    worksheet_set_comments_author(worksheet, "John");

    return workbook_close(workbook);
}
