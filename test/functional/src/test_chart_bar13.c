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

    lxw_workbook   *workbook    = workbook_new("test_chart_bar13.xlsx");
    lxw_worksheet  *worksheet1  = workbook_add_worksheet(workbook, NULL);
    lxw_chartsheet *chartsheet1 = workbook_add_chartsheet(workbook, NULL);
    lxw_worksheet  *worksheet2  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet  *worksheet3  = workbook_add_worksheet(workbook, NULL);
    lxw_chartsheet *chartsheet2 = workbook_add_chartsheet(workbook, NULL);
    lxw_worksheet  *worksheet4  = workbook_add_worksheet(workbook, NULL);
    lxw_chart      *chart1      = workbook_add_chart(workbook, LXW_CHART_BAR);
    lxw_chart      *chart2      = workbook_add_chart(workbook, LXW_CHART_BAR);

    (void)worksheet2;
    (void)worksheet3;
    (void)worksheet4;

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart1->axis_id_1 = 40294272;
    chart1->axis_id_2 = 40295808;

    chart2->axis_id_1 = 62356096;
    chart2->axis_id_2 = 62366080;

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
            worksheet_write_number(worksheet1, row, col, data[row][col], NULL);

    chart_add_series(chart1, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart1, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart1, NULL, "=Sheet1!$C$1:$C$5");

    chart_add_series(chart2, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart2, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart2, NULL, "=Sheet1!$C$1:$C$5");

    chartsheet_set_chart(chartsheet1, chart1);
    chartsheet_set_chart(chartsheet2, chart2);

    return workbook_close(workbook);
}
