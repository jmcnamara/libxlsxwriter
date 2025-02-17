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

    lxw_workbook  *workbook  = workbook_new("test_row_col_format12.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_row_col_options options = {1, 0, 0};

    worksheet_set_column_opt(worksheet, 2, 2, LXW_DEF_COL_WIDTH, NULL, &options);

    return workbook_close(workbook);
}
