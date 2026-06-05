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

    lxw_workbook  *workbook  = workbook_new("test_chart_combined09.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart1    = workbook_add_chart(workbook, LXW_CHART_COLUMN);
    lxw_chart     *chart2    = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart1->axis_id_1 = 114984064;
    chart1->axis_id_2 = 114985600;
    chart2->axis_id_3 = 114988928;
    chart2->axis_id_4 = 114987392;

    uint8_t data[5][2] = {
        {2, 20},
        {7, 25},
        {3, 10},
        {6, 10},
        {2, 20}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 2; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    chart_add_series(chart1, NULL, "=Sheet1!$A$1:$A$5");

    lxw_chart_series *series2 = chart_add_series(chart2, NULL, "=Sheet1!$B$1:$B$5");
    chart_series_set_secondary_axis(series2);

    lxw_chart_font font1 = {.bold = LXW_TRUE, .baseline = -1};
    lxw_chart_font font2 = {.bold = LXW_TRUE, .baseline = -1};

    chart_axis_set_num_font(chart1->y_axis, &font1);
    chart_axis_set_num_font(chart2->y2_axis, &font2);

    chart_combine(chart1, chart2);

    worksheet_insert_chart(worksheet, CELL("E9"), chart1);

    return workbook_close(workbook);
}
