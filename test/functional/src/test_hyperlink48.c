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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink48.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_image_options options = {.url = "https://github.com/jmcnamara"};

    worksheet_insert_image_opt(worksheet, CELL("E9"),  "images/red.png", &options);
    worksheet_insert_image_opt(worksheet, CELL("E13"), "images/red.png", &options);

    return workbook_close(workbook);
}
