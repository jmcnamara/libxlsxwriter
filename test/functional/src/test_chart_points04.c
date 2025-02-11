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

    lxw_workbook  *workbook  = workbook_new("test_chart_points04.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_SCATTER);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 48542464;
    chart->axis_id_2 = 46807296;

    uint8_t data[5][3] = {
        {1, 2,  3},
        {2, 4,  6},
        {3, 6,  9},
        {4, 8,  12},
        {5, 10, 15}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 3; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    lxw_chart_series *series1 = chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$B$1:$B$5"
    );

    lxw_chart_series *series2 = chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$C$1:$C$5");

    lxw_chart_fill red_fill    = {.color = LXW_COLOR_RED};
    lxw_chart_fill yellow_fill = {.color = LXW_COLOR_YELLOW};

    lxw_chart_point default_point = {0, 0, 0};
    lxw_chart_point red_point     = {.fill = &red_fill};
    lxw_chart_point yellow_point  = {.fill = &yellow_fill};

    lxw_chart_point *points1[] = {&red_point,
                                  &yellow_point,
                                  NULL};

    lxw_chart_point *points2[] = {&default_point,
                                  &default_point,
                                  &yellow_point,
                                  NULL};

    chart_series_set_points(series1, points1);
    chart_series_set_points(series2, points2);


    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
