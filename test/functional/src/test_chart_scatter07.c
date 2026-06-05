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

    lxw_workbook  *workbook  = workbook_new("test_chart_scatter07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_SCATTER);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 63597952;
    chart->axis_id_2 = 63616128;
    chart->axis_id_3 = 63617664;
    chart->axis_id_4 = 63619456;

    uint8_t data[5][4] = {
        {27, 6, 20,  0},
        {33, 8, 10, 27},
        {44, 6, 30, 23},
        {12, 4, 50, 30},
        { 1, 2, 40, 40}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 4; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    chart_add_series(chart, "=Sheet1!$A$1:$A$5", "=Sheet1!$B$1:$B$5");
    lxw_chart_series *series2 = chart_add_series(chart, "=Sheet1!$C$1:$C$5", "=Sheet1!$D$1:$D$5");

    chart_series_set_secondary_axis(series2);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
