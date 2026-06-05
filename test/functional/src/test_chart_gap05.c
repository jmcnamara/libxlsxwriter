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

    lxw_workbook  *workbook  = workbook_new("test_chart_gap05.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_BAR);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 45938176;
    chart->axis_id_2 = 59715584;
    chart->axis_id_3 = 70848512;
    chart->axis_id_4 = 54519680;

    uint8_t data[5][2] = {
        {1, 6},
        {2, 8},
        {3, 6},
        {4, 4},
        {5, 2}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 2; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");

    lxw_chart_series *series2 = chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5");
    chart_series_set_secondary_axis(series2);

    /* Set gap and overlap for primary axis. */
    chart_set_series_gap(chart, 51);
    chart_set_series_overlap(chart, 12);

    /* Set gap and overlap for secondary axis. */
    chart_set_y2_series_gap(chart, 251);
    chart_set_y2_series_overlap(chart, -27);

    /* Set label position on secondary x axis. */
    chart_axis_set_label_position(chart->x2_axis, LXW_CHART_AXIS_LABEL_POSITION_NEXT_TO);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
