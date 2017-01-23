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

    lxw_workbook  *workbook  = new_workbook("test_chart_data_labels25.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 108652800;
    chart->axis_id_2 = 108656128;

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

    lxw_chart_series *series1 = chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");
    lxw_chart_series *series2 = chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$5");

    lxw_chart_font font1 = {.rotation =  45, .baseline = -1};
    lxw_chart_font font2 = {.rotation = -45, .baseline = -1};

    chart_series_set_labels(series1);
    chart_series_set_labels(series2);
    chart_series_set_labels_font(series1, &font1);
    chart_series_set_labels_font(series2, &font2);
    chart_series_set_labels_position(series2, LXW_CHART_LABEL_POSITION_OUTSIDE_END);
    chart_series_set_labels_position(series2, LXW_CHART_LABEL_POSITION_INSIDE_BASE);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
