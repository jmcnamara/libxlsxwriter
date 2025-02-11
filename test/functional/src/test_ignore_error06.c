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

    lxw_workbook  *workbook  = workbook_new("test_ignore_error06.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_formula(worksheet, CELL("A1"), "=B1" , NULL);
    worksheet_write_formula(worksheet, CELL("A2"), "=B1" , NULL);
    worksheet_write_formula(worksheet, CELL("A3"), "=B3" , NULL);

    worksheet_ignore_errors(worksheet, LXW_IGNORE_FORMULA_DIFFERS, "A2");

    return workbook_close(workbook);
}
