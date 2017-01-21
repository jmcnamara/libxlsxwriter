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

    lxw_workbook  *workbook  = new_workbook("test_chart_blank04.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 42268928;
    chart->axis_id_2 = 42990208;

    /* Leave some blanks in the default test data. */

    worksheet_write_number(worksheet, 0, 0, 1,     NULL);
    worksheet_write_number(worksheet, 1, 0, 2,     NULL);

    worksheet_write_number(worksheet, 3, 0, 4,     NULL);
    worksheet_write_number(worksheet, 4, 0, 5,     NULL);

    worksheet_write_number(worksheet, 0, 1, 2,     NULL);
    worksheet_write_number(worksheet, 1, 1, 4,     NULL);

    worksheet_write_number(worksheet, 3, 1, 8,     NULL);
    worksheet_write_number(worksheet, 4, 1, 10,    NULL);

    worksheet_write_number(worksheet, 0, 2, 3,     NULL);
    worksheet_write_number(worksheet, 1, 2, 6,     NULL);

    worksheet_write_number(worksheet, 3, 2, 12,    NULL);
    worksheet_write_number(worksheet, 4, 2, 15,    NULL);



    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$5");

    chart_show_blanks_as(chart, LXW_CHART_BLANKS_AS_CONNECTED);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
