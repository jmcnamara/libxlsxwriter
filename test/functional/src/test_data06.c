/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case to test data writing.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_data06.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_format    *format1   = workbook_add_format(workbook);
    lxw_format    *format2   = workbook_add_format(workbook);
    lxw_format    *format3   = workbook_add_format(workbook);

    format1->bold = 1;

    format2->italic = 1;

    format3->bold = 1;
    format3->italic = 1;

    worksheet_write_string(worksheet, CELL("A1"), "Foo", format1);
    worksheet_write_string(worksheet, CELL("A2"), "Bar", format2);
    worksheet_write_string(worksheet, CELL("A3"), "Baz", format3);

    return workbook_close(workbook);
}

