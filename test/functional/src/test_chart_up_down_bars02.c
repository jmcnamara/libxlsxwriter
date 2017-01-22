/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_chart_up_down_bars02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 49019136;
    chart->axis_id_2 = 49222016;

    uint8_t data[5][3] = {
        {1, 2,  3},
        {2, 4,  6},
        {3, 6,  5},
        {4, 8,  12},
        {5, 10, 15}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 3; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$B$1:$B$5"
    );

    chart_add_series(chart,
         "=Sheet1!$A$1:$A$5",
         "=Sheet1!$C$1:$C$5"
    );

    lxw_chart_line up_line   = {.color = LXW_COLOR_YELLOW};
    lxw_chart_fill up_fill   = {.color = LXW_COLOR_RED};
    lxw_chart_line down_line = {.color = 0x00B0F0,
                                .dash_type = LXW_CHART_LINE_DASH_SQUARE_DOT};
    lxw_chart_fill down_fill = {.color = 0x00B050};

    chart_set_up_down_bars_format(chart, &up_line, &up_fill, &down_line, &down_fill);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
