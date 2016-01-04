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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink09.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_url(worksheet, CELL("A1"), "external:..\\foo.xlsx" , NULL);
    worksheet_write_url(worksheet, CELL("A3"), "external:..\\foo.xlsx#Sheet1!A1" , NULL);
    worksheet_write_url_opt(worksheet, CELL("A5"), "external:\\\\VBOXSVR\\share\\foo.xlsx#Sheet1!B2", NULL, "J:\\foo.xlsx#Sheet1!B2", NULL);

    return workbook_close(workbook);
}
