/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_chart_layout05.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_AREA);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 43495808;
    chart->axis_id_2 = 43497728;

    uint8_t data[5][3] = {
        {1, 8,  3},
        {2, 7,  6},
        {3, 6,  9},
        {4, 8,  12},
        {5, 10, 15}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 3; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$B$1:$B$5"
    );

    chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$C$1:$C$5"
    );



    lxw_chart_layout layout_x = {
        .x = 0.346203193350831,
        .y = 0.850902595508894,
    };

    chart_axis_set_name(chart->x_axis, "XXX");
    chart_axis_set_name_layout(chart->x_axis, &layout_x);



    lxw_chart_layout layout_y = {
        .x = 0.213888888888888,
        .y = 0.263499198016914,
    };

    chart_axis_set_name(chart->y_axis, "YYY");
    chart_axis_set_name_layout(chart->y_axis, &layout_y);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
