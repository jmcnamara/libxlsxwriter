/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test case for writing data in optimization mode.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_optimize21.xlsx");

    workbook->optimize = 1;

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, CELL("A1"), "Foo",     NULL);
    worksheet_write_string(worksheet, CELL("C3"), " Foo",    NULL);
    worksheet_write_string(worksheet, CELL("E5"), "Foo ",    NULL);
    worksheet_write_string(worksheet, CELL("A7"), "\tFoo\t", NULL);

    return workbook_close(workbook);
}
