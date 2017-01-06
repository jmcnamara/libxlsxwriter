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

    lxw_workbook  *workbook  = new_workbook("test_chart_gridlines09.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 48744320;
    chart->axis_id_2 = 49566848;

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

    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$5");


    lxw_chart_line line1 = {.color = LXW_COLOR_RED,
                            .width = 0.5,
                            .dash_type = LXW_CHART_LINE_DASH_SQUARE_DOT};

    lxw_chart_line line2 = {.color = LXW_COLOR_YELLOW};

    lxw_chart_line line3 = {.width = 1.25,
                            .dash_type = LXW_CHART_LINE_DASH_DASH};

    lxw_chart_line line4 = {.color =  0x00B050};

    chart_axis_major_gridlines_set_line(chart->x_axis, &line1);
    chart_axis_minor_gridlines_set_line(chart->x_axis, &line2);
    chart_axis_major_gridlines_set_line(chart->y_axis, &line3);
    chart_axis_minor_gridlines_set_line(chart->y_axis, &line4);


    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
