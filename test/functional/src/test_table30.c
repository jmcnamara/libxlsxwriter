/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_table30.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("C:F"), 10.288, NULL);

    worksheet_add_table(worksheet, RANGE("C3:F13"), NULL);

    worksheet_set_background(worksheet, "images/logo.jpg");

    return workbook_close(workbook);
}
