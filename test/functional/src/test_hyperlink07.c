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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_unset_default_url_format(workbook);

    worksheet_write_url_opt(worksheet, CELL("A1"), "external:\\\\VBOXSVR\\share\\foo.xlsx", NULL, "J:\\foo.xlsx", NULL);
    worksheet_write_url(    worksheet, CELL("A3"), "external:foo.xlsx" , NULL);

    return workbook_close(workbook);
}
