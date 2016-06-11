/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_properties05.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_set_custom_property_string  (workbook, "Location", "Café");

    worksheet_set_column(worksheet, 0, 0, 70, NULL);
    worksheet_write_string(worksheet, CELL("A1"), "Select 'Office Button -> Prepare -> Properties' to see the file properties." , NULL);

    return workbook_close(workbook);
}
