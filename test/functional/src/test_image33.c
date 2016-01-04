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

    lxw_workbook  *workbook  = workbook_new("test_image33.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_image_options options = {.x_offset = -2, .y_offset = -1};

    worksheet_set_column(worksheet, 3, 3, 3.86, NULL);
    worksheet_set_column(worksheet, 4, 4, 1.43, NULL);
    worksheet_set_row(worksheet, 7, 7.5, NULL);
    worksheet_set_row(worksheet, 8, 9.75, NULL);

    worksheet_insert_image_opt(worksheet, CELL("E9"), "images/red.png", &options);

    return workbook_close(workbook);
}
