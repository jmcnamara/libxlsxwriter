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

    lxw_workbook  *workbook  = workbook_new("test_set_column09.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("A:A"),     100, NULL);
    worksheet_set_column(worksheet, COLS("F:H"),     8,   NULL);
    worksheet_set_column(worksheet, COLS("C:D"),     12,  NULL);
    worksheet_set_column(worksheet, COLS("A:A"),     10,  NULL);
    worksheet_set_column(worksheet, COLS("XFD:XFD"), 5,   NULL);
    worksheet_set_column(worksheet, COLS("ZZ:ZZ"),   3,   NULL);

    return workbook_close(workbook);
}
