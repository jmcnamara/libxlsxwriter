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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink03.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);

    worksheet_write_url(worksheet1, CELL("A1"),  "http://www.perl.org/", NULL);
    worksheet_write_url(worksheet1, CELL("D4"),  "http://www.perl.org/", NULL);
    worksheet_write_url(worksheet1, CELL("A8"),  "http://www.perl.org/", NULL);
    worksheet_write_url(worksheet1, CELL("B6"),  "http://www.cpan.org/", NULL);
    worksheet_write_url(worksheet1, CELL("F12"), "http://www.cpan.org/", NULL);

    worksheet_write_url(worksheet2, CELL("C2"),  "http://www.google.com/", NULL);
    worksheet_write_url(worksheet2, CELL("C5"),  "http://www.cpan.org/",   NULL);
    worksheet_write_url(worksheet2, CELL("C7"),  "http://www.perl.org/",   NULL);

    return workbook_close(workbook);
}
