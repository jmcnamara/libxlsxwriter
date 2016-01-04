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

    lxw_workbook  *workbook  = workbook_new("test_format09.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format    *border1   = workbook_add_format(workbook);
    lxw_format    *border2   = workbook_add_format(workbook);
    lxw_format    *border3   = workbook_add_format(workbook);
    lxw_format    *border4   = workbook_add_format(workbook);


    format_set_border      (border1, LXW_BORDER_HAIR);
    format_set_border_color(border1, LXW_COLOR_RED);

    format_set_diag_type (border2, LXW_DIAGONAL_BORDER_UP);
    format_set_diag_color(border2, LXW_COLOR_RED);

    format_set_diag_type (border3, LXW_DIAGONAL_BORDER_DOWN);
    format_set_diag_color(border3, LXW_COLOR_RED);

    format_set_diag_type (border4, LXW_DIAGONAL_BORDER_UP_DOWN);
    format_set_diag_color(border4, LXW_COLOR_RED);

    worksheet_write_blank(worksheet, 1, 1, border1);
    worksheet_write_blank(worksheet, 3, 1, border2);
    worksheet_write_blank(worksheet, 5, 1, border3);
    worksheet_write_blank(worksheet, 7, 1, border4);

    return workbook_close(workbook);
}
