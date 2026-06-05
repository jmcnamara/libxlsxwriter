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

    lxw_workbook  *workbook  = workbook_new("test_chart_combined06.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart1    = workbook_add_chart(workbook, LXW_CHART_AREA);
    lxw_chart     *chart2    = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart1->axis_id_1 = 91755648;
    chart1->axis_id_2 = 91757952;
    chart2->axis_id_1 = 91755648;
    chart2->axis_id_2 = 91757952;

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
    chart_add_series(chart2, NULL, "=Sheet1!$B$1:$B$5");

    chart_combine(chart1, chart2);

    /* Set cross_between to 'between' for the area chart. */
    chart1->x_axis->position_axis = LXW_CHART_AXIS_POSITION_BETWEEN;

    worksheet_insert_chart(worksheet, CELL("E9"), chart1);

    return workbook_close(workbook);
}
