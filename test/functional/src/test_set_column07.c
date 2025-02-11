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

    lxw_workbook  *workbook  = workbook_new("test_set_column07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    lxw_format *italic = workbook_add_format(workbook);
    format_set_italic(italic);

    lxw_format *bold_italic = workbook_add_format(workbook);
    format_set_bold(bold_italic);
    format_set_italic(bold_italic);

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

    worksheet_write_string(worksheet, CELL("A1"), "Foo", italic);
    worksheet_write_string(worksheet, CELL("B1"), "Bar", bold);

    worksheet_set_row(worksheet, 12, LXW_DEF_ROW_HEIGHT, italic);
    worksheet_set_column(worksheet, COLS("F:F"), LXW_DEF_COL_WIDTH, bold);

    worksheet_write_blank(worksheet, CELL("F13"), bold_italic);

    worksheet_insert_image(worksheet, CELL("E12"), "images/logo.png");

    return workbook_close(workbook);
}
