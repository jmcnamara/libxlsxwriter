/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case for TODO.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_format12.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *top_left_bottom = workbook_add_format(workbook);
    format_set_bottom(top_left_bottom, LXW_BORDER_THIN);
    format_set_left(top_left_bottom, LXW_BORDER_THIN);
    format_set_top(top_left_bottom, LXW_BORDER_THIN);

    lxw_format *top_bottom = workbook_add_format(workbook);
    format_set_bottom(top_bottom, LXW_BORDER_THIN);
    format_set_top(top_bottom, LXW_BORDER_THIN);

    lxw_format *top_left = workbook_add_format(workbook);
    format_set_left(top_left, LXW_BORDER_THIN);
    format_set_top(top_left, LXW_BORDER_THIN);

    lxw_format *unused = workbook_add_format(workbook);
    format_set_left(unused, LXW_BORDER_THIN);

    worksheet_write_string(worksheet, CELL("B2"), "test", top_left_bottom);
    worksheet_write_string(worksheet, CELL("D2"), "test", top_left);
    worksheet_write_string(worksheet, CELL("F2"), "test", top_bottom);

    return workbook_close(workbook);
}
