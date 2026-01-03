/*
 * An example of creating Excel sparklines with libxlsxwriter.
 *
 * Sparklines are small charts that fit in a single cell and allow you to
 * show trends in data at a glance.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("sparklines.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Set the column widths for the data and sparklines. */
    worksheet_set_column(worksheet, COLS("A:F"), 12, NULL);
    worksheet_set_column(worksheet, COLS("G:G"), 16, NULL);

    /* Add some row headers. */
    worksheet_write_string(worksheet, 0, 0, "Line", NULL);
    worksheet_write_string(worksheet, 1, 0, "Column", NULL);
    worksheet_write_string(worksheet, 2, 0, "Win/Loss", NULL);
    worksheet_write_string(worksheet, 3, 0, "With options", NULL);
    worksheet_write_string(worksheet, 4, 0, "Custom colors", NULL);

    /* Write the sample data. */
    int data[][5] = {
        {-2, 2, 3, -1, 0},
        {30, 20, 33, 20, 15},
        {1, -1, -1, 1, -1},
        {5, 10, 3, 8, 6},
        {2, 4, 6, 8, 10},
    };

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            worksheet_write_number(worksheet, row, col + 1, data[row][col], NULL);
        }
    }

    /* Add a simple line sparkline. */
    lxw_sparkline_options line_options = {
        .range = "Sheet1!B1:F1",
    };
    worksheet_add_sparkline(worksheet, 0, 6, &line_options);

    /* Add a column sparkline. */
    lxw_sparkline_options column_options = {
        .range = "Sheet1!B2:F2",
        .type = LXW_SPARKLINE_COLUMN,
    };
    worksheet_add_sparkline(worksheet, 1, 6, &column_options);

    /* Add a win/loss sparkline. */
    lxw_sparkline_options winloss_options = {
        .range = "Sheet1!B3:F3",
        .type = LXW_SPARKLINE_WIN_LOSS,
    };
    worksheet_add_sparkline(worksheet, 2, 6, &winloss_options);

    /* Add a sparkline with options: markers, high/low points, line weight. */
    lxw_sparkline_options markers_options = {
        .range = "Sheet1!B4:F4",
        .markers = LXW_TRUE,
        .high_point = LXW_TRUE,
        .low_point = LXW_TRUE,
        .first_point = LXW_TRUE,
        .last_point = LXW_TRUE,
        .weight = 1.5,
    };
    worksheet_add_sparkline(worksheet, 3, 6, &markers_options);

    /* Add a sparkline with custom colors. */
    lxw_sparkline_options color_options = {
        .range = "Sheet1!B5:F5",
        .series_color = 0x0000FF,   /* Blue line */
        .markers = LXW_TRUE,
        .markers_color = 0xFF0000,  /* Red markers */
    };
    worksheet_add_sparkline(worksheet, 4, 6, &color_options);

    return workbook_close(workbook);
}
