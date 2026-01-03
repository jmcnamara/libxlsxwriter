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

    lxw_workbook  *workbook  = workbook_new("test_chart_line07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 77034624;
    chart->axis_id_2 = 77036544;
    chart->axis_id_3 = 95388032;
    chart->axis_id_4 = 103040896;

    uint8_t data1[5] = {1, 2, 3, 4, 5};
    uint8_t data2[5] = {10, 40, 50, 20, 10};
    uint8_t data3[7] = {1, 2, 3, 4, 5, 6, 7};
    uint8_t data4[7] = {30, 10, 20, 40, 30, 10, 20};

    int row;
    for (row = 0; row < 5; row++) {
        worksheet_write_number(worksheet, row, 0, data1[row], NULL);
        worksheet_write_number(worksheet, row, 1, data2[row], NULL);
    }
    for (row = 0; row < 7; row++) {
        worksheet_write_number(worksheet, row, 2, data3[row], NULL);
        worksheet_write_number(worksheet, row, 3, data4[row], NULL);
    }

    /* Add series to primary axis. */
    chart_add_series(chart,
        "=Sheet1!$A$1:$A$5",
        "=Sheet1!$B$1:$B$5",
        0);

    /* Add series to secondary axis. */
    chart_add_series(chart,
        "=Sheet1!$C$1:$C$7",
        "=Sheet1!$D$1:$D$7",
        1);

    /* Configure secondary X axis to show labels. */
    chart_axis_set_label_position(chart->x2_axis, LXW_CHART_AXIS_LABEL_POSITION_NEXT_TO);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
