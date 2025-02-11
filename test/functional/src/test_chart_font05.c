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

    lxw_workbook  *workbook  = workbook_new("test_chart_font05.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_BAR);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 49407488;
    chart->axis_id_2 = 53740288;

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
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$5");

    chart_title_set_name(chart, "Title");

    lxw_chart_font font1 = {.name = "Arial", .pitch_family = 34, .charset = 0};
    lxw_chart_font font2 = {.bold = LXW_TRUE, .italic = LXW_TRUE, .underline = LXW_TRUE};

    chart_axis_set_name(chart->x_axis, "XXX");
    chart_axis_set_name(chart->y_axis, "YYY");

    chart_axis_set_num_font(chart->x_axis, &font1);
    chart_axis_set_num_font(chart->y_axis, &font2);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
