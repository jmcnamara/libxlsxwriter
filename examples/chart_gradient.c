/*
 * An example of creating an Excel column chart with gradient fills using
 * libxlsxwriter.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("chart_gradient.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart_series *series;

    /* Add a bold format for the headers. */
    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    /* Write the worksheet data that the chart will refer to. */
    worksheet_write_string(worksheet, 0, 0, "Number",  bold);
    worksheet_write_string(worksheet, 0, 1, "Batch 1", bold);
    worksheet_write_string(worksheet, 0, 2, "Batch 2", bold);

    worksheet_write_number(worksheet, 1, 0, 2, NULL);
    worksheet_write_number(worksheet, 2, 0, 3, NULL);
    worksheet_write_number(worksheet, 3, 0, 4, NULL);
    worksheet_write_number(worksheet, 4, 0, 5, NULL);
    worksheet_write_number(worksheet, 5, 0, 6, NULL);
    worksheet_write_number(worksheet, 6, 0, 7, NULL);

    worksheet_write_number(worksheet, 1, 1, 10, NULL);
    worksheet_write_number(worksheet, 2, 1, 40, NULL);
    worksheet_write_number(worksheet, 3, 1, 50, NULL);
    worksheet_write_number(worksheet, 4, 1, 20, NULL);
    worksheet_write_number(worksheet, 5, 1, 10, NULL);
    worksheet_write_number(worksheet, 6, 1, 50, NULL);

    worksheet_write_number(worksheet, 1, 2, 30, NULL);
    worksheet_write_number(worksheet, 2, 2, 60, NULL);
    worksheet_write_number(worksheet, 3, 2, 70, NULL);
    worksheet_write_number(worksheet, 4, 2, 50, NULL);
    worksheet_write_number(worksheet, 5, 2, 40, NULL);
    worksheet_write_number(worksheet, 6, 2, 30, NULL);

    /* Create a column chart. */
    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Configure the first series with a gradient fill. */
    series = chart_add_series(chart,
                              "=Sheet1!$A$2:$A$7",
                              "=Sheet1!$B$2:$B$7", 0);
    chart_series_set_name(series, "=Sheet1!$B$1");

    /* Set gradient: dark red to light red. */
    lxw_chart_gradient_fill gradient1 = {
        .type = LXW_CHART_GRADIENT_FILL_LINEAR,
        .colors = {0x963735, 0xF1DCDB},
        .num_stops = 2,
    };
    chart_series_set_gradient(series, &gradient1);

    /* Configure the second series with a gradient fill. */
    series = chart_add_series(chart,
                              "=Sheet1!$A$2:$A$7",
                              "=Sheet1!$C$2:$C$7", 0);
    chart_series_set_name(series, "=Sheet1!$C$1");

    /* Set gradient: dark orange to light orange. */
    lxw_chart_gradient_fill gradient2 = {
        .type = LXW_CHART_GRADIENT_FILL_LINEAR,
        .colors = {0xE36C0A, 0xFCEADA},
        .num_stops = 2,
    };
    chart_series_set_gradient(series, &gradient2);

    /* Set a gradient fill for the plotarea. */
    lxw_chart_gradient_fill plotarea_gradient = {
        .type = LXW_CHART_GRADIENT_FILL_LINEAR,
        .colors = {0xFFEFD1, 0xF0EBD5, 0xB69F66},
        .num_stops = 3,
    };
    chart_plotarea_set_gradient(chart, &plotarea_gradient);

    /* Add axis labels. */
    chart_axis_set_name(chart->x_axis, "Test number");
    chart_axis_set_name(chart->y_axis, "Sample length (mm)");

    /* Turn off the chart legend. */
    chart_legend_set_position(chart, LXW_CHART_LEGEND_NONE);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("E2"), chart);

    return workbook_close(workbook);
}
