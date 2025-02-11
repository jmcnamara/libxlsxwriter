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

    lxw_workbook  *workbook  = workbook_new("test_comment06.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_comment_options options = {.visible = LXW_COMMENT_DISPLAY_VISIBLE};


    worksheet_write_comment(worksheet, CELL("A1"), "Some text");
    worksheet_write_comment(worksheet, CELL("A2"), "Some text");

    worksheet_write_comment_opt(worksheet, CELL("A3"), "Some text", &options);

    worksheet_write_comment(worksheet, CELL("A4"), "Some text");
    worksheet_write_comment(worksheet, CELL("A5"), "Some text");

    worksheet_set_comments_author(worksheet, "John");

    return workbook_close(workbook);
}
