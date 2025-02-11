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

    lxw_workbook  *workbook  = workbook_new("test_chart_format20.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart1 = workbook_add_chart(workbook, LXW_CHART_LINE);
    lxw_chart     *chart2 = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart1->axis_id_1 = 80553856;
    chart1->axis_id_2 = 80555392;

    chart2->axis_id_1 = 84583936;
    chart2->axis_id_2 = 84585856;

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

    lxw_chart_series *series1 = chart_add_series(chart1, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart1, NULL, "=Sheet1!$C$1:$C$5");

    lxw_chart_series *series2 = chart_add_series(chart2, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart2, NULL, "=Sheet1!$C$1:$C$5");


    lxw_chart_line line = {.color     = LXW_COLOR_RED,
                           .dash_type = LXW_CHART_LINE_DASH_DASH};

    chart_series_set_trendline(series1, LXW_CHART_TRENDLINE_TYPE_LINEAR, 0);
    chart_series_set_trendline_line(series1, &line);
    chart_series_set_trendline(series2, LXW_CHART_TRENDLINE_TYPE_LINEAR, 0);
    chart_series_set_trendline_line(series2, &line);


    worksheet_insert_chart(worksheet, CELL("E9"), chart1);
    worksheet_insert_chart(worksheet, CELL("E25"), chart2);

    return workbook_close(workbook);
}
