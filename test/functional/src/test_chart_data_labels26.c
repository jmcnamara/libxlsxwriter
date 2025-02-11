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

    lxw_workbook  *workbook  = workbook_new("test_chart_data_labels26.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 48514944;
    chart->axis_id_2 = 48516480;

    uint8_t data[5][4] = {
        {1, 2,  3,  10},
        {2, 4,  6,  20},
        {3, 6,  9,  30},
        {4, 8,  12, 40},
        {5, 10, 15, 50}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 4; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    lxw_chart_series *series = chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");

    lxw_chart_data_label data_label1 = {.value = "33"};
    lxw_chart_data_label *data_labels[] = {&data_label1, NULL};

    chart_series_set_labels_custom(series, data_labels);

    chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$5");

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
