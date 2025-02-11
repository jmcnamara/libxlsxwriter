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

    lxw_workbook  *workbook  = workbook_new("test_cond_format17.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_number(worksheet, CELL("A1"), 10 , NULL);
    worksheet_write_number(worksheet, CELL("A2"), 20 , NULL);
    worksheet_write_number(worksheet, CELL("A3"), 30 , NULL);
    worksheet_write_number(worksheet, CELL("A4"), 40 , NULL);

    lxw_conditional_format *conditional_format = calloc(1, sizeof(lxw_conditional_format));

    conditional_format->type          = LXW_CONDITIONAL_TYPE_ICON_SETS;
    conditional_format->icon_style    = LXW_CONDITIONAL_ICONS_3_ARROWS_COLORED;
    worksheet_conditional_format_range(worksheet, RANGE("A1:A4"), conditional_format);

    free(conditional_format);
    return workbook_close(workbook);
}
