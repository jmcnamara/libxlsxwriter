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

    lxw_workbook  *workbook  = workbook_new("test_chart_display_units01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 69572096;
    chart->axis_id_2 = 93549312;

    worksheet_write_number(worksheet, 0, 0, 10000000, NULL);
    worksheet_write_number(worksheet, 1, 0, 20000000, NULL);
    worksheet_write_number(worksheet, 2, 0, 30000000, NULL);
    worksheet_write_number(worksheet, 3, 0, 20000000, NULL);
    worksheet_write_number(worksheet, 4, 0, 10000000, NULL);

    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
