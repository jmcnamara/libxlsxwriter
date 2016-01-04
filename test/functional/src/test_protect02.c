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

    lxw_workbook  *workbook  = workbook_new("test_protect02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *unlocked = workbook_add_format(workbook);
    format_set_unlocked(unlocked);

    lxw_format *hidden = workbook_add_format(workbook);
    format_set_unlocked(hidden);
    format_set_hidden(hidden);

    worksheet_protect(worksheet, NULL, NULL);

    worksheet_write_number(worksheet, CELL("A1"), 1 , NULL);
    worksheet_write_number(worksheet, CELL("A2"), 2, unlocked);
    worksheet_write_number(worksheet, CELL("A3"), 3, hidden);

    return workbook_close(workbook);
}
