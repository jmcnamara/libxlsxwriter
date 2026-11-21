/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_cond_format23.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format1 = workbook_add_format(workbook);
    format_set_bg_color(format1, 0xFF0000);

    lxw_format *format2 = workbook_add_format(workbook);
    format_set_bg_color(format2, 0x0000FF);

    lxw_format *format3 = workbook_add_format(workbook);
    format_set_bg_color(format3, 0x008000);

    int row;
    for (row = 0; row < 5; row++) {
        worksheet_write_number(worksheet, row, 0, 1, NULL);
        worksheet_write_number(worksheet, row, 1, row, NULL);
    }

    /* We manually set the indices to get the same order as the target file. */
    lxw_format_get_dxf_index(format2);
    lxw_format_get_dxf_index(format3);
    lxw_format_get_dxf_index(format1);

    lxw_conditional_format *conditional_format = calloc(1, sizeof(lxw_conditional_format));

    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_GREATER_THAN;
    conditional_format->value    = 5;
    conditional_format->format   = format1;
    worksheet_conditional_format_range(worksheet, RANGE("A1:A5"), conditional_format);

    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_GREATER_THAN;
    conditional_format->value    = 0;
    conditional_format->format   = format2;
    worksheet_conditional_format_range(worksheet, RANGE("A1:B5"), conditional_format);

    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_GREATER_THAN;
    conditional_format->value    = 0;
    conditional_format->format   = format3;
    worksheet_conditional_format_range(worksheet, RANGE("A1:A5"), conditional_format);

    free(conditional_format);
    return workbook_close(workbook);
}
