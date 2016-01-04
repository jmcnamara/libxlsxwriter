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

    lxw_workbook  *workbook  = workbook_new("test_image23.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_insert_image(worksheet, CELL("B2"), "images/black_72.jpg");
    worksheet_insert_image(worksheet, CELL("B8"), "images/black_96.jpg");
    worksheet_insert_image(worksheet, CELL("B13"), "images/black_150.jpg");
    worksheet_insert_image(worksheet, CELL("B17"), "images/black_300.jpg");

    return workbook_close(workbook);
}
