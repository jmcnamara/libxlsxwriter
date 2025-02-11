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

    lxw_workbook  *workbook  = workbook_new("test_image27.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_insert_image(worksheet, CELL("B2"), "images/mylogo.png");

    return workbook_close(workbook);
}
