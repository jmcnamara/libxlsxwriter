/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case for merged ranges.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_merge_range03.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format = workbook_add_format(workbook);
    format_set_align(format, LXW_ALIGN_CENTER);

    worksheet_merge_range(worksheet, 1, 1, 1, 2, "Foo", format);
    worksheet_merge_range(worksheet, 1, 3, 1, 4, "Foo", format);
    worksheet_merge_range(worksheet, 1, 5, 1, 6, "Foo", format);

    return workbook_close(workbook);
}
