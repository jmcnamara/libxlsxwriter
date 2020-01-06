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

    lxw_workbook  *workbook  = workbook_new("test_chart_pattern07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart->axis_id_1 = 110902272;
    chart->axis_id_2 = 110756608;

    uint8_t data[3][8] = {
        {2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2}
    };

    int row, col;
    for (row = 0; row < 3; row++)
        for (col = 0; col < 8; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    lxw_chart_series *series1 = chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$3");
    lxw_chart_series *series2 = chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$3");
    lxw_chart_series *series3 = chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$3");
    lxw_chart_series *series4 = chart_add_series(chart, NULL, "=Sheet1!$D$1:$D$3");
    lxw_chart_series *series5 = chart_add_series(chart, NULL, "=Sheet1!$E$1:$E$3");
    lxw_chart_series *series6 = chart_add_series(chart, NULL, "=Sheet1!$F$1:$F$3");
    lxw_chart_series *series7 = chart_add_series(chart, NULL, "=Sheet1!$G$1:$G$3");
    lxw_chart_series *series8 = chart_add_series(chart, NULL, "=Sheet1!$H$1:$H$3");

    lxw_chart_pattern pattern1 = {.type     = LXW_CHART_PATTERN_PERCENT_40,
                                  .fg_color = 0xC00000,
                                  .bg_color = 0xFFFFFF};

    lxw_chart_pattern pattern2 = {.type     = LXW_CHART_PATTERN_PERCENT_90,
                                  .fg_color = 0xFF0000};

    lxw_chart_pattern pattern3 = {.type     = LXW_CHART_PATTERN_WIDE_UPWARD_DIAGONAL,
                                  .fg_color = 0xFFC000};

    lxw_chart_pattern pattern4 = {.type     = LXW_CHART_PATTERN_DARK_HORIZONTAL,
                                  .fg_color = 0xFFFF00};

    lxw_chart_pattern pattern5 = {.type     = LXW_CHART_PATTERN_LARGE_CONFETTI,
                                  .fg_color = 0x92D050};

    lxw_chart_pattern pattern6 = {.type     = LXW_CHART_PATTERN_PLAID,
                                  .fg_color = 0x00B050};

    lxw_chart_pattern pattern7 = {.type     = LXW_CHART_PATTERN_SPHERE,
                                  .fg_color = 0x00B0F0};

    lxw_chart_pattern pattern8 = {.type     = LXW_CHART_PATTERN_SOLID_DIAMOND,
                                  .fg_color = 0x0070C0};

    chart_series_set_pattern(series1, &pattern1);
    chart_series_set_pattern(series2, &pattern2);
    chart_series_set_pattern(series3, &pattern3);
    chart_series_set_pattern(series4, &pattern4);
    chart_series_set_pattern(series5, &pattern5);
    chart_series_set_pattern(series6, &pattern6);
    chart_series_set_pattern(series7, &pattern7);
    chart_series_set_pattern(series8, &pattern8);

    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
