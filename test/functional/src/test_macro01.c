/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2019, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_macro01.xlsm");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_add_vba_project(workbook, "images/vbaProject01.bin");

    worksheet_write_number(worksheet, CELL("A1"), 123 , NULL);

    return workbook_close(workbook);
}
