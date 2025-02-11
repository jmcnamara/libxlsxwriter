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

    lxw_workbook  *workbook  = workbook_new("test_row_col_format02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format    *bold      = workbook_add_format(workbook);
    format_set_bold(bold);

    worksheet_set_row(worksheet, 0, 15, bold);

    worksheet_write_string(worksheet, 0, 0, "Foo", NULL);

    return workbook_close(workbook);
}
