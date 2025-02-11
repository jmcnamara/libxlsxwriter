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

    lxw_workbook  *workbook  = workbook_new("test_format15.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format1 = workbook_add_format(workbook);
    lxw_format *format2 = workbook_add_format(workbook);

    format_set_bold(format1);
    format_set_bold(format2);
    format_set_num_format_index(format2, 1);

    worksheet_write_number(worksheet, CELL("A1"), 1, format1);
    worksheet_write_number(worksheet, CELL("A2"), 2, format2);

    return workbook_close(workbook);
}
