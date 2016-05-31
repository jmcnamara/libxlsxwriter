/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook   = new_workbook("test_chart_order02.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart4     = workbook_add_chart(workbook, LXW_CHART_PIE);
    lxw_chart     *chart3     = workbook_add_chart(workbook, LXW_CHART_LINE);
    lxw_chart     *chart2     = workbook_add_chart(workbook, LXW_CHART_BAR);
    lxw_chart     *chart1     = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* For testing, copy the randomly generated axis ids in the target file. */
    chart1->axis_id_1 = 54976896;
    chart1->axis_id_2 = 54978432;

    chart2->axis_id_1 = 54310784;
    chart2->axis_id_2 = 54312320;

    chart3->axis_id_1 = 69816704;
    chart3->axis_id_2 = 69818240;

    chart4->axis_id_1 = 69816704;
    chart4->axis_id_2 = 69818240;

    uint8_t data[5][3] = {
        {1, 2,  3},
        {2, 4,  6},
        {3, 6,  9},
        {4, 8,  12},
        {5, 10, 15}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 3; col++) {
            worksheet_write_number(worksheet1, row, col, data[row][col], NULL);
            worksheet_write_number(worksheet2, row, col, data[row][col], NULL);
            worksheet_write_number(worksheet3, row, col, data[row][col], NULL);
        }

    chart_add_series(chart1, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart2, NULL, "=Sheet2!$A$1:$A$5");
    chart_add_series(chart3, NULL, "=Sheet3!$A$1:$A$5");
    chart_add_series(chart4, NULL, "=Sheet1!$B$1:$B$5");

    worksheet_insert_chart(worksheet1, CELL("E9"),  chart1);
    worksheet_insert_chart(worksheet2, CELL("E9"),  chart2);
    worksheet_insert_chart(worksheet3, CELL("E9"),  chart3);
    worksheet_insert_chart(worksheet1, CELL("E24"), chart4);

    return workbook_close(workbook);
}
