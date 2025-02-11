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

    lxw_workbook   *workbook    = workbook_new("test_chart_bar14.xlsx");
    lxw_worksheet  *worksheet1  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet  *worksheet2  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet  *worksheet3  = workbook_add_worksheet(workbook, NULL);
    lxw_chartsheet *chartsheet1 = workbook_add_chartsheet(workbook, NULL);
    lxw_chart      *chart1      = workbook_add_chart(workbook, LXW_CHART_BAR);
    lxw_chart      *chart2      = workbook_add_chart(workbook, LXW_CHART_BAR);
    lxw_chart      *chart3      = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    workbook_unset_default_url_format(workbook);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart1->axis_id_1 = 40294272;
    chart1->axis_id_2 = 40295808;
    chart2->axis_id_1 = 40261504;
    chart2->axis_id_2 = 65749760;
    chart3->axis_id_1 = 65465728;
    chart3->axis_id_2 = 66388352;

    (void)worksheet1;
    (void)worksheet3;

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
            worksheet_write_number(worksheet2, row, col, data[row][col], NULL);

    worksheet_write_url(worksheet2, CELL("A6"), "http://www.perl.com/" , NULL);

    chart_add_series(chart3, NULL, "=Sheet2!$A$1:$A$5");
    chart_add_series(chart3, NULL, "=Sheet2!$B$1:$B$5");
    chart_add_series(chart3, NULL, "=Sheet2!$C$1:$C$5");

    chart_add_series(chart1, NULL, "=Sheet2!$A$1:$A$5");
    chart_add_series(chart1, NULL, "=Sheet2!$B$1:$B$5");
    chart_add_series(chart1, NULL, "=Sheet2!$C$1:$C$5");

    chart_add_series(chart2, NULL, "=Sheet2!$A$1:$A$5");

    worksheet_insert_chart(worksheet2, CELL("E9"), chart1);
    worksheet_insert_chart(worksheet2, CELL("F25"), chart2);

    chartsheet_set_chart(chartsheet1, chart3);

    return workbook_close(workbook);
}
