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

    lxw_workbook  *workbook  = workbook_new("test_set_column11.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_LINE);

    lxw_row_col_options options = {.hidden = LXW_TRUE};

    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    lxw_format *italic = workbook_add_format(workbook);
    format_set_italic(italic);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 69197824;
    chart->axis_id_2 = 69199360;

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

    worksheet_set_row_pixels_opt(worksheet, 12, LXW_DEF_ROW_HEIGHT_PIXELS, NULL, &options);
    worksheet_set_column_pixels_opt(worksheet, COLS("F:F"), LXW_DEF_COL_WIDTH_PIXELS, NULL, &options);

    chart_add_series(chart, NULL, "=Sheet1!$A$2:$A$6");
    chart_add_series(chart, NULL, "=Sheet1!$B$2:$B$6");
    chart_add_series(chart, NULL, "=Sheet1!$C$2:$C$6");

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
