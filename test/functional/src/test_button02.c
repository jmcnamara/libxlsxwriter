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

    lxw_workbook  *workbook  = workbook_new("test_button02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_button_options options = {.x_offset = 4, .y_offset = 3, .caption = "my text"};

    worksheet_insert_button(worksheet, CELL("B4"), &options);

    return workbook_close(workbook);
}
