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
    lxw_workbook *workbook = workbook_new("test_quote_name02.xlsx");

    uint8_t data[5][3] = {
        {1, 2, 3},
        {2, 4, 6},
        {3, 6, 9},
        {4, 8, 12},
        {5, 10, 15}};

    char *sheetnames[] = {
        "Sheet'1",
        "S'heet'2",
        "Sheet(3",
        "Sheet)4",
        "Sheet+5",
        "Sheet,6",
        "Sheet-7",
        "Sheet;8",
    };

    for (int i = 0; i < 8; i++)
    {
        lxw_worksheet *worksheet = workbook_add_worksheet(workbook, sheetnames[i]);

        for (int row = 0; row < 5; row++)
            for (int col = 0; col < 3; col++)
                worksheet_write_number(worksheet, row, col, data[row][col], NULL);

        lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_PIE);
        lxw_chart_series *series = chart_add_series(chart, NULL, NULL);
        chart_series_set_values(series, sheetnames[i], 0, 0, 4, 0);

        lxw_chart_options options = {.x_offset = 26, .y_offset = 17};
        worksheet_insert_chart_opt(worksheet, CELL("E6"), chart, &options);
    }

    return workbook_close(workbook);
}
