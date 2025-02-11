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

    lxw_workbook  *workbook  = workbook_new("test_chart_data_labels21.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_PIE);

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


    lxw_chart_series *series1 = chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");

    lxw_chart_font font = {.name = "Consolas", .pitch_family = 49, .charset = 0, .baseline = -1};

    chart_series_set_labels(series1);
    chart_series_set_labels_options(series1, LXW_TRUE, LXW_TRUE, LXW_TRUE);
    chart_series_set_labels_position(series1, LXW_CHART_LABEL_POSITION_INSIDE_END);
    chart_series_set_labels_separator(series1, LXW_CHART_LABEL_SEPARATOR_SEMICOLON);
    chart_series_set_labels_leader_line(series1);
    chart_series_set_labels_legend(series1);
    chart_series_set_labels_percentage(series1);
    chart_series_set_labels_num_format(series1, "#,##0.00");
    chart_series_set_labels_font(series1, &font);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
