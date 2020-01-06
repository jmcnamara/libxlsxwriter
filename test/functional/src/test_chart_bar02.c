/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2019, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook   = workbook_new("test_chart_bar02.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart      = workbook_add_chart(workbook, LXW_CHART_BAR);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 93218304;
    chart->axis_id_2 = 93219840;

    uint8_t data[5][3] = {
        {1, 2,  3},
        {2, 4,  6},
        {3, 6,  9},
        {4, 8,  12},
        {5, 10, 15}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 3; col++)
            worksheet_write_number(worksheet2, row, col, data[row][col] , NULL);

    worksheet_write_string(worksheet1, CELL("A1"), "Foo" , NULL);

    chart_add_series(chart, "Sheet2!$A$1:$A$5", "Sheet2!$B$1:$B$5");
    chart_add_series(chart, "Sheet2!$A$1:$A$5", "Sheet2!$C$1:$C$5");


    worksheet_insert_chart(worksheet2, CELL("E9"), chart);

    return workbook_close(workbook);
}
