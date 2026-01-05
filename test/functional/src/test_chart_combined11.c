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

    lxw_workbook  *workbook       = workbook_new("test_chart_combined11.xlsx");
    lxw_worksheet *worksheet      = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart_doughnut = workbook_add_chart(workbook, LXW_CHART_DOUGHNUT);
    lxw_chart     *chart_pie      = workbook_add_chart(workbook, LXW_CHART_PIE);

    /* Write data for the doughnut chart. */
    worksheet_write_string(worksheet, 1, 7, "Donut", NULL);
    worksheet_write_number(worksheet, 2, 7, 25, NULL);
    worksheet_write_number(worksheet, 3, 7, 50, NULL);
    worksheet_write_number(worksheet, 4, 7, 25, NULL);
    worksheet_write_number(worksheet, 5, 7, 100, NULL);

    /* Write data for the pie chart. */
    worksheet_write_string(worksheet, 1, 8, "Pie", NULL);
    worksheet_write_number(worksheet, 2, 8, 75, NULL);
    worksheet_write_number(worksheet, 3, 8, 1, NULL);
    worksheet_write_number(worksheet, 4, 8, 124, NULL);

    /* Set up point formatting for doughnut chart. */
    lxw_chart_fill fill_red    = {.color = 0xFF0000};
    lxw_chart_fill fill_yellow = {.color = 0xFFC000};
    lxw_chart_fill fill_green  = {.color = 0x00B050};
    lxw_chart_fill fill_none   = {.none = LXW_TRUE};

    lxw_chart_point point1_1 = {.fill = &fill_red};
    lxw_chart_point point1_2 = {.fill = &fill_yellow};
    lxw_chart_point point1_3 = {.fill = &fill_green};
    lxw_chart_point point1_4 = {.fill = &fill_none};

    lxw_chart_point *points1[] = {&point1_1, &point1_2, &point1_3, &point1_4, NULL};

    lxw_chart_series *series1 = chart_add_series(chart_doughnut, NULL, "=Sheet1!$H$3:$H$6");
    chart_series_set_name(series1, "=Sheet1!$H$2");
    chart_series_set_points(series1, points1);

    chart_set_rotation(chart_doughnut, 270);
    chart_legend_set_position(chart_doughnut, LXW_CHART_LEGEND_NONE);

    /* Set chart area to have no border and no fill. */
    lxw_chart_line line_none = {.none = LXW_TRUE};
    chart_chartarea_set_line(chart_doughnut, &line_none);
    chart_chartarea_set_fill(chart_doughnut, &fill_none);

    /* Set up point formatting for pie chart. */
    lxw_chart_point point2_1 = {.fill = &fill_none};
    lxw_chart_point point2_2 = {.fill = &fill_red};
    lxw_chart_point point2_3 = {.fill = &fill_none};

    lxw_chart_point *points2[] = {&point2_1, &point2_2, &point2_3, NULL};

    lxw_chart_series *series2 = chart_add_series(chart_pie, NULL, "=Sheet1!$I$3:$I$6");
    chart_series_set_name(series2, "=Sheet1!$I$2");
    chart_series_set_points(series2, points2);

    chart_set_rotation(chart_pie, 270);

    chart_combine(chart_doughnut, chart_pie);

    worksheet_insert_chart(worksheet, CELL("A1"), chart_doughnut);

    return workbook_close(workbook);
}
