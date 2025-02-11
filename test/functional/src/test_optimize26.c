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

    /* Use deprecated constructor for testing. */
    lxw_workbook  *workbook  = workbook_new_opt("test_optimize26.xlsx", &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 2, 2, "café", NULL);

    return workbook_close(workbook);
}
