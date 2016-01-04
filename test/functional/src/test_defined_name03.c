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

    lxw_workbook  *workbook  = workbook_new("test_defined_name03.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "sheet One");

    workbook_define_name(workbook, "Sales", "='sheet One'!G1:H10");

    (void)worksheet;

    return workbook_close(workbook);
}
