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

    lxw_workbook  *workbook  = workbook_new("test_autofit01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Write a single character. */
    worksheet_write_string(worksheet, 0, 0, "A", NULL);

    /* Set column width (simulating what autofit would do after user override). */
    worksheet_set_column(worksheet, 0, 0, 1.57143, NULL);

    return workbook_close(workbook);
}
