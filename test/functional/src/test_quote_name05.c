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
    lxw_workbook *workbook = workbook_new("test_quote_name05.xlsx");

    char *sheetname = "Sheet1";
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, sheetname);
    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 54437760;
    chart->axis_id_2 = 59195776;

    uint8_t data[5][3] = {
        {1, 2, 3},
        {2, 4, 6},
        {3, 6, 9},
        {4, 8, 12},
        {5, 10, 15}};

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 3; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    worksheet_repeat_rows(worksheet, 0, 1);
    worksheet_set_portrait(worksheet);
    worksheet->vertical_dpi = 200;

    lxw_chart_series *series1 = chart_add_series(chart, NULL, NULL);
    lxw_chart_series *series2 = chart_add_series(chart, NULL, NULL);
    lxw_chart_series *series3 = chart_add_series(chart, NULL, NULL);

    chart_series_set_values(series1, sheetname, 0, 0, 4, 0);
    chart_series_set_values(series2, sheetname, 0, 1, 4, 1);
    chart_series_set_values(series3, sheetname, 0, 2, 4, 2);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
