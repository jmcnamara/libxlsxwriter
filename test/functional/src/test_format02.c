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

    lxw_workbook  *workbook  = workbook_new("test_format02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format    *format1    = workbook_add_format(workbook);
    lxw_format    *format2    = workbook_add_format(workbook);

    worksheet_set_row(worksheet, 0, 30, NULL);

    format_set_font_name(format1, "Arial");
    format_set_bold(format1);
    format_set_align(format1, LXW_ALIGN_LEFT);
    format_set_align(format1, LXW_ALIGN_VERTICAL_BOTTOM);

    format_set_font_name(format2, "Arial");
    format_set_bold(format2);
    format_set_rotation(format2, 90);
    format_set_align(format2, LXW_ALIGN_CENTER);
    format_set_align(format2, LXW_ALIGN_VERTICAL_BOTTOM);

    worksheet_write_string(worksheet, 0, 0, "Foo", format1);
    worksheet_write_string(worksheet, 0, 1, "Bar", format2);

    return workbook_close(workbook);
}
