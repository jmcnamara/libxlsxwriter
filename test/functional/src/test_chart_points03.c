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

    lxw_workbook  *workbook  = workbook_new("test_chart_points03.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_PIE);


    worksheet_write_number(worksheet, 0, 0, 2, NULL);
    worksheet_write_number(worksheet, 1, 0, 5, NULL);
    worksheet_write_number(worksheet, 2, 0, 4, NULL);

    lxw_chart_series *series = chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$3");


    lxw_chart_fill fill1 = {.color = 0xFF0000};
    lxw_chart_fill fill2 = {.color = 0xCC0000};
    lxw_chart_fill fill3 = {.color = 0x990000};

    lxw_chart_point point1 = {.fill = &fill1};
    lxw_chart_point point2 = {.fill = &fill2};
    lxw_chart_point point3 = {.fill = &fill3};

    lxw_chart_point *points[] = {&point1, &point2, &point3, NULL};

    chart_series_set_points(series, points);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
