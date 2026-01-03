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

    lxw_workbook  *workbook  = workbook_new("test_chart_stock03.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_STOCK);
    lxw_chart_series *series;

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 53202304;
    chart->axis_id_2 = 66693376;

    lxw_format *date_format = workbook_add_format(workbook);
    format_set_num_format_index(date_format, 14);

    /* Dates as Excel serial numbers. */
    double dates[5] = {39083, 39084, 39085, 39086, 39087};
    /* Open values. */
    double open[5] = {23.49, 19.55, 15.12, 17.84, 16.34};
    /* High values. */
    double high[5] = {27.2, 25.03, 19.05, 20.34, 18.5};
    /* Low values. */
    double low[5] = {23.49, 19.55, 15.12, 17.84, 16.34};
    /* Close values. */
    double close[5] = {25.45, 23.05, 17.32, 20.45, 17.34};

    int row;
    for (row = 0; row < 5; row++) {
        worksheet_write_number(worksheet, row, 0, dates[row], date_format);
        worksheet_write_number(worksheet, row, 1, open[row], NULL);
        worksheet_write_number(worksheet, row, 2, high[row], NULL);
        worksheet_write_number(worksheet, row, 3, low[row], NULL);
        worksheet_write_number(worksheet, row, 4, close[row], NULL);
    }

    worksheet_set_column(worksheet, COLS("A:E"), 11, NULL);

    /* Line format hidden, marker none - like Rust test. */
    lxw_chart_line line = {.width = 2.25, .none = LXW_TRUE};

    series = chart_add_series(chart, "=Sheet1!$A$1:$A$5", "=Sheet1!$B$1:$B$5", 0);
    chart_series_set_line(series, &line);
    chart_series_set_marker_type(series, LXW_CHART_MARKER_NONE);

    series = chart_add_series(chart, "=Sheet1!$A$1:$A$5", "=Sheet1!$C$1:$C$5", 0);
    chart_series_set_line(series, &line);
    chart_series_set_marker_type(series, LXW_CHART_MARKER_NONE);

    series = chart_add_series(chart, "=Sheet1!$A$1:$A$5", "=Sheet1!$D$1:$D$5", 0);
    chart_series_set_line(series, &line);
    chart_series_set_marker_type(series, LXW_CHART_MARKER_NONE);

    series = chart_add_series(chart, "=Sheet1!$A$1:$A$5", "=Sheet1!$E$1:$E$5", 0);
    chart_series_set_line(series, &line);
    chart_series_set_marker_type(series, LXW_CHART_MARKER_NONE);

    chart_set_high_low_lines(chart, NULL);
    chart_set_up_down_bars(chart);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
