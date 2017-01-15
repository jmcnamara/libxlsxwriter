/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_chart_points02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_PIE);

    worksheet_write_number(worksheet, 0, 0, 2, NULL);
    worksheet_write_number(worksheet, 1, 0, 5, NULL);
    worksheet_write_number(worksheet, 2, 0, 4, NULL);
    worksheet_write_number(worksheet, 3, 0, 1, NULL);
    worksheet_write_number(worksheet, 4, 0, 7, NULL);
    worksheet_write_number(worksheet, 5, 0, 4, NULL);

    lxw_chart_series *series = chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$6");

    lxw_chart_line line1 = {.color     = LXW_COLOR_RED,
                            .dash_type = LXW_CHART_LINE_DASH_SQUARE_DOT};

    lxw_chart_fill fill1 = {.color = LXW_COLOR_YELLOW};

    lxw_chart_point default_point = {0, 0, 0};
    lxw_chart_point point2 = {.line = &line1};
    lxw_chart_point point4 = {.fill = &fill1};

    lxw_chart_point *points[] = {&default_point,
                                 &point2,
                                 &default_point,
                                 &point4,
                                 NULL};

    chart_series_set_points(series, points);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
