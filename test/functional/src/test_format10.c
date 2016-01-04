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

    lxw_workbook  *workbook  = workbook_new("test_format10.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format    *border1   = workbook_add_format(workbook);
    lxw_format    *border2   = workbook_add_format(workbook);
    lxw_format    *border3   = workbook_add_format(workbook);


    format_set_bg_color(border1, LXW_COLOR_RED);

    format_set_bg_color(border2, LXW_COLOR_YELLOW);
    format_set_pattern (border2, LXW_PATTERN_DARK_VERTICAL);

    format_set_bg_color(border3, LXW_COLOR_YELLOW);
    format_set_fg_color(border3, LXW_COLOR_RED);
    format_set_pattern (border3, LXW_PATTERN_GRAY_0625);

    worksheet_write_blank(worksheet, 1, 1, border1);
    worksheet_write_blank(worksheet, 3, 1, border2);
    worksheet_write_blank(worksheet, 5, 1, border3);

    return workbook_close(workbook);
}
