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

    lxw_workbook  *workbook  = workbook_new("test_object_position13.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_row_col_options col_options = {.hidden = LXW_TRUE};
    worksheet_set_column_opt(worksheet, COLS("B:B"), LXW_DEF_COL_WIDTH, NULL, &col_options);

    worksheet_insert_image(worksheet, CELL("E9"), "images/red.png");

    return workbook_close(workbook);
}
