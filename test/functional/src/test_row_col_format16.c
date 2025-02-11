/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case to test worksheet set_row() and set_column().
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_row_col_format16.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format    *italic      = workbook_add_format(workbook);
    format_set_italic(italic);

    worksheet_set_column(worksheet, 16383, 16383, 8.43, italic);

    worksheet_write_string(worksheet, 0, 16383, "Foo", NULL);

    return workbook_close(workbook);
}
