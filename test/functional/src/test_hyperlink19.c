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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink19.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* This test requires that we check if the cell that the hyperlink refers
     * to is a string. If it isn't be have to add a display attribute. However,
     * looking up the cell is currently too expensive.
     */ 
    worksheet_write_url        (worksheet, CELL("A1"), "http://www.perl.com/", NULL);
    worksheet_write_formula_num(worksheet, CELL("A1"), "=1+1", NULL, 2);

    workbook->sst->string_count = 0;

    return workbook_close(workbook);
}
