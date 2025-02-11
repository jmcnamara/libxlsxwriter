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

    lxw_workbook  *workbook  = workbook_new("test_cond_format06.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format = workbook_add_format(workbook);
    format_set_pattern (format, LXW_PATTERN_LIGHT_GRID);
    format_set_bg_color(format, 0xFFFF00);
    format_set_fg_color(format, 0xFF0000);

    worksheet_write_number(worksheet, CELL("A1"), 10 , NULL);
    worksheet_write_number(worksheet, CELL("A2"), 20 , NULL);
    worksheet_write_number(worksheet, CELL("A3"), 30 , NULL);
    worksheet_write_number(worksheet, CELL("A4"), 40 , NULL);

    lxw_conditional_format *conditional_format = calloc(1, sizeof(lxw_conditional_format));

    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_GREATER_THAN;
    conditional_format->value    = 7;
    conditional_format->format   = format;
    worksheet_conditional_format_cell(worksheet, CELL("A1"), conditional_format);

    free(conditional_format);
    return workbook_close(workbook);
}

