/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test case for textbox functionality.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_textbox01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_insert_textbox(worksheet, 8, 4, "This is some text");

    return workbook_close(workbook);
}
