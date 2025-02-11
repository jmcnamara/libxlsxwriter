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

    lxw_workbook  *workbook  = workbook_new("test_chart_data_labels49.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 59202176;
    chart->axis_id_2 = 60966784;

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

    lxw_chart_data_label data_label1 = {.value = "=Sheet1!$B$1"};
    lxw_chart_data_label data_label2 = {.value = "=Sheet1!$B$2"};
    lxw_chart_data_label *data_labels1[] = {&data_label1, NULL};
    lxw_chart_data_label *data_labels2[] = {&data_label2, NULL};

    lxw_chart_series *series1 = chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");
    lxw_chart_series *series2 = chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$5");

    chart_series_set_labels(series1);
    chart_series_set_labels(series2);
    chart_series_set_labels_position(series2, LXW_CHART_LABEL_POSITION_INSIDE_BASE);
    chart_series_set_labels_custom(series1, data_labels1);
    chart_series_set_labels_custom(series2, data_labels2);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
