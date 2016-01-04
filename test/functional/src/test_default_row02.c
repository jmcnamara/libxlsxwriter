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

    lxw_workbook  *workbook  = workbook_new("test_default_row02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    uint8_t row;

    worksheet_set_default_row(worksheet, 15, LXW_TRUE);

    worksheet_write_string(worksheet, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet, CELL("A10"), "Bar" , NULL);

    for (row = 1; row <= 8; row++)
        worksheet_set_row(worksheet, row, 15, NULL);

    return workbook_close(workbook);
}
