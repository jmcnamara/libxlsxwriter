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

    lxw_workbook  *workbook  = new_workbook("test_properties04.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_datetime   datetime  = {2016, 12, 12,  23, 0, 0};

    workbook_set_custom_property_string  (workbook, "Checked by",     "Adam");
    workbook_set_custom_property_datetime(workbook, "Date completed",  &datetime);
    workbook_set_custom_property_integer (workbook, "Document number", 12345);
    workbook_set_custom_property_number  (workbook, "Reference",       1.2345);
    workbook_set_custom_property_boolean (workbook, "Source",          1);
    workbook_set_custom_property_boolean (workbook, "Status",          0);
    workbook_set_custom_property_string  (workbook, "Department",      "Finance");
    workbook_set_custom_property_number  (workbook, "Group",           1.2345678901234);

    worksheet_set_column(worksheet, 0, 0, 70, NULL);
    worksheet_write_string(worksheet, CELL("A1"), "Select 'Office Button -> Prepare -> Properties' to see the file properties." , NULL);

    return workbook_close(workbook);
}
