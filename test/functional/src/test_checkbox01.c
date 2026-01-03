/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test case for checkbox functionality.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_checkbox01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_insert_checkbox(worksheet, 0, 0, LXW_FALSE);
    worksheet_insert_checkbox(worksheet, 1, 0, LXW_TRUE);
    worksheet_insert_checkbox(worksheet, 2, 0, LXW_FALSE);
    worksheet_insert_checkbox(worksheet, 3, 0, LXW_TRUE);
    worksheet_insert_checkbox(worksheet, 4, 0, LXW_TRUE);

    return workbook_close(workbook);
}
