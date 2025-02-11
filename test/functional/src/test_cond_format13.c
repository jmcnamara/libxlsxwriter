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

    lxw_workbook  *workbook  = workbook_new("test_cond_format13.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format1 = workbook_add_format(workbook);
    format_set_num_format_index(format1, 2);

    lxw_format *format2 = workbook_add_format(workbook);
    format_set_num_format(format2, "0.000");

    /* We manually set the indices to get the same order as the target file. */
    lxw_format_get_dxf_index(format2);
    lxw_format_get_dxf_index(format1);

    worksheet_write_number(worksheet, CELL("A1"), 10 , NULL);
    worksheet_write_number(worksheet, CELL("A2"), 20 , NULL);
    worksheet_write_number(worksheet, CELL("A3"), 30 , NULL);
    worksheet_write_number(worksheet, CELL("A4"), 40 , NULL);

    lxw_conditional_format *conditional_format = calloc(1, sizeof(lxw_conditional_format));

    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_GREATER_THAN;
    conditional_format->value    = 2;
    conditional_format->format   = format1;
    worksheet_conditional_format_cell(worksheet, CELL("A1"), conditional_format);

    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_LESS_THAN;
    conditional_format->value    = 8;
    conditional_format->format   = format2;
    worksheet_conditional_format_cell(worksheet, CELL("A2"), conditional_format);

    free(conditional_format);
    return workbook_close(workbook);
}
