/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_hyperlink20.xlsx");

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_format *format1 = workbook_add_format(workbook);
    lxw_format *format2 = workbook_add_format(workbook);

    format_set_underline(format1, LXW_UNDERLINE_SINGLE);
    format_set_font_color(format1, LXW_COLOR_BLUE);

    format_set_underline(format2, LXW_UNDERLINE_SINGLE);
    format_set_font_color(format2, LXW_COLOR_RED);


    worksheet_write_url(worksheet, CELL("A1"), "http://www.python.org/1", format1);
    worksheet_write_url(worksheet, CELL("A2"), "http://www.python.org/2", format2);

    return workbook_close(workbook);
}
