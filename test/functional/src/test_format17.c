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

    lxw_workbook  *workbook  = workbook_new("test_format17.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *pattern = workbook_add_format(workbook);
    format_set_pattern(pattern, LXW_PATTERN_MEDIUM_GRAY);
    format_set_fg_color(pattern, LXW_COLOR_RED);

    worksheet_write_string(worksheet, CELL("A1"), "", pattern);

    return workbook_close(workbook);
}
