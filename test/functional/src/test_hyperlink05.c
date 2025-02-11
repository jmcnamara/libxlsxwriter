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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink05.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_unset_default_url_format(workbook);

    worksheet_write_url(    worksheet, CELL("A1"), "http://www.perl.org/", NULL);
    worksheet_write_url_opt(worksheet, CELL("A3"), "http://www.perl.org/", NULL, "Perl home", NULL);
    worksheet_write_url_opt(worksheet, CELL("A5"), "http://www.perl.org/", NULL, "Perl home", "Tool Tip");
    worksheet_write_url_opt(worksheet, CELL("A7"), "http://www.cpan.org/", NULL, "CPAN",      "Download");

    return workbook_close(workbook);
}
