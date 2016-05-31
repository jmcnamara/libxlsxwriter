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

    lxw_workbook  *workbook  = new_workbook("test_chart_column08.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 68809856;
    chart->axis_id_2 = 68811392;

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

    lxw_chart_series *series1 = chart_add_series(chart,
                                                 "=(Sheet1!$A$1:$A$2,Sheet1!$A$4:$A$5)",
                                                 "=(Sheet1!$B$1:$B$2,Sheet1!$B$4:$B$5)");

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    /* Add the cached data for testing. */
    uint8_t test_data[4][3] = {
        {1, 2,  3},
        {2, 4,  6},
        {4, 8,  12},
        {5, 10, 15}
    };

    lxw_chart_add_data_cache(series1->categories, test_data[0], 4, 3, 0);
    lxw_chart_add_data_cache(series1->values,     test_data[0], 4, 3, 1);



    return workbook_close(workbook);
}
