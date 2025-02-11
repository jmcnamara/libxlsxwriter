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

    lxw_workbook  *workbook  = workbook_new("test_escapes08.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_unset_default_url_format(workbook);

    /* Test an already escaped string. */
    worksheet_write_url_opt(worksheet, CELL("A1"), "http://example.com/%5b0%5d", NULL, "http://example.com/[0]", NULL);

    return workbook_close(workbook);
}
