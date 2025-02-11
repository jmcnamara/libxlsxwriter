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

    lxw_workbook  *workbook  = workbook_new("test_default_row01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_default_row(worksheet, 24, LXW_FALSE);

    worksheet_write_string(worksheet, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet, CELL("A10"), "Bar" , NULL);

    return workbook_close(workbook);
}
