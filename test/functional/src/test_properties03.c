/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_properties03.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_set_custom_property_string(workbook, "Checked by", "Adam");

    worksheet_set_column(worksheet, 0, 0, 70, NULL);
    worksheet_write_string(worksheet, CELL("A1"), "Select 'Office Button -> Prepare -> Properties' to see the file properties." , NULL);

    return workbook_close(workbook);
}
