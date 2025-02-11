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

    lxw_workbook  *workbook  = workbook_new("test_object_position10.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_image_options options = {.object_position = LXW_OBJECT_MOVE_AND_SIZE_AFTER};
    worksheet_insert_image_opt(worksheet, CELL("E9"), "images/red.png", &options);

    lxw_row_col_options col_hidden = {.hidden = LXW_TRUE};
    worksheet_set_column_opt(worksheet, COLS("E:E"), LXW_DEF_COL_WIDTH, NULL, &col_hidden);

    return workbook_close(workbook);
}
