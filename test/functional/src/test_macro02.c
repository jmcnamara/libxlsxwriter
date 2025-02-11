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

    lxw_workbook  *workbook  = workbook_new("test_macro02.xlsm");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_add_vba_project(workbook, "images/vbaProject03.bin");
    workbook_set_vba_name(workbook, "MyWorkbook");
    worksheet_set_vba_name(worksheet, "MySheet1");

    worksheet_write_number(worksheet, CELL("A1"), 123 , NULL);

    return workbook_close(workbook);
}
