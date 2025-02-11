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

    lxw_workbook  *workbook  = workbook_new("test_comment10.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_comment_options options = {.color = 0x98FE97};

    worksheet_write_string(worksheet, CELL("A1"), "Foo", NULL);
    worksheet_write_comment_opt(worksheet, CELL("B2"), "Some text", &options);

    worksheet_set_comments_author(worksheet, "John");

    return workbook_close(workbook);
}
