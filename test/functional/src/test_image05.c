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

    lxw_workbook  *workbook  = workbook_new("test_image05.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_insert_image(worksheet, CELL("A1"), "images/blue.png");
    worksheet_insert_image(worksheet, CELL("B3"), "images/red.jpg");
    worksheet_insert_image(worksheet, CELL("D5"), "images/yellow.jpg");
    worksheet_insert_image(worksheet, CELL("F9"), "images/grey.png");

    return workbook_close(workbook);
}
