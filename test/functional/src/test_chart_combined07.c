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

    lxw_workbook  *workbook  = workbook_new("test_chart_combined07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart1    = workbook_add_chart(workbook, LXW_CHART_COLUMN);
    lxw_chart     *chart2    = workbook_add_chart(workbook, LXW_CHART_SCATTER);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart1->axis_id_1 = 81267328;
    chart1->axis_id_2 = 81297792;

    uint8_t data1[5] = {2, 3, 4, 5, 6};
    uint8_t data2[5] = {20, 25, 10, 10, 20};
    uint8_t data3[5] = {5, 10, 15, 10, 5};

    int row;
    for (row = 0; row < 5; row++) {
        worksheet_write_number(worksheet, row, 0, data1[row], NULL);
        worksheet_write_number(worksheet, row, 1, data2[row], NULL);
        worksheet_write_number(worksheet, row, 2, data3[row], NULL);
    }

    chart_add_series(chart1, "=Sheet1!$A$1:$A$5", "=Sheet1!$B$1:$B$5");

    chart_add_series(chart2, "=Sheet1!$A$1:$A$5", "=Sheet1!$C$1:$C$5");

    chart_combine(chart1, chart2);

    worksheet_insert_chart(worksheet, CELL("E9"), chart1);

    return workbook_close(workbook);
}
