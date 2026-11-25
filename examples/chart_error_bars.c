/****************************************************************************
 *
 * Example of a chart with custom error bars.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("chart_error_bars.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart;
    lxw_chart_series *series;

    /* Data values and separate positive/negative error magnitudes. */
    worksheet_write_string(worksheet, CELL("A1"), "Sample", NULL);
    worksheet_write_string(worksheet, CELL("B1"), "Value", NULL);
    worksheet_write_string(worksheet, CELL("C1"), "Positive error", NULL);
    worksheet_write_string(worksheet, CELL("D1"), "Negative error", NULL);

    worksheet_write_number(worksheet, CELL("A2"), 1, NULL);
    worksheet_write_number(worksheet, CELL("A3"), 2, NULL);
    worksheet_write_number(worksheet, CELL("A4"), 3, NULL);
    worksheet_write_number(worksheet, CELL("A5"), 4, NULL);
    worksheet_write_number(worksheet, CELL("A6"), 5, NULL);

    worksheet_write_number(worksheet, CELL("B2"), 12, NULL);
    worksheet_write_number(worksheet, CELL("B3"), 18, NULL);
    worksheet_write_number(worksheet, CELL("B4"), 15, NULL);
    worksheet_write_number(worksheet, CELL("B5"), 23, NULL);
    worksheet_write_number(worksheet, CELL("B6"), 20, NULL);

    worksheet_write_number(worksheet, CELL("C2"), 2, NULL);
    worksheet_write_number(worksheet, CELL("C3"), 3, NULL);
    worksheet_write_number(worksheet, CELL("C4"), 1, NULL);
    worksheet_write_number(worksheet, CELL("C5"), 4, NULL);
    worksheet_write_number(worksheet, CELL("C6"), 2, NULL);

    worksheet_write_number(worksheet, CELL("D2"), 1, NULL);
    worksheet_write_number(worksheet, CELL("D3"), 2, NULL);
    worksheet_write_number(worksheet, CELL("D4"), 3, NULL);
    worksheet_write_number(worksheet, CELL("D5"), 2, NULL);
    worksheet_write_number(worksheet, CELL("D6"), 1, NULL);

    chart = workbook_add_chart(workbook, LXW_CHART_SCATTER);
    chart_title_set_name(chart, "Scatter Chart with Custom Error Bars");
    chart_axis_set_name(chart->x_axis, "Sample");
    chart_axis_set_name(chart->y_axis, "Value");

    series = chart_add_series(chart, "=Sheet1!$A$2:$A$6", "=Sheet1!$B$2:$B$6");
    chart_series_set_error_bars_custom(series->y_error_bars,
                                       "=Sheet1!$C$2:$C$6",
                                       "=Sheet1!$D$2:$D$6");

    worksheet_insert_chart(worksheet, CELL("F2"), chart);

    return workbook_close(workbook);
}
