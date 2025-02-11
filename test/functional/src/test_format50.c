/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case to test data writing.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"


int main() {

    lxw_workbook  *workbook  = workbook_new("test_format50.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format    *format1   = workbook_add_format(workbook);
    lxw_format    *format2   = workbook_add_format(workbook);

    format_set_num_format(format1, "#,##0.00000");
    format_set_num_format(format2, "#,##0.0");

    worksheet_set_column(worksheet, 0, 0, 12, NULL);

    worksheet_write_number(worksheet, 0, 0, 1234.5, format1);
    worksheet_write_number(worksheet, 1, 0, 1234.5, format2);


    return workbook_close(workbook);
}
