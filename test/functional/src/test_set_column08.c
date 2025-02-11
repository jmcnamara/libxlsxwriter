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

    lxw_workbook  *workbook  = workbook_new("test_set_column08.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_row_col_options options = {.hidden = LXW_TRUE};

    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    lxw_format *italic = workbook_add_format(workbook);
    format_set_italic(italic);

    uint8_t data[5][3] = {
        {1, 2,  3},
        {2, 4,  6},
        {3, 6,  9},
        {4, 8,  12},
        {5, 10, 15}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 3; col++)
            worksheet_write_number(worksheet, row + 1, col, data[row][col], NULL);

    worksheet_write_string(worksheet, CELL("A1"), "Foo", bold);
    worksheet_write_string(worksheet, CELL("B1"), "Bar", italic);

    worksheet_set_row_opt(worksheet, 12, LXW_DEF_ROW_HEIGHT, NULL, &options);
    worksheet_set_column_opt(worksheet, COLS("F:F"), LXW_DEF_COL_WIDTH, NULL, &options);

    worksheet_insert_image(worksheet, CELL("E12"), "images/logo.png");

    return workbook_close(workbook);
}
