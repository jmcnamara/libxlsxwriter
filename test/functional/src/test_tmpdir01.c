/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case to test data writing.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook_options options = {LXW_FALSE, ".", LXW_FALSE, NULL, NULL};

    lxw_workbook  *workbook  = workbook_new_opt("test_tmpdir01.xlsx", &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
    worksheet_write_number(worksheet, 1, 0, 123,     NULL);

    return workbook_close(workbook);
}
