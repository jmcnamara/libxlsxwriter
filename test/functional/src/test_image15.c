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

    lxw_workbook  *workbook  = workbook_new("test_image15.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_image_options options = {.x_offset = 13, .y_offset = 2};

    worksheet_set_row(worksheet,    1,     4.5,  NULL);
    worksheet_set_row(worksheet,    2,    35.25, NULL);
    worksheet_set_column(worksheet, 2, 4,  3.29, NULL);
    worksheet_set_column(worksheet, 5, 5, 10.71, NULL);

    worksheet_insert_image_opt(worksheet, CELL("C2"), "images/logo.png", &options);

    return workbook_close(workbook);
}
