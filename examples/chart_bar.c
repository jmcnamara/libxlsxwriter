/*
 * An example of writing a bar chart in a worksheet using the libxlsxwriter
 * library.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("chart_bar.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    int row, col;

    /* Some data for the chart. */
    uint8_t data[5][3] = {
        {1, 2,  3},
        {2, 4,  6},
        {3, 6,  9},
        {4, 8,  12},
        {5, 10, 15}
    };

    /* Write the data to the worksheet. */
    for (row = 0; row < 5; row++)
        for (col = 0; col < 3; col++)
            worksheet_write_number(worksheet, row, col, data[row][col] , NULL);


    /* Create a chart object. */
    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_BAR);

    /* Add two series to the chart. */
    chart_add_series(chart, NULL, "Sheet1!$A$1:$A$5");
    chart_add_series(chart, NULL, "Sheet1!$B$1:$B$5");

    /* Position the chart in the worksheet. */
    worksheet_insert_chart(worksheet, CELL("B7"), chart);

    return workbook_close(workbook);
}
