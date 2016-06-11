/*
 * An example of a simple Excel chart using the libxlsxwriter library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

/* Write some data to the worksheet. */
void write_worksheet_data(lxw_worksheet *worksheet) {

    uint8_t data[5][3] = {
        /* Three columns of data. */
        {1,   2,   3},
        {2,   4,   6},
        {3,   6,   9},
        {4,   8,  12},
        {5,  10,  15}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 3; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);
}

/* Create a worksheet with a chart. */
int main() {

    lxw_workbook  *workbook  = new_workbook("chart.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Write some data for the chart. */
    write_worksheet_data(worksheet);

    /* Create a chart object. */
    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Configure the chart. In simplest case we just add some value data
     * series. The NULL categories will default to 1 to 5 like in Excel.
     */
    chart_add_series(chart, NULL, "Sheet1!$A$1:$A$5");
    chart_add_series(chart, NULL, "Sheet1!$B$1:$B$5");
    chart_add_series(chart, NULL, "Sheet1!$C$1:$C$5");

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("B7"), chart);

    return workbook_close(workbook);
}
