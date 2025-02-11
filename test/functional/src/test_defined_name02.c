/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case for defined names.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_defined_name02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "sheet One");

    workbook_define_name(workbook, "Sales", "='sheet One'!$G$1:$H$10");

    (void)worksheet;

    return workbook_close(workbook);
}
