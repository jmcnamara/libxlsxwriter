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

    lxw_workbook  *workbook  = workbook_new("test_cond_format12.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format = workbook_add_format(workbook);
    format_set_pattern (format, LXW_PATTERN_LIGHT_VERTICAL);
    format_set_bg_color(format, 0xFFFF00);
    format_set_fg_color(format, 0xFF0000);

    worksheet_write_string(worksheet, CELL("A1"), "Hello", format);

    worksheet_write_number(worksheet, CELL("B3"), 10 , NULL);
    worksheet_write_number(worksheet, CELL("B4"), 20 , NULL);
    worksheet_write_number(worksheet, CELL("B5"), 30 , NULL);
    worksheet_write_number(worksheet, CELL("B6"), 40 , NULL);

    lxw_conditional_format *conditional_format = calloc(1, sizeof(lxw_conditional_format));

    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_GREATER_THAN;
    conditional_format->value    = 20;
    conditional_format->format   = format;
    worksheet_conditional_format_range(worksheet, RANGE("B3:B6"), conditional_format);

    free(conditional_format);
    return workbook_close(workbook);
}
