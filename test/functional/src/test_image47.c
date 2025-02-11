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

    lxw_workbook  *workbook  = workbook_new("test_image47.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_insert_image(worksheet, CELL("E9"), "images/red.png");

    lxw_row_col_options options = {.hidden = LXW_TRUE};
    worksheet_set_row_opt(worksheet, 9, LXW_DEF_ROW_HEIGHT, NULL, &options);

    return workbook_close(workbook);
}
