/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook   *workbook   = workbook_new("test_chartsheet08.xlsx");
    lxw_worksheet  *worksheet  = workbook_add_worksheet(workbook, NULL);
    lxw_chartsheet *chartsheet = workbook_add_chartsheet(workbook, NULL);
    lxw_chart      *chart      = workbook_add_chart(workbook, LXW_CHART_BAR);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 61297792;
    chart->axis_id_2 = 61299328;

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

    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$5");



    chartsheet_set_margins(chartsheet, 0.511811023622047,
                                       0.511811023622047,
                                       0.551181102362204,
                                       0.944881889763779);

    chartsheet_set_paper(chartsheet, 9);
    chartsheet_set_portrait(chartsheet);

    lxw_header_footer_options header_options = {.margin = 0.118110236220472 };
    chartsheet_set_header_opt(chartsheet, "&CPage &P", &header_options);
    chartsheet_set_footer_opt(chartsheet, "&C&A",      &header_options);

    /* For testing. */
    chartsheet->worksheet->horizontal_dpi = 200;
    chartsheet->worksheet->vertical_dpi   = 200;

    chartsheet_set_chart(chartsheet, chart);

return workbook_close(workbook);
}
