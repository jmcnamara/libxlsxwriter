/*
 * An example of creating an Excel line chart with a secondary axis using
 * the libxlsxwriter library.
 *
 * A secondary Y axis allows you to display two series with different scales
 * on the same chart. The secondary Y axis appears on the right side of the
 * chart.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"


int main() {

    lxw_workbook     *workbook  = workbook_new("chart_secondary_axis.xlsx");
    lxw_worksheet    *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart_series *series;

    /* Add a bold format for the headings. */
    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    /* Write the worksheet data that the chart will refer to. */
    worksheet_write_string(worksheet, CELL("A1"), "Aliens", bold);
    worksheet_write_string(worksheet, CELL("B1"), "Humans", bold);

    /* Data for the "Aliens" series - small scale (2-7). */
    worksheet_write_number(worksheet, 1, 0, 2, NULL);
    worksheet_write_number(worksheet, 2, 0, 3, NULL);
    worksheet_write_number(worksheet, 3, 0, 4, NULL);
    worksheet_write_number(worksheet, 4, 0, 5, NULL);
    worksheet_write_number(worksheet, 5, 0, 6, NULL);
    worksheet_write_number(worksheet, 6, 0, 7, NULL);

    /* Data for the "Humans" series - larger scale (10-50). */
    worksheet_write_number(worksheet, 1, 1, 10, NULL);
    worksheet_write_number(worksheet, 2, 1, 40, NULL);
    worksheet_write_number(worksheet, 3, 1, 50, NULL);
    worksheet_write_number(worksheet, 4, 1, 20, NULL);
    worksheet_write_number(worksheet, 5, 1, 10, NULL);
    worksheet_write_number(worksheet, 6, 1, 50, NULL);

    /* Create a line chart. */
    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* Add the first series to the SECONDARY Y axis (y2_axis = 1).
     *
     * When y2_axis is set to 1 (LXW_TRUE), the series will be plotted
     * against the secondary Y axis on the right side of the chart.
     * This is useful when you have data with different scales.
     */
    series = chart_add_series(chart,
        NULL,
        "=Sheet1!$A$2:$A$7",
        1);  /* y2_axis = 1: Use secondary Y axis */
    chart_series_set_name(series, "=Sheet1!$A$1");

    /* Add the second series to the PRIMARY Y axis (y2_axis = 0). */
    series = chart_add_series(chart,
        NULL,
        "=Sheet1!$B$2:$B$7",
        0);  /* y2_axis = 0: Use primary Y axis */
    chart_series_set_name(series, "=Sheet1!$B$1");

    /* Configure the chart legend position. */
    chart_legend_set_position(chart, LXW_CHART_LEGEND_RIGHT);

    /* Add a chart title and axis labels. */
    chart_title_set_name(chart, "Survey results");
    chart_axis_set_name(chart->x_axis, "Days");
    chart_axis_set_name(chart->y_axis, "Population");
    chart_axis_set_name(chart->y2_axis, "Laser wounds");

    /* Hide the major gridlines on the primary Y axis for clarity. */
    chart_axis_major_gridlines_set_visible(chart->y_axis, LXW_FALSE);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("D2"), chart);

    return workbook_close(workbook);
}
