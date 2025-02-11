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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink44.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "Sheet 1");

    lxw_image_options options = {.url = "internal:'Sheet 1'!A1"};

    worksheet_insert_image_opt(worksheet, CELL("E9"), "images/red.png", &options);

    return workbook_close(workbook);
}
