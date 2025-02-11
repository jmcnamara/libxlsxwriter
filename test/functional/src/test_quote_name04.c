/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main()
{
    lxw_workbook *workbook = workbook_new("test_quote_name04.xlsx");

    uint8_t data[5][3] = {
        {1, 2, 3},
        {2, 4, 6},
        {3, 6, 9},
        {4, 8, 12},
        {5, 10, 15}};

    char *sheetname = "Sheet 1";
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, sheetname);

    for (int row = 0; row < 5; row++)
        for (int col = 0; col < 3; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_PIE);

    lxw_chart_series *series = chart_add_series(chart, NULL, NULL);
    chart_series_set_values(series, sheetname, 0, 0, 4, 0);
    chart_series_set_name_range(series, sheetname, 0, 0);

    chart_title_set_name(chart, "Foo");

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}