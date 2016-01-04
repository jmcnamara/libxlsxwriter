/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case for TODO.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_simple03.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, "Data Sheet");
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);

    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    worksheet_write_string(worksheet1, CELL("A1"), "Foo" , NULL);
    worksheet_write_number(worksheet1, CELL("A2"), 123 , NULL);

    worksheet_write_string(worksheet3, CELL("B2"), "Foo" , NULL);
    worksheet_write_string(worksheet3, CELL("B3"), "Bar", bold);
    worksheet_write_number(worksheet3, CELL("C4"), 234 , NULL);

    /* Ensure the active worksheet is overwritten, below. */
    worksheet_activate(worksheet2);

    worksheet_select(worksheet2);
    worksheet_select(worksheet3);
    worksheet_activate(worksheet3);

    return workbook_close(workbook);
}
