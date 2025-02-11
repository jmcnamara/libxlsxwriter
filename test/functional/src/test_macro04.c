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

    lxw_workbook  *workbook  = workbook_new("test_macro04.xlsm");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "Foo");

    workbook_add_signed_vba_project(workbook,
                                    "images/vbaProject05.bin",
                                    "images/vbaProject05Signature.bin");

    worksheet_set_column(worksheet, COLS("A:A"), 30, NULL);
    worksheet_write_string(worksheet, 2, 0, "Press the button to say hello.", NULL);

    lxw_button_options options = { .caption = "Press Me", .macro = "say_hello",
                                  .width = 80, .height = 30 };

    worksheet_insert_button(worksheet, 2, 1, &options);

    return workbook_close(workbook);
}
