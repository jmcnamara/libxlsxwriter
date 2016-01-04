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

    lxw_workbook  *workbook  = workbook_new("test_escapes05.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, "Start");
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, "A & B");

    (void)worksheet2;

    worksheet_write_url_opt(worksheet1, CELL("A1"), "internal:'A & B'!A1", NULL, "Jump to A & B" , NULL);

    return workbook_close(workbook);
}
