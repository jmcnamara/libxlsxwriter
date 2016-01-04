/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case for defined names.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_defined_name01.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, "Sheet 3");

    worksheet_set_paper(worksheet1, 9);
    worksheet1->vertical_dpi = 200;

    worksheet_print_area(worksheet1, RANGE("A1:E6"));
    worksheet_autofilter(worksheet1, RANGE("F1:G1"));
    worksheet_write_string(worksheet1, CELL("G1"), "Filter", NULL);
    worksheet_write_string(worksheet1, CELL("F1"), "Auto", NULL);
    worksheet_fit_to_pages(worksheet1, 2, 2);

    workbook_define_name(workbook, "'Sheet 3'!Bar", "='Sheet 3'!$A$1");
    workbook_define_name(workbook, "Abc",           "=Sheet1!$A$1");
    workbook_define_name(workbook, "Baz",           "=0.98");
    workbook_define_name(workbook, "Sheet1!Bar",    "=Sheet1!$A$1");
    workbook_define_name(workbook, "Sheet2!Bar",    "=Sheet2!$A$1");
    workbook_define_name(workbook, "Sheet2!aaa",    "=Sheet2!$A$1");
    workbook_define_name(workbook, "_Egg",          "=Sheet1!$A$1");
    workbook_define_name(workbook, "_Fog",          "=Sheet1!$A$1");

    (void)worksheet2;
    (void)worksheet3;

    return workbook_close(workbook);
}
