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

    lxw_workbook  *workbook  = workbook_new("test_default_row05.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    uint8_t row;

    worksheet_set_default_row(worksheet, 24, LXW_TRUE);

    worksheet_write_string(worksheet, CELL("A1"),  "Foo" , NULL);
    worksheet_write_string(worksheet, CELL("A10"), "Bar" , NULL);
    worksheet_write_string(worksheet, CELL("A20"), "Baz" , NULL);

    for (row = 1; row <= 8; row++)
        worksheet_set_row(worksheet, row, 24, NULL);

    for (row = 10; row <= 19; row++)
        worksheet_set_row(worksheet, row, 24, NULL);

    return workbook_close(workbook);
}
