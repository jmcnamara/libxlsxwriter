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

    lxw_workbook  *workbook  = workbook_new("test_chart_pattern10.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 143227136;
    chart->axis_id_2 = 143245312;

    uint8_t data[3][8] = {
        {2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2}
    };

    int row, col;
    for (row = 0; row < 3; row++)
        for (col = 0; col < 8; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$3");
    chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$3");
    chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$3");
    chart_add_series(chart, NULL, "=Sheet1!$D$1:$D$3");
    chart_add_series(chart, NULL, "=Sheet1!$E$1:$E$3");
    chart_add_series(chart, NULL, "=Sheet1!$F$1:$F$3");
    chart_add_series(chart, NULL, "=Sheet1!$G$1:$G$3");
    chart_add_series(chart, NULL, "=Sheet1!$H$1:$H$3");

    lxw_chart_pattern pattern = {.type     = LXW_CHART_PATTERN_PERCENT_5,
                                 .fg_color = LXW_COLOR_RED,
                                 .bg_color = LXW_COLOR_YELLOW};

    chart_plotarea_set_pattern(chart, &pattern);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
