/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case to test data writing.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_data06.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_format    *format1   = workbook_add_format(workbook);
    lxw_format    *format2   = workbook_add_format(workbook);
    lxw_format    *format3   = workbook_add_format(workbook);

    format1->bold = 1;

    format2->italic = 1;

    format3->bold = 1;
    format3->italic = 1;

    worksheet_write_string(worksheet, 0, 0, "Foo", format1);
    worksheet_write_string(worksheet, 1, 0, "Bar", format2);
    worksheet_write_string(worksheet, 2, 0, "Baz", format3);

    return workbook_close(workbook);
}

