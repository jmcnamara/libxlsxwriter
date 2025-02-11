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

    lxw_workbook  *workbook  = workbook_new("test_types11.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, 0, 0, 32, NULL);

    /* Test floating point formatting. */
    worksheet_write_number(worksheet, 0,  0,  0, NULL);
    worksheet_write_number(worksheet, 1,  0,  1, NULL);
    worksheet_write_number(worksheet, 2,  0, -1, NULL);
    worksheet_write_number(worksheet, 3,  0,  1.2, NULL);
    worksheet_write_number(worksheet, 4,  0, -1.2, NULL);
    worksheet_write_number(worksheet, 5,  0,  1.2E8, NULL);
    worksheet_write_number(worksheet, 6,  0,  1.2E+20, NULL);
    worksheet_write_number(worksheet, 7,  0,  1.2E-20, NULL);
    worksheet_write_number(worksheet, 8,  0, -1.2E+20, NULL);
    worksheet_write_number(worksheet, 9,  0, -1.2E-20, NULL);
    worksheet_write_number(worksheet, 10, 0,  1.E+100, NULL);
    worksheet_write_number(worksheet, 11, 0,  1.E-100, NULL);

    return workbook_close(workbook);
}
