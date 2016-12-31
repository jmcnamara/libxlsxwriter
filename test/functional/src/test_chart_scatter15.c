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

    lxw_workbook  *workbook  = new_workbook("test_chart_scatter15.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_SCATTER);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 58843520;
    chart->axis_id_2 = 58845440;


    worksheet_write_string(worksheet, 0, 0, "X", NULL);
    worksheet_write_string(worksheet, 0, 1, "Y", NULL);
    worksheet_write_number(worksheet, 1, 0, 1,   NULL);
    worksheet_write_number(worksheet, 1, 1, 10,  NULL);
    worksheet_write_number(worksheet, 2, 0, 3,   NULL);
    worksheet_write_number(worksheet, 2, 1, 30,  NULL);

    chart_add_series(chart, "=Sheet1!$A$2:$A$3", "=Sheet1!$B$2:$B$3");

    lxw_chart_font font1 = {.italic = LXW_TRUE, .baseline = -1};
    chart_axis_set_name_range(chart->x_axis, "Sheet1", 0, 0);
    chart_axis_set_name_font(chart->x_axis, &font1);

    chart_axis_set_name_range(chart->y_axis, "Sheet1", 0, 1);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
