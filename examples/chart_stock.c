/*
 * An example of creating Excel Stock charts with libxlsxwriter.
 *
 * Stock charts display High-Low-Close data and are suitable for financial
 * and other data that shows variation over time.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("chart_stock.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_STOCK);

    /* Add a bold format for the headers. */
    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    /* Add a date format for the date column. */
    lxw_format *date_format = workbook_add_format(workbook);
    format_set_num_format(date_format, "dd/mm/yyyy");

    /* Add the worksheet data that the chart will refer to. */
    /* Dates as Excel serial numbers (Jan 1-5, 2007). */
    double dates[5] = {39083, 39084, 39085, 39086, 39087};
    double high[5]  = {27.2, 25.03, 19.05, 20.34, 18.5};
    double low[5]   = {23.49, 19.55, 15.12, 17.84, 16.34};
    double close[5] = {25.45, 23.05, 17.32, 20.45, 17.34};

    /* Write the column headers. */
    worksheet_write_string(worksheet, 0, 0, "Date",  bold);
    worksheet_write_string(worksheet, 0, 1, "High",  bold);
    worksheet_write_string(worksheet, 0, 2, "Low",   bold);
    worksheet_write_string(worksheet, 0, 3, "Close", bold);

    /* Write the data columns. */
    for (int row = 0; row < 5; row++) {
        worksheet_write_number(worksheet, row + 1, 0, dates[row], date_format);
        worksheet_write_number(worksheet, row + 1, 1, high[row], NULL);
        worksheet_write_number(worksheet, row + 1, 2, low[row], NULL);
        worksheet_write_number(worksheet, row + 1, 3, close[row], NULL);
    }

    /* Widen the columns for visibility. */
    worksheet_set_column(worksheet, COLS("A:D"), 11, NULL);

    /* Add a series for each of the High-Low-Close columns. Stock charts
     * should have 3 series (High-Low-Close) or 4 series (Open-High-Low-Close). */
    chart_add_series(chart, "=Sheet1!$A$2:$A$6", "=Sheet1!$B$2:$B$6", 0);
    chart_add_series(chart, "=Sheet1!$A$2:$A$6", "=Sheet1!$C$2:$C$6", 0);
    chart_add_series(chart, "=Sheet1!$A$2:$A$6", "=Sheet1!$D$2:$D$6", 0);

    /* Add a chart title and axis labels. */
    chart_title_set_name(chart, "High-Low-Close");
    chart_axis_set_name(chart->x_axis, "Date");
    chart_axis_set_name(chart->y_axis, "Share price");

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
