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

    lxw_workbook_options options = {.constant_memory = LXW_TRUE};

    lxw_workbook  *workbook  = workbook_new_opt("test_optimize13.xlsx", &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, CELL("A1"), "Foo", NULL);
    worksheet_write_comment(worksheet, CELL("B2"), "Some text");

    worksheet_set_comments_author(worksheet, "John");

    return workbook_close(workbook);
}
