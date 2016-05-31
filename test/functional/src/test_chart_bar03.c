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

    lxw_workbook  *workbook  = new_workbook("test_chart_bar03.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart1    = workbook_add_chart(workbook, LXW_CHART_BAR);
    lxw_chart     *chart2    = workbook_add_chart(workbook, LXW_CHART_BAR);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart1->axis_id_1 = 64265216;
    chart1->axis_id_2 = 64447616;

    chart2->axis_id_1 = 86048128;
    chart2->axis_id_2 = 86058112;

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
            worksheet_write_number(worksheet, row, col, data[row][col] , NULL);

    chart_add_series(chart1,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$B$1:$B$5"
    );

    chart_add_series(chart1,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$C$1:$C$5"
    );

    worksheet_insert_chart(worksheet, CELL("E9"), chart1);

    chart_add_series(chart2,
         "=Sheet1!$A$1:$A$4",
         "=Sheet1!$B$1:$B$4"
    );

    chart_add_series(chart2,
         "=Sheet1!$A$1:$A$4",
         "=Sheet1!$C$1:$C$4"
    );

    worksheet_insert_chart(worksheet, CELL("F25"), chart2);


    return workbook_close(workbook);
}
