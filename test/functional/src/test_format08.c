/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case to test data writing.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_format08.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format    *border1   = workbook_add_format(workbook);
    lxw_format    *border2   = workbook_add_format(workbook);
    lxw_format    *border3   = workbook_add_format(workbook);
    lxw_format    *border4   = workbook_add_format(workbook);
    lxw_format    *border5   = workbook_add_format(workbook);


    format_set_bottom(border1, LXW_BORDER_THIN);
    format_set_bottom_color(border1, LXW_COLOR_RED);

    format_set_top(border2, LXW_BORDER_THIN);
    format_set_top_color(border2, LXW_COLOR_RED);

    format_set_left(border3, LXW_BORDER_THIN);
    format_set_left_color(border3, LXW_COLOR_RED);

    format_set_right(border4, LXW_BORDER_THIN);
    format_set_right_color(border4, LXW_COLOR_RED);

    format_set_border(border5, LXW_BORDER_THIN);
    format_set_border_color(border5, LXW_COLOR_RED);

    worksheet_write_blank(worksheet, 1, 1, border1);
    worksheet_write_blank(worksheet, 3, 1, border2);
    worksheet_write_blank(worksheet, 5, 1, border3);
    worksheet_write_blank(worksheet, 7, 1, border4);
    worksheet_write_blank(worksheet, 9, 1, border5);

    return workbook_close(workbook);
}
