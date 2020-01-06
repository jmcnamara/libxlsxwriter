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


    lxw_workbook   *workbook   = workbook_new("test_chartsheet09.xlsx");
    lxw_worksheet  *worksheet  = workbook_add_worksheet(workbook, NULL);
    lxw_chartsheet *chartsheet = workbook_add_chartsheet(workbook, NULL);
    lxw_chart      *chart      = workbook_add_chart(workbook, LXW_CHART_BAR);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 49044480;
    chart->axis_id_2 = 49055232;

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
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    lxw_chart_series *series1 = chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$5");

    lxw_chart_line border   = {.color = LXW_COLOR_YELLOW};
    lxw_chart_fill red_fill = {.color = LXW_COLOR_RED};

    chart_series_set_line(series1, &border);
    chart_series_set_fill(series1, &red_fill);

    chartsheet_set_chart(chartsheet, chart);
    chartsheet_activate(chartsheet);

    return workbook_close(workbook);
}
