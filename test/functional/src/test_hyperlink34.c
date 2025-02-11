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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink34.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_image_options options1 = {.url = "https://github.com/jmcnamara"};
    lxw_image_options options2 = {.url = "https://github.com"};

    worksheet_insert_image    (worksheet, CELL("A1"), "images/blue.png");
    worksheet_insert_image_opt(worksheet, CELL("B3"), "images/red.jpg", &options1);
    worksheet_insert_image    (worksheet, CELL("D5"), "images/yellow.jpg");
    worksheet_insert_image_opt(worksheet, CELL("F9"), "images/grey.png", &options2);

    return workbook_close(workbook);
}
