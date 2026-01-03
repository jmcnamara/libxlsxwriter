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

    lxw_workbook  *workbook       = workbook_new("test_chart_combined10.xlsx");
    lxw_worksheet *worksheet      = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart_doughnut = workbook_add_chart(workbook, LXW_CHART_DOUGHNUT);
    lxw_chart     *chart_pie      = workbook_add_chart(workbook, LXW_CHART_PIE);

    /* Write data for the doughnut chart. */
    worksheet_write_string(worksheet, 1, 7, "Donut", NULL);
    worksheet_write_number(worksheet, 2, 7, 25, NULL);
    worksheet_write_number(worksheet, 3, 7, 50, NULL);
    worksheet_write_number(worksheet, 4, 7, 25, NULL);
    worksheet_write_number(worksheet, 5, 7, 100, NULL);

    /* Write data for the pie chart. */
    worksheet_write_string(worksheet, 1, 8, "Pie", NULL);
    worksheet_write_number(worksheet, 2, 8, 75, NULL);
    worksheet_write_number(worksheet, 3, 8, 1, NULL);
    worksheet_write_number(worksheet, 4, 8, 124, NULL);

    lxw_chart_series *series1 = chart_add_series(chart_doughnut, NULL, "=Sheet1!$H$3:$H$6");
    chart_series_set_name(series1, "=Sheet1!$H$2");

    lxw_chart_series *series2 = chart_add_series(chart_pie, NULL, "=Sheet1!$I$3:$I$6");
    chart_series_set_name(series2, "=Sheet1!$I$2");

    chart_combine(chart_doughnut, chart_pie);

    worksheet_insert_chart(worksheet, CELL("E9"), chart_doughnut);

    return workbook_close(workbook);
}
