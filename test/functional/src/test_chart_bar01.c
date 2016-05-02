/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_chart_bar01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_BAR);
    lxw_chart_series *series1;
    lxw_chart_series *series2;
    int row, col;

    uint8_t data[5][3] = {
        {1, 2,  3},
        {2, 4,  6},
        {3, 6,  9},
        {4, 8,  12},
        {5, 10, 15}
    };

    for (row = 0; row < 5; row++)
        for (col = 0; col < 3; col++)
            worksheet_write_number(worksheet, row, col, data[row][col] , NULL);


    /* For testing. */
    /* chart->axis_id1 = 64052224; */
    /* chart->axis_ids = 64055552; */

    series1 = chart_add_series(chart, NULL, "Sheet1!$A$1:$A$5");
    series2 = chart_add_series(chart, NULL, "Sheet1!$B$1:$B$5");


    uint8_t test_data1[] = {1, 2, 3, 4, 5};
    uint8_t test_data2[] = {2, 4, 6, 8, 10};

    lxw_chart_add_data_cache(&series1->values, 5, test_data1);
    lxw_chart_add_data_cache(&series2->values, 5, test_data2);


    /* $chart.add_series( */
    /*     'categories': '=Sheet1! $A$1:$A$5', */
    /*     'values': '=Sheet1! $B$1:$B$5', */
    /* ); */

    /* $chart.add_series( */
    /*     'categories': '=Sheet1!$A$1:$A$5', */
    /*     'values': '=Sheet1!$C$1:$C$5', */
    /* ); */
    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
