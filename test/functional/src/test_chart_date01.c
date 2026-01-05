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

    lxw_workbook  *workbook  = workbook_new("test_chart_date01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* Date format. */
    lxw_format *date_format = workbook_add_format(workbook);
    format_set_num_format_index(date_format, 14);

    /* Set column width. */
    worksheet_set_column(worksheet, 0, 0, 12, NULL);

    /* Write date values (2013-01-01 to 2013-01-10). */
    lxw_datetime dates[] = {
        {2013, 1, 1, 0, 0, 0.0},
        {2013, 1, 2, 0, 0, 0.0},
        {2013, 1, 3, 0, 0, 0.0},
        {2013, 1, 4, 0, 0, 0.0},
        {2013, 1, 5, 0, 0, 0.0},
        {2013, 1, 6, 0, 0, 0.0},
        {2013, 1, 7, 0, 0, 0.0},
        {2013, 1, 8, 0, 0, 0.0},
        {2013, 1, 9, 0, 0, 0.0},
        {2013, 1, 10, 0, 0, 0.0}
    };

    for (int i = 0; i < 10; i++) {
        worksheet_write_datetime(worksheet, i, 0, &dates[i], date_format);
    }

    /* Write data values. */
    int values[] = {10, 30, 20, 40, 20, 60, 50, 40, 30, 30};
    for (int i = 0; i < 10; i++) {
        worksheet_write_number(worksheet, i, 1, values[i], NULL);
    }

    /* Add chart series. */
    chart_add_series(chart, "=Sheet1!$A$1:$A$10", "=Sheet1!$B$1:$B$10");

    /* Set the X axis as a date axis with linked format. */
    chart_axis_set_date_axis(chart->x_axis);
    chart_axis_set_num_format(chart->x_axis, "dd/mm/yyyy");
    chart->x_axis->source_linked = LXW_TRUE;

    /* Insert the chart. */
    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
