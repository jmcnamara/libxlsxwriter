/*
 * An example of creating an Excel doughnut chart using the libxlsxwriter library.
 *
 * The demo also shows how to set segment colors. It is possible to define
 * chart colors for most types of libxlsxwriter charts via the series
 * formatting functions. However, Pie/Doughnut charts are a special case since
 * each segment is represented as a point so it is necessary to assign
 * formatting to each point in the series.
 *
 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

/*
 * Write some data to the worksheet.
 */
void write_worksheet_data(lxw_worksheet *worksheet, lxw_format *bold) {

    worksheet_write_string(worksheet, CELL("A1"), "Category",  bold);
    worksheet_write_string(worksheet, CELL("A2"), "Glazed",    NULL);
    worksheet_write_string(worksheet, CELL("A3"), "Chocolate", NULL);
    worksheet_write_string(worksheet, CELL("A4"), "Cream",     NULL);

    worksheet_write_string(worksheet, CELL("B1"), "Values",    bold);
    worksheet_write_number(worksheet, CELL("B2"), 50,          NULL);
    worksheet_write_number(worksheet, CELL("B3"), 35,          NULL);
    worksheet_write_number(worksheet, CELL("B4"), 15,          NULL);
}

/*
 * Create a worksheet with examples charts.
 *
 */
int main() {

    lxw_workbook     *workbook  = workbook_new("chart_doughnut.xlsx");
    lxw_worksheet    *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart *chart ;
    lxw_chart_series *series;

    /* Add a bold format to use to highlight the header cells. */
    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    /* Write some data for the chart. */
    write_worksheet_data(worksheet, bold);


    /*
     * Chart 1: Create a simple doughnut chart.
     */
    chart = workbook_add_chart(workbook, LXW_CHART_DOUGHNUT);

    /* Add the first series to the chart. */
    series = chart_add_series(chart, "=Sheet1!$A$2:$A$4", "=Sheet1!$B$2:$B$4");

    /* Set the name for the series instead of the default "Series 1". */
    chart_series_set_name(series, "Doughnut sales data");

    /* Add a chart title. */
    chart_title_set_name(chart, "Popular Doughnut Types");

    /* Set an Excel chart style. */
    chart_set_style(chart, 10);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("D2"), chart);


    /*
     * Chart 2: Create a doughnut chart with user defined segment colors.
     */
    chart = workbook_add_chart(workbook, LXW_CHART_DOUGHNUT);

    /* Add the first series to the chart. */
    series = chart_add_series(chart, "=Sheet1!$A$2:$A$4", "=Sheet1!$B$2:$B$4");

    /* Set the name for the series instead of the default "Series 1". */
    chart_series_set_name(series, "Doughnut sales data");

    /* Add a chart title. */
    chart_title_set_name(chart, "Doughnut Chart with user defined colors");

    /* Add for fills for use in the chart. */
    lxw_chart_fill fill1 = {.color = 0xFA58D0};
    lxw_chart_fill fill2 = {.color = 0x61210B};
    lxw_chart_fill fill3 = {.color = 0xF5F6CE};

    /* Add some points with the above fills. */
    lxw_chart_point point1 = {.fill = &fill1};
    lxw_chart_point point2 = {.fill = &fill2};
    lxw_chart_point point3 = {.fill = &fill3};

    /* Create an array of the point objects. */
    lxw_chart_point *points[] = {&point1,
                                 &point2,
                                 &point3,
                                 NULL};

    /* Add/override the points/segments of the chart. */
    chart_series_set_points(series, points);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("D18"), chart);


    /*
     * Chart 3: Create a Doughnut chart with rotation of the segments.
     */
    chart = workbook_add_chart(workbook, LXW_CHART_DOUGHNUT);

    /* Add the first series to the chart. */
    series = chart_add_series(chart, "=Sheet1!$A$2:$A$4", "=Sheet1!$B$2:$B$4");

    /* Set the name for the series instead of the default "Series 1". */
    chart_series_set_name(series, "Doughnut sales data");

    /* Add a chart title. */
    chart_title_set_name(chart, "Doughnut Chart with segment rotation");


    /* Change the angle/rotation of the first segment. */
    chart_set_rotation(chart, 90);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("D34"), chart);


    /*
     * Chart 4: Create a Doughnut chart with user defined hole size and other
     *          options.
     */
    chart = workbook_add_chart(workbook, LXW_CHART_DOUGHNUT);

    /* Add the first series to the chart. */
    series = chart_add_series(chart, "=Sheet1!$A$2:$A$4", "=Sheet1!$B$2:$B$4");

    /* Set the name for the series instead of the default "Series 1". */
    chart_series_set_name(series, "Doughnut sales data");

    /* Add a chart title. */
    chart_title_set_name(chart, "Doughnut Chart with options applied.");

    /* Add/override the points/segments defined in Chart 2. */
    chart_series_set_points(series, points);

    /* Set an Excel chart style. */
    chart_set_style(chart, 26);

    /* Change the angle/rotation of the first segment. */
    chart_set_rotation(chart, 28);

    /* Change the hole size. */
    chart_set_hole_size(chart, 33);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("D50"), chart);

    return workbook_close(workbook);
}
