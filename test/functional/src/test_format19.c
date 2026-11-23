/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_format19.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format1 = workbook_add_format(workbook);
    format_set_num_format(format1, "hh:mm;@");

    lxw_format *format2 = workbook_add_format(workbook);
    format_set_num_format(format2, "hh:mm;@");
    format_set_bg_color(format2, LXW_COLOR_YELLOW);

    worksheet_write_number(worksheet, 0, 0, 1, format1);
    worksheet_write_number(worksheet, 1, 0, 2, format2);

    return workbook_close(workbook);
}
