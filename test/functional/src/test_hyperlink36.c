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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink36.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart     *chart     = workbook_add_chart(workbook, LXW_CHART_PIE);

    lxw_image_options options = {.url = "https://github.com/jmcnamara"};

    worksheet_write_number(worksheet, CELL("A1"), 1, NULL);
    worksheet_write_number(worksheet, CELL("A2"), 2, NULL);

    worksheet_insert_image_opt(worksheet, CELL("E9"), "images/red.png", &options);

    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$2");

    worksheet_insert_chart(worksheet, CELL("E12"), chart);

    return workbook_close(workbook);
}
