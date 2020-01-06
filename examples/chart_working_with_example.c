/*
 * An example of a simple Excel chart using the libxlsxwriter library. This
 * example is used in the "Working with Charts" section of the docs.
 *
 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"


/* Create a worksheet with a chart. */
int main() {

    lxw_workbook  *workbook  = workbook_new("chart_line.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart *chart;
    lxw_chart_series *series;

    /* Write some data for the chart. */
    worksheet_write_number(worksheet, 0, 0, 10, NULL);
    worksheet_write_number(worksheet, 1, 0, 40, NULL);
    worksheet_write_number(worksheet, 2, 0, 50, NULL);
    worksheet_write_number(worksheet, 3, 0, 20, NULL);
    worksheet_write_number(worksheet, 4, 0, 10, NULL);
    worksheet_write_number(worksheet, 5, 0, 50, NULL);

    /* Create a chart object. */
    chart = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* Configure the chart. */
    series = chart_add_series(chart, NULL, "Sheet1!$A$1:$A$6");

    (void)series; /* Do something with series in the real examples. */

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("C1"), chart);

    return workbook_close(workbook);
}
