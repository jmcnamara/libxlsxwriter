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

    lxw_workbook  *workbook  = workbook_new("test_chart_column13.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 60474496;
    chart->axis_id_2 = 78612736;

    worksheet_write_string(worksheet, CELL("A1"), "1.1_1", NULL);
    worksheet_write_string(worksheet, CELL("B1"), "2.2_2", NULL);
    worksheet_write_number(worksheet, CELL("A2"), 1,       NULL);
    worksheet_write_number(worksheet, CELL("B2"), 2,       NULL);

    chart_add_series(chart, "=Sheet1!$A$1:$B$1", "=Sheet1!$A$2:$B$2");

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
