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

    lxw_workbook  *workbook  = workbook_new("test_print_options07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Foo" , NULL);

    worksheet_set_paper(worksheet, 9);
    worksheet->vertical_dpi = 200;

    worksheet_print_black_and_white(worksheet);

    return workbook_close(workbook);
}
