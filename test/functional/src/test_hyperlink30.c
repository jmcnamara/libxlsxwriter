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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink30.xlsx");

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_format *format1 = workbook_add_format(workbook);
    lxw_format *format2 = workbook_add_format(workbook);
    lxw_format *format3 = workbook_add_format(workbook);

    workbook_unset_default_url_format(workbook);

    format_set_hyperlink(format1);

    format_set_underline(format2, LXW_UNDERLINE_SINGLE);
    format_set_font_color(format2, LXW_COLOR_RED);

    format_set_font_color(format3, LXW_COLOR_BLUE);
    format_set_underline(format3, LXW_UNDERLINE_SINGLE);

    worksheet_write_url(worksheet, CELL("A1"), "http://www.python.org/1", format1);
    worksheet_write_url(worksheet, CELL("A2"), "http://www.python.org/2", format2);
    worksheet_write_url(worksheet, CELL("A3"), "http://www.python.org/3", format3);

    return workbook_close(workbook);
}
