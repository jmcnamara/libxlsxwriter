/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
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

    uint8_t data1[5] = {1, 2, 3, 4, 5};
    uint8_t data2[5] = {6, 8, 6, 4, 2};

    int row;
    for (row = 0; row < 5; row++) {
        worksheet_write_number(worksheet, row, 0, data1[row], NULL);
        worksheet_write_number(worksheet, row, 1, data2[row], NULL);
    }

    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5", 1);

    /* Set gap and overlap for primary axis. */
    chart_set_series_gap(chart, 51);
    chart_set_series_overlap(chart, 12);

    /* Set gap and overlap for secondary axis. */
    chart_set_series_gap_y2(chart, 251);
    chart_set_series_overlap_y2(chart, -27);

    /* Set x2 axis label position to next_to. */
    chart_axis_set_label_position(chart->x2_axis, LXW_CHART_AXIS_LABEL_POSITION_NEXT_TO);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
