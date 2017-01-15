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

    lxw_workbook  *workbook  = new_workbook("test_chart_points05.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 45471616;
    chart->axis_id_2 = 46804992;

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
    lxw_chart_point red_point  = {.fill = &red_fill};

    lxw_chart_point *points[] = {&red_point, NULL};

    chart_series_set_points(series1, points);

    chart_series_set_marker_type(series1, LXW_CHART_MARKER_AUTOMATIC);
    chart_series_set_marker_type(series2, LXW_CHART_MARKER_AUTOMATIC);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
