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

    lxw_workbook  *workbook  = workbook_new("test_comment05.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);
    uint32_t row;
    uint16_t col;

    (void)worksheet2;

    for (row = 0; row <= 127; row++)
        for (col = 0; col <= 15; col++)
            worksheet_write_comment(worksheet1, row, col, "Some text");

    worksheet_write_comment(worksheet3, CELL("A1"), "More text");

    worksheet_set_comments_author(worksheet1, "John");
    worksheet_set_comments_author(worksheet3, "John");

    return workbook_close(workbook);
}
