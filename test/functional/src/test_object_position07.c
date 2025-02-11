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

    lxw_workbook  *workbook  = workbook_new("test_object_position07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_image_options options = {.object_position = LXW_OBJECT_MOVE_AND_SIZE_AFTER};
    worksheet_insert_image_opt(worksheet, CELL("E9"), "images/red.png", &options);

    lxw_row_col_options hidden = {.hidden = LXW_TRUE};
    worksheet_set_row_opt(worksheet, 8, 30, NULL, &hidden);

    return workbook_close(workbook);
}
