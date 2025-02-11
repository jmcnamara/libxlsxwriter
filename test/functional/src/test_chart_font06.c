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

    lxw_workbook  *workbook  = workbook_new("test_chart_font06.xlsx");
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


    lxw_chart_font font1 = {.name = "Calibri",     .pitch_family = 34, .charset = 0, .color = LXW_COLOR_YELLOW};
    lxw_chart_font font2 = {.name = "Courier New", .pitch_family = 49, .charset = 0, .color = 0x92D050};
    lxw_chart_font font3 = {.name = "Arial",       .pitch_family = 34, .charset = 0, .color = 0x00B0F0};
    lxw_chart_font font4 = {.name = "Century",     .pitch_family = 18, .charset = 0, .color = LXW_COLOR_RED};
    lxw_chart_font font5 = {.bold = LXW_TRUE, .italic = LXW_TRUE, .underline = LXW_TRUE, .color = 0x7030A0};


    chart_title_set_name(chart, "Title");
    chart_title_set_name_font(chart, &font1);

    chart_axis_set_name(chart->x_axis, "XXX");
    chart_axis_set_name_font(chart->x_axis, &font2);
    chart_axis_set_num_font(chart->x_axis, &font3);

    chart_axis_set_name(chart->y_axis, "YYY");
    chart_axis_set_name_font(chart->y_axis, &font4);
    chart_axis_set_num_font(chart->y_axis, &font5);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
