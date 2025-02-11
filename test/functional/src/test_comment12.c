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

    lxw_workbook  *workbook  = workbook_new("test_comment12.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_row(worksheet, 0, 21, NULL);
    worksheet_set_column(worksheet, 1, 1, 10, NULL);

    worksheet_write_string(worksheet, CELL("A1"), "Foo", NULL);
    worksheet_write_comment(worksheet, CELL("A1"), "Some text");

    worksheet_set_comments_author(worksheet, "John");

    return workbook_close(workbook);
}
