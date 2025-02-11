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

    lxw_workbook  *workbook  = workbook_new("test_dynamic_array52.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_dynamic_formula(worksheet, CELL("A1"), "=AVERAGE(TIMEVALUE(B1:B2))", NULL);
    worksheet_write_string(worksheet, CELL("B1"), "12:00" , NULL);
    worksheet_write_string(worksheet, CELL("B2"), "12:00" , NULL);

    return workbook_close(workbook);
}
