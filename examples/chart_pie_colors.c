/*
 * An example of creating an Excel pie chart with user defined colors using
 * the libxlsxwriter library.
 *
 * In general formatting is applied to an entire series in a chart. However,
 * it is occasionally required to format individual points in a series. In
 * particular this is required for Pie/Doughnut charts where each segment is
 * represented by a point.

 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"


/*
 * Create a worksheet with an example Pie chart.
 */
int main() {

    lxw_workbook     *workbook  = workbook_new("chart_pie_colors.xlsx");
    lxw_worksheet    *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart_series *series;


    /* Write some data for the chart. */
    worksheet_write_string(worksheet, CELL("A1"), "Pass", NULL);
    worksheet_write_string(worksheet, CELL("A2"), "Fail", NULL);
    worksheet_write_number(worksheet, CELL("B1"), 90,     NULL);
    worksheet_write_number(worksheet, CELL("B2"), 10,     NULL);

    /* Create a pie chart. */
    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_PIE);

    /* Add the data series to the chart. */
    series = chart_add_series(chart, "=Sheet1!$A$1:$A$2",
                                     "=Sheet1!$B$1:$B$2");

    /* Create some fills for the chart points/segments. */
    lxw_chart_fill red_fill     = {.color = LXW_COLOR_RED  };
    lxw_chart_fill green_fill   = {.color = LXW_COLOR_GREEN};

    /* Add the fills to the point objects. */
    lxw_chart_point red_point   = {.fill = &red_fill  };
    lxw_chart_point green_point = {.fill = &green_fill};

    /* Create an array of pointer to chart points. Note, the array should be
     * NULL terminated. */
    lxw_chart_point *points[] = {&green_point,
                                 &red_point,
                                 NULL};

    /* Add the points to the series. */
    chart_series_set_points(series, points);


    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("D2"), chart);

    return workbook_close(workbook);
}
