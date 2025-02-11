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

    lxw_workbook  *workbook  = workbook_new("test_image51.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_image_options options1 = {.url = "https://duckduckgo.com/?q=1"};
    lxw_image_options options2 = {.url = "https://duckduckgo.com/?q=2"};

    worksheet_insert_image_opt(worksheet, CELL("E9"),  "images/red.png",  &options1);
    worksheet_insert_image_opt(worksheet, CELL("E13"), "images/red2.png", &options2);

    return workbook_close(workbook);
}
