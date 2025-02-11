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

    lxw_workbook  *workbook  = workbook_new("test_set_row01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_row(worksheet, 0, 0.75,  NULL);
    worksheet_set_row(worksheet, 1, 1.50,  NULL);
    worksheet_set_row(worksheet, 2, 2.25,  NULL);
    worksheet_set_row(worksheet, 3, 3,     NULL);

    worksheet_set_row(worksheet, 11, 9,     NULL);
    worksheet_set_row(worksheet, 12, 9.75,  NULL);
    worksheet_set_row(worksheet, 13, 10.50, NULL);
    worksheet_set_row(worksheet, 14, 11.25, NULL);

    worksheet_set_row(worksheet, 18, 14.25, NULL);
    worksheet_set_row(worksheet, 20, 15.75, NULL);
    worksheet_set_row(worksheet, 21, 16.50, NULL);

    return workbook_close(workbook);
}
