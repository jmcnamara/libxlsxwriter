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

    lxw_workbook  *workbook  = workbook_new("test_chart_errorbars04.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_SCATTER);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 61626624;
    chart->axis_id_2 = 69664128;

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

    lxw_chart_series *series1 = chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$B$1:$B$5"
    );

    chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$C$1:$C$5"
    );

    lxw_series_error_bars *x_error_bars;
    lxw_series_error_bars *y_error_bars;

    x_error_bars = chart_series_get_error_bars(series1, LXW_CHART_ERROR_BAR_AXIS_X);
    y_error_bars = chart_series_get_error_bars(series1, LXW_CHART_ERROR_BAR_AXIS_Y);

    chart_series_set_error_bars(x_error_bars,
                                LXW_CHART_ERROR_BAR_TYPE_STD_DEV, 1);

    chart_series_set_error_bars(y_error_bars,
                                LXW_CHART_ERROR_BAR_TYPE_STD_ERROR, 0);


    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
