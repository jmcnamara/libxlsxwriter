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

    lxw_workbook  *workbook  = workbook_new("test_table06.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("C:H"), 10.288, NULL);

    worksheet_add_table(worksheet, RANGE("C3:F13"), NULL);
    worksheet_add_table(worksheet, RANGE("F15:H20"), NULL);
    worksheet_add_table(worksheet, RANGE("C23:D30"), NULL);

    /* Add other objects to check rId handling. */
    workbook_unset_default_url_format(workbook);
    worksheet_write_url(worksheet, CELL("A1"), "http://perl.com/", NULL);
    worksheet_write_url(worksheet, CELL("C1"), "http://perl.com/", NULL);

    worksheet_set_comments_author(worksheet, "John");
    worksheet_write_comment(worksheet, CELL("H1"), "Test1");
    worksheet_write_comment(worksheet, CELL("J1"), "Test2");

    worksheet_insert_image(worksheet, CELL("A4"), "images/blue.png");

    return workbook_close(workbook);
}
