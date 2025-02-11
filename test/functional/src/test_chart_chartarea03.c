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

    lxw_workbook  *workbook  = workbook_new("test_chart_chartarea03.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_SCATTER);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 46210048;
    chart->axis_id_2 = 46208512;

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
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$B$1:$B$5"
    );

    chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$C$1:$C$5"
    );


    lxw_chart_line line1 = {.dash_type = LXW_CHART_LINE_DASH_ROUND_DOT};
    lxw_chart_line line2 = {.dash_type = LXW_CHART_LINE_DASH_SQUARE_DOT};

    lxw_chart_fill fill1 = {.color = 0x9999FF};
    lxw_chart_fill fill2 = {.color = 0xFFC000};

    chart_chartarea_set_line(chart, &line1);
    chart_chartarea_set_fill(chart, &fill1);

    chart_plotarea_set_line(chart, &line2);
    chart_plotarea_set_fill(chart, &fill2);


    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
