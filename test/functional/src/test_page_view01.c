/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case fort set_page_view().
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_page_view01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_page_view(worksheet);

    worksheet_write_string(worksheet, CELL("A1"), "Foo" , NULL);

    worksheet_set_paper(worksheet, 9);
    worksheet->vertical_dpi = 200;

    return workbook_close(workbook);
}
