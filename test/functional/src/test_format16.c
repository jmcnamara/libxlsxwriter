/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_format16.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *pattern = workbook_add_format(workbook);
    format_set_pattern(pattern, LXW_PATTERN_MEDIUM_GRAY);

    worksheet_write_string(worksheet, CELL("A1"), "", pattern);

    return workbook_close(workbook);
}
