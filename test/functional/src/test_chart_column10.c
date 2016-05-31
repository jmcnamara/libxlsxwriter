/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_chart_column10.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 45686144;
    chart->axis_id_2 = 45722240;

    char   *data_1[5] = { "A", "B", "C", "D", "E"};
    uint8_t data_2[5] = {  1,   2,   3,   2,   1 };

    int row;
    for (row = 0; row < 5; row++) {
        worksheet_write_string(worksheet, row, 0, data_1[row], NULL);
        worksheet_write_number(worksheet, row, 1, data_2[row], NULL);
    }

    chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$B$1:$B$5"
    );

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
