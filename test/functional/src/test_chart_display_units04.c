/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_chart_display_units04.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 56159232;
    chart->axis_id_2 = 61364096;

    worksheet_write_number(worksheet, 0, 0, 10000000, NULL);
    worksheet_write_number(worksheet, 1, 0, 20000000, NULL);
    worksheet_write_number(worksheet, 2, 0, 30000000, NULL);
    worksheet_write_number(worksheet, 3, 0, 20000000, NULL);
    worksheet_write_number(worksheet, 4, 0, 10000000, NULL);

    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");

    chart_axis_set_display_units(chart->y_axis, LXW_CHART_AXIS_UNITS_TEN_THOUSANDS);
    chart_axis_set_display_units_visible(chart->y_axis, LXW_FALSE);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
