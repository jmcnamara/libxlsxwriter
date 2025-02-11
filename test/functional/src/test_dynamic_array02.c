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

    lxw_workbook  *workbook  = workbook_new("test_dynamic_array02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_dynamic_formula(worksheet, CELL("B1"), "=_xlfn.UNIQUE(A1)", NULL);
    worksheet_write_number(worksheet, CELL("A1"), 0 , NULL);

    return workbook_close(workbook);
}
