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

    lxw_workbook  *workbook  = workbook_new("test_ignore_error04.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, CELL("A1"), "123" , NULL);
    worksheet_write_string(worksheet, CELL("C3"), "123" , NULL);
    worksheet_write_string(worksheet, CELL("E5"), "123" , NULL);

    worksheet_ignore_errors(worksheet, LXW_IGNORE_NUMBER_STORED_AS_TEXT, "A1 C3 E5");

    return workbook_close(workbook);
}
