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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink29.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_format *format1 = workbook_add_format(workbook);
    lxw_format *format2 = workbook_add_format(workbook);

    format_set_hyperlink(format1);

    format_set_underline(format2, LXW_UNDERLINE_SINGLE);
    format_set_font_color(format2, LXW_COLOR_RED);

    worksheet_write_url(worksheet, CELL("A1"), "http://www.perl.org/", format1);
    worksheet_write_url(worksheet, CELL("A2"), "http://www.perl.com/", format2);

    return workbook_close(workbook);
}
