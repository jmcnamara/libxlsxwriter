/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test case for writing data in optimization mode.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook_options options = {LXW_TRUE, NULL, LXW_FALSE, NULL, NULL};

    lxw_workbook  *workbook  = workbook_new_opt("test_optimize02.xlsx", &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
    worksheet_write_number(worksheet, 1, 0, 123,     NULL);

    /* G1 should be ignored since a later row has already been written. */
    worksheet_write_string(worksheet, CELL("G1"), "World", NULL);

    return workbook_close(workbook);
}
