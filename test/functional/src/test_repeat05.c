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

    lxw_workbook  *workbook   = workbook_new("test_repeat05.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);

    worksheet_set_paper(worksheet1, 9);
    worksheet1->vertical_dpi = 200;

    worksheet_set_paper(worksheet3, 9);
    worksheet3->vertical_dpi = 200;

    (void) worksheet2;

    worksheet_repeat_rows(worksheet1, 0, 0);
    worksheet_repeat_rows(worksheet3, 2, 3);
    worksheet_repeat_columns(worksheet3, 1, 5);

    worksheet_write_string(worksheet1, CELL("A1"), "Foo" , NULL);

    return workbook_close(workbook);
}
