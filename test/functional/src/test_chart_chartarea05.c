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

    lxw_workbook  *workbook  = workbook_new("test_chart_chartarea05.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_PIE);

    uint8_t data[3][2] = {
        {2,  60},
        {4,  30},
        {6,  10},
    };

    int row, col;
    for (row = 0; row < 3; row++)
        for (col = 0; col < 2; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    chart_add_series(chart,
         "=Sheet1!$A$1:$A$3",
         "=Sheet1!$B$1:$B$3"
    );


    lxw_chart_line line1 = {.dash_type = LXW_CHART_LINE_DASH_LONG_DASH, .color = 0xFFFF00};
    lxw_chart_line line2 = {.dash_type = LXW_CHART_LINE_DASH_SQUARE_DOT};

    lxw_chart_fill fill1 = {.color = 0x92D050};
    lxw_chart_fill fill2 = {.color = 0xFF0000};

    chart_chartarea_set_line(chart, &line1);
    chart_chartarea_set_fill(chart, &fill1);

    chart_plotarea_set_line(chart, &line2);
    chart_plotarea_set_fill(chart, &fill2);


    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
