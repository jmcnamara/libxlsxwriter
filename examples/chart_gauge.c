/*
 * An example of creating a Gauge Chart in Excel with libxlsxwriter.
 *
 * A Gauge Chart isn't a native chart type in Excel. It is constructed by
 * combining a doughnut chart and a pie chart and by using some non-filled
 * elements. This example follows the following online example of how to create
 * a Gauge Chart in Excel: https://www.excel-easy.com/examples/gauge-chart.html
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("chart_gauge.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart_series *series;

    /* Add some data for the Doughnut and Pie charts. This is set up so the
     * gauge goes from 0-100. It is initially set at 75%. */
    worksheet_write_string(worksheet, 1, 7, "Donut", NULL);
    worksheet_write_number(worksheet, 2, 7, 25, NULL);
    worksheet_write_number(worksheet, 3, 7, 50, NULL);
    worksheet_write_number(worksheet, 4, 7, 25, NULL);
    worksheet_write_number(worksheet, 5, 7, 100, NULL);

    worksheet_write_string(worksheet, 1, 8, "Pie", NULL);
    worksheet_write_number(worksheet, 2, 8, 75, NULL);
    worksheet_write_number(worksheet, 3, 8, 1, NULL);
    worksheet_write_formula(worksheet, 4, 8, "=200-I4-I3", NULL);

    /* Create a doughnut chart for the background of the gauge. */
    lxw_chart *chart_doughnut = workbook_add_chart(workbook, LXW_CHART_DOUGHNUT);

    /* Add the doughnut chart series with colors for the segments. */
    series = chart_add_series(chart_doughnut,
                              NULL, "=Sheet1!$H$3:$H$6", 0);
    chart_series_set_name(series, "=Sheet1!$H$2");

    /* Create fills for the doughnut segments: green, yellow, red, and no fill. */
    lxw_chart_fill green_fill  = {.color = 0x00B050};
    lxw_chart_fill yellow_fill = {.color = 0xFFFF00};
    lxw_chart_fill red_fill    = {.color = 0xFF0000};
    lxw_chart_fill no_fill     = {.none = LXW_TRUE};

    lxw_chart_point green_point  = {.fill = &green_fill};
    lxw_chart_point yellow_point = {.fill = &yellow_fill};
    lxw_chart_point red_point    = {.fill = &red_fill};
    lxw_chart_point empty_point  = {.fill = &no_fill};

    lxw_chart_point *doughnut_points[] = {&green_point,
                                          &yellow_point,
                                          &red_point,
                                          &empty_point,
                                          NULL};

    chart_series_set_points(series, doughnut_points);

    /* Rotate the doughnut chart so the gauge segments are above the horizontal. */
    chart_set_rotation(chart_doughnut, 270);

    /* Turn off the chart legend. */
    chart_legend_set_position(chart_doughnut, LXW_CHART_LEGEND_NONE);

    /* Turn off the chart fill and border. */
    lxw_chart_fill chart_no_fill = {.none = LXW_TRUE};
    lxw_chart_line chart_no_line = {.none = LXW_TRUE};
    chart_chartarea_set_fill(chart_doughnut, &chart_no_fill);
    chart_chartarea_set_line(chart_doughnut, &chart_no_line);

    /* Create a pie chart for the needle of the gauge. */
    lxw_chart *chart_pie = workbook_add_chart(workbook, LXW_CHART_PIE);

    /* Add the pie chart series for the needle. */
    series = chart_add_series(chart_pie,
                              NULL, "=Sheet1!$I$3:$I$6", 0);
    chart_series_set_name(series, "=Sheet1!$I$2");

    /* Create fills for the pie segments: no fill, black (needle), no fill. */
    lxw_chart_fill black_fill = {.color = 0x000000};

    lxw_chart_point pie_empty1 = {.fill = &no_fill};
    lxw_chart_point pie_needle = {.fill = &black_fill};
    lxw_chart_point pie_empty2 = {.fill = &no_fill};

    lxw_chart_point *pie_points[] = {&pie_empty1,
                                     &pie_needle,
                                     &pie_empty2,
                                     NULL};

    chart_series_set_points(series, pie_points);

    /* Rotate the pie chart to align the needle with the doughnut gauge. */
    chart_set_rotation(chart_pie, 270);

    /* Combine the doughnut and pie charts. */
    chart_combine(chart_doughnut, chart_pie);

    /* Insert the combined chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("A1"), chart_doughnut);

    return workbook_close(workbook);
}
