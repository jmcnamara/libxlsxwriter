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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink06.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_url_opt(worksheet, CELL("A1"), "external:C:\\Temp\\foo.xlsx",            NULL, NULL,       NULL);
    worksheet_write_url_opt(worksheet, CELL("A3"), "external:C:\\Temp\\foo.xlsx#Sheet1!A1",  NULL, NULL,       NULL);
    worksheet_write_url_opt(worksheet, CELL("A5"), "external:C:\\Temp\\foo.xlsx#Sheet1!A1",  NULL, "External", "Tip");

    return workbook_close(workbook);
}
