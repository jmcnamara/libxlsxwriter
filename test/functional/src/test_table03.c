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

    lxw_workbook  *workbook  = workbook_new("test_table03.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("C:F"), 10.288, NULL);

    worksheet_add_table(worksheet, RANGE("C3:F13"), NULL);

    /* Add other objects to check rId handling. */
    workbook_unset_default_url_format(workbook);
    worksheet_write_url(worksheet, CELL("A1"), "http://perl.com/", NULL);

    return workbook_close(workbook);
}
