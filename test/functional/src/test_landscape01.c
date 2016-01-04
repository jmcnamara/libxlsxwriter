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

    lxw_workbook  *workbook  = workbook_new("test_landscape01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Foo" , NULL);
    worksheet_set_landscape(worksheet);
    worksheet_set_paper(worksheet, 9);
    worksheet->vertical_dpi = 200;

    return workbook_close(workbook);
}
