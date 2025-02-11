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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_unset_default_url_format(workbook);

    worksheet_write_url(worksheet, CELL("A1"),  "http://www.perl.org/", NULL);
    worksheet_write_url(worksheet, CELL("D4"),  "http://www.perl.org/", NULL);
    worksheet_write_url(worksheet, CELL("A8"),  "http://www.perl.org/", NULL);
    worksheet_write_url(worksheet, CELL("B6"),  "http://www.cpan.org/", NULL);
    worksheet_write_url(worksheet, CELL("F12"), "http://www.cpan.org/", NULL);

    return workbook_close(workbook);
}
