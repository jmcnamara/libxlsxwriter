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

    lxw_workbook  *workbook  = workbook_new("test_ignore_error03.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    int row;
    for (row = 0; row < 10; row++)
        worksheet_write_string(worksheet, row, 0, "123" , NULL);

    worksheet_ignore_errors(worksheet, LXW_IGNORE_NUMBER_STORED_AS_TEXT, "A1:A10");

    return workbook_close(workbook);
}
