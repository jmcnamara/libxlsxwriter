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

    lxw_workbook  *workbook  = workbook_new("test_table08.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("C:F"), 10.288, NULL);

    worksheet_write_string(worksheet, CELL("A1"), "Column1", NULL);
    worksheet_write_string(worksheet, CELL("B1"), "Column2", NULL);
    worksheet_write_string(worksheet, CELL("C1"), "Column3", NULL);
    worksheet_write_string(worksheet, CELL("D1"), "Column4", NULL);
    worksheet_write_string(worksheet, CELL("E1"), "Total",   NULL);

    lxw_table_column col1 = {.total_string = "Total"};
    lxw_table_column col2 = {0};
    lxw_table_column col3 = {0};
    lxw_table_column col4 = {.total_function = LXW_TABLE_FUNCTION_COUNT};
    lxw_table_column *columns[] = {&col1, &col2, &col3, &col4, NULL};

    lxw_table_options options = {.total_row = LXW_TRUE, .columns = columns};

    worksheet_add_table(worksheet, RANGE("C3:F14"), &options);

    return workbook_close(workbook);
}
