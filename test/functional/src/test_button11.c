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

    lxw_workbook  *workbook  = workbook_new("test_button11.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);

    worksheet_insert_button(worksheet1, CELL("C2"), NULL);

    worksheet_write_comment(worksheet2, CELL("B2"), "Some text");

    worksheet_write_comment(worksheet3, CELL("C3"), "More text");

    worksheet_set_comments_author(worksheet2, "John");
    worksheet_set_comments_author(worksheet3, "John");

    return workbook_close(workbook);
}
