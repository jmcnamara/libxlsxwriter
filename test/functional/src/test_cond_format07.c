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

    lxw_workbook  *workbook  = workbook_new("test_cond_format07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format1 = workbook_add_format(workbook);
    format_set_bg_color(format1, 0xFF0000);

    lxw_format *format2 = workbook_add_format(workbook);
    format_set_bg_color(format2, 0x92D050);

    uint8_t data[10][10] = {
            { 90, 80,  50, 10,  20,  90,  40, 90,  30,  40 },
            { 20, 10,  90, 100, 30,  60,  70, 60,  50,  90 },
            { 10, 50,  60, 50,  20,  50,  80, 30,  40,  60 },
            { 10, 90,  20, 40,  10,  40,  50, 70,  90,  50 },
            { 70, 100, 10, 90,  10,  10,  20, 100, 100, 40 },
            { 20, 60,  10, 100, 30,  10,  20, 60,  100, 10 },
            { 10, 60,  10, 80,  100, 80,  30, 30,  70,  40 },
            { 30, 90,  60, 10,  10,  100, 40, 40,  30,  40 },
            { 80, 90,  10, 20,  20,  50,  80, 20,  60,  90 },
            { 60, 80,  30, 30,  10,  50,  80, 60,  50,  30 },
    };

    int row, col;
    for (row = 0; row < 10; row++)
        for (col = 0; col < 10; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);

    /* We manually set the indices to get the same order as the target file. */
    lxw_format_get_dxf_index(format2);
    lxw_format_get_dxf_index(format1);

    lxw_conditional_format *conditional_format = calloc(1, sizeof(lxw_conditional_format));

    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_GREATER_THAN_OR_EQUAL_TO;
    conditional_format->value    = 50;
    conditional_format->format   = format1;
    worksheet_conditional_format_range(worksheet, RANGE("A1:J10"), conditional_format);

    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_LESS_THAN;
    conditional_format->value    = 50;
    conditional_format->format   = format2;
    worksheet_conditional_format_range(worksheet, RANGE("A1:J10"), conditional_format);

    free(conditional_format);
    return workbook_close(workbook);
}
