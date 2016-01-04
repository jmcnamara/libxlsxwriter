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

    lxw_workbook  *workbook  = workbook_new("test_image26.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_insert_image(worksheet, CELL("B2"), "images/black_72.png");
    worksheet_insert_image(worksheet, CELL("B8"), "images/black_96.png");
    worksheet_insert_image(worksheet, CELL("B13"), "images/black_150.png");
    worksheet_insert_image(worksheet, CELL("B17"), "images/black_300.png");

    return workbook_close(workbook);
}
