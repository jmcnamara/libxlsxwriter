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

    lxw_workbook  *workbook  = workbook_new("test_row_col_format11.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, 2, 2, 4, NULL);

    return workbook_close(workbook);
}
