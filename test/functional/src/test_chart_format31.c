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

    lxw_workbook  *workbook  = workbook_new("test_chart_format31.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 115443200;
    chart->axis_id_2 = 115459200;

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

    lxw_chart_series *series = chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$B$1:$B$5"
    );

    chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$C$1:$C$5"
    );

    lxw_chart_line line = {.color     = LXW_COLOR_RED,
                           .width     = 1,
                           .dash_type = LXW_CHART_LINE_DASH_LONG_DASH};

    chart_series_set_trendline(series, LXW_CHART_TRENDLINE_TYPE_POLY, 2);
    chart_series_set_trendline_line(series, &line);
    chart_series_set_trendline_name(series, "My trend name");
    chart_series_set_trendline_forecast(series, 0.5, 0.5);
    chart_series_set_trendline_equation(series);
    chart_series_set_trendline_r_squared(series);
    chart_series_set_trendline_intercept(series, 1.5);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
