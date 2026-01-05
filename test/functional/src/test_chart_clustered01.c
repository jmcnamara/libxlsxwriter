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

    lxw_workbook  *workbook  = workbook_new("test_chart_clustered01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 45886080;
    chart->axis_id_2 = 45928832;

    /* Write the worksheet data. */
    worksheet_write_string(worksheet, 0, 0, "Types",      NULL);
    worksheet_write_string(worksheet, 0, 1, "Sub Type",   NULL);
    worksheet_write_string(worksheet, 0, 2, "Value 1",    NULL);
    worksheet_write_string(worksheet, 0, 3, "Value 2",    NULL);
    worksheet_write_string(worksheet, 0, 4, "Value 3",    NULL);

    worksheet_write_string(worksheet, 1, 0, "Type 1",     NULL);
    worksheet_write_string(worksheet, 1, 1, "Sub Type A", NULL);
    worksheet_write_number(worksheet, 1, 2, 5000,         NULL);
    worksheet_write_number(worksheet, 1, 3, 8000,         NULL);
    worksheet_write_number(worksheet, 1, 4, 6000,         NULL);

    worksheet_write_string(worksheet, 2, 0, "",           NULL);
    worksheet_write_string(worksheet, 2, 1, "Sub Type B", NULL);
    worksheet_write_number(worksheet, 2, 2, 2000,         NULL);
    worksheet_write_number(worksheet, 2, 3, 3000,         NULL);
    worksheet_write_number(worksheet, 2, 4, 4000,         NULL);

    worksheet_write_string(worksheet, 3, 0, "",           NULL);
    worksheet_write_string(worksheet, 3, 1, "Sub Type C", NULL);
    worksheet_write_number(worksheet, 3, 2, 250,          NULL);
    worksheet_write_number(worksheet, 3, 3, 1000,         NULL);
    worksheet_write_number(worksheet, 3, 4, 2000,         NULL);

    worksheet_write_string(worksheet, 4, 0, "Type 2",     NULL);
    worksheet_write_string(worksheet, 4, 1, "Sub Type D", NULL);
    worksheet_write_number(worksheet, 4, 2, 6000,         NULL);
    worksheet_write_number(worksheet, 4, 3, 6000,         NULL);
    worksheet_write_number(worksheet, 4, 4, 6500,         NULL);

    worksheet_write_string(worksheet, 5, 0, "",           NULL);
    worksheet_write_string(worksheet, 5, 1, "Sub Type E", NULL);
    worksheet_write_number(worksheet, 5, 2, 500,          NULL);
    worksheet_write_number(worksheet, 5, 3, 300,          NULL);
    worksheet_write_number(worksheet, 5, 4, 200,          NULL);

    lxw_chart_series *series1 = chart_add_series(chart,
                                                  "=Sheet1!$A$2:$B$6",
                                                  "=Sheet1!$C$2:$C$6");
    chart_series_set_name(series1, "=Sheet1!$C$1");

    lxw_chart_series *series2 = chart_add_series(chart,
                                                  "=Sheet1!$A$2:$B$6",
                                                  "=Sheet1!$D$2:$D$6");
    chart_series_set_name(series2, "=Sheet1!$D$1");

    lxw_chart_series *series3 = chart_add_series(chart,
                                                  "=Sheet1!$A$2:$B$6",
                                                  "=Sheet1!$E$2:$E$6");
    chart_series_set_name(series3, "=Sheet1!$E$1");

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
