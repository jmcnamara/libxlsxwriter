/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_chart_bar19.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_BAR);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 66558592;
    chart->axis_id_2 = 66569344;

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

    lxw_chart_series *series1 = chart_add_series(chart, NULL, NULL);
    lxw_chart_series *series2 = chart_add_series(chart, NULL, NULL);
    lxw_chart_series *series3 = chart_add_series(chart, NULL, NULL);

    chart_series_set_values(series1, "Sheet1", 0, 0, 4, 0);
    chart_series_set_values(series2, "Sheet1", 0, 1, 4, 1);
    chart_series_set_values(series3, "Sheet1", 0, 2, 4, 2);

    chart_axis_set_name(chart->x_axis, "=Sheet1!$A$2");
    chart_axis_set_name(chart->y_axis, "=Sheet1!$A$3");
    chart_title_set_name(chart, "=Sheet1!$A$1");

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
