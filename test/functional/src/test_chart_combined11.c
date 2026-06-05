/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook      = workbook_new("test_chart_combined11.xlsx");
    lxw_worksheet *worksheet     = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart_doughnut = workbook_add_chart(workbook, LXW_CHART_DOUGHNUT);
    lxw_chart     *chart_pie     = workbook_add_chart(workbook, LXW_CHART_PIE);

    worksheet_write_string(worksheet, 1, 7, "Donut", NULL);
    worksheet_write_number(worksheet, 2, 7, 25, NULL);
    worksheet_write_number(worksheet, 3, 7, 50, NULL);
    worksheet_write_number(worksheet, 4, 7, 25, NULL);
    worksheet_write_number(worksheet, 5, 7, 100, NULL);

    worksheet_write_string(worksheet, 1, 8, "Pie", NULL);
    worksheet_write_number(worksheet, 2, 8, 75, NULL);
    worksheet_write_number(worksheet, 3, 8, 1, NULL);
    worksheet_write_number(worksheet, 4, 8, 124, NULL);

    lxw_chart_series *series1 = chart_add_series(chart_doughnut, NULL, "=Sheet1!$H$3:$H$6");
    chart_series_set_name(series1, "=Sheet1!$H$2");

    lxw_chart_fill fill1 = {.color = 0xFF0000};
    lxw_chart_fill fill2 = {.color = 0xFFC000};
    lxw_chart_fill fill3 = {.color = 0x00B050};
    lxw_chart_fill fill4 = {.none = LXW_TRUE};

    lxw_chart_point point1 = {.fill = &fill1};
    lxw_chart_point point2 = {.fill = &fill2};
    lxw_chart_point point3 = {.fill = &fill3};
    lxw_chart_point point4 = {.fill = &fill4};
    lxw_chart_point *doughnut_points[] = {&point1, &point2, &point3, &point4, NULL};

    chart_series_set_points(series1, doughnut_points);

    chart_set_rotation(chart_doughnut, 270);
    chart_legend_set_position(chart_doughnut, LXW_CHART_LEGEND_NONE);

    lxw_chart_fill chartarea_fill = {.none = LXW_TRUE};
    lxw_chart_line chartarea_line = {.none = LXW_TRUE};
    chart_chartarea_set_fill(chart_doughnut, &chartarea_fill);
    chart_chartarea_set_line(chart_doughnut, &chartarea_line);

    lxw_chart_series *series2 = chart_add_series(chart_pie, NULL, "=Sheet1!$I$3:$I$6");
    chart_series_set_name(series2, "=Sheet1!$I$2");

    lxw_chart_fill pie_fill1 = {.none = LXW_TRUE};
    lxw_chart_fill pie_fill2 = {.color = 0xFF0000};
    lxw_chart_fill pie_fill3 = {.none = LXW_TRUE};

    lxw_chart_point pie_point1 = {.fill = &pie_fill1};
    lxw_chart_point pie_point2 = {.fill = &pie_fill2};
    lxw_chart_point pie_point3 = {.fill = &pie_fill3};
    lxw_chart_point *pie_points[] = {&pie_point1, &pie_point2, &pie_point3, NULL};

    chart_series_set_points(series2, pie_points);

    chart_set_rotation(chart_pie, 270);

    chart_combine(chart_doughnut, chart_pie);

    worksheet_insert_chart(worksheet, CELL("A1"), chart_doughnut);

    return workbook_close(workbook);
}
