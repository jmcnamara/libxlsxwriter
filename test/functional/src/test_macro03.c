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

    lxw_workbook  *workbook  = workbook_new("test_macro03.xlsm");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "Foo");

    workbook_add_vba_project(workbook, "images/vbaProject04.bin");

    worksheet_write_number(worksheet, CELL("A1"), 123 , NULL);

    return workbook_close(workbook);
}
