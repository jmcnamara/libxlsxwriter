/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_hyperlink31.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_format *format1 = workbook_add_format(workbook);

    format_set_bold(format1);

    worksheet_write_string(worksheet, CELL("A1"), "Test", format1);
    worksheet_write_url(worksheet, CELL("A3"), "http://www.python.org/" , NULL);

    return workbook_close(workbook);
}
