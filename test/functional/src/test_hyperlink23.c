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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink23.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_unset_default_url_format(workbook);

    worksheet_write_url_opt(worksheet, CELL("A1"), "https://en.wikipedia.org/wiki/Microsoft_Excel#Data_storage_and_communication", NULL, "Display text", NULL);

    return workbook_close(workbook);
}
