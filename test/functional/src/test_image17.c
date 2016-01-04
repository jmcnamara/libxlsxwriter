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

    lxw_workbook  *workbook  = workbook_new("test_image17.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_row(worksheet, 1, 96, NULL);
    worksheet_set_column(worksheet, 2, 2, 18, NULL);

    worksheet_insert_image(worksheet, CELL("C2"), "images/issue32.png");

    return workbook_close(workbook);
}
