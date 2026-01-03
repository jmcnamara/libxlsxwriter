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

    lxw_workbook  *workbook  = workbook_new("test_chart_line02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 63593856;
    chart->axis_id_2 = 63612032;
    chart->axis_id_3 = 63615360;
    chart->axis_id_4 = 63613568;

    uint8_t data1[5] = {1, 2, 3, 4, 5};
    uint8_t data2[5] = {6, 8, 6, 4, 2};

    int row;
    for (row = 0; row < 5; row++) {
        worksheet_write_number(worksheet, row, 0, data1[row], NULL);
        worksheet_write_number(worksheet, row, 1, data2[row], NULL);
    }

    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5", 1);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
