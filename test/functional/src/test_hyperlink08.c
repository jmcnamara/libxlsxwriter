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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink08.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_unset_default_url_format(workbook);

    /* Test with forward slashes instead of back slashes in test_hyperlink07.c. */
    worksheet_write_url_opt(worksheet, CELL("A1"), "external://VBOXSVR/share/foo.xlsx", NULL, "J:/foo.xlsx", NULL);
    worksheet_write_url(    worksheet, CELL("A3"), "external:foo.xlsx" , NULL);

    return workbook_close(workbook);
}
