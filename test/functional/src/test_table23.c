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

    lxw_workbook  *workbook  = workbook_new("test_table23.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("B:F"), 10.288, NULL);

    worksheet_write_string(worksheet, CELL("A1"), "Column1", NULL);
    worksheet_write_string(worksheet, CELL("F1"), "Total",   NULL);
    worksheet_write_string(worksheet, CELL("B1"), "Column'", NULL);
    worksheet_write_string(worksheet, CELL("C1"), "Column#", NULL);
    worksheet_write_string(worksheet, CELL("D1"), "Column[", NULL);
    worksheet_write_string(worksheet, CELL("E1"), "Column]", NULL);

    lxw_table_column col1  = {.header = "Column1", .total_string = "Total"};
    lxw_table_column col2  = {.header = "Column'", .total_function = LXW_TABLE_FUNCTION_SUM};
    lxw_table_column col3  = {.header = "Column#", .total_function = LXW_TABLE_FUNCTION_SUM};
    lxw_table_column col4  = {.header = "Column[", .total_function = LXW_TABLE_FUNCTION_SUM};
    lxw_table_column col5  = {.header = "Column]", .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column *columns[] = {&col1, &col2, &col3, &col4, &col5, NULL};
    lxw_table_options options = {.total_row = LXW_TRUE, .columns = columns};

    worksheet_add_table(worksheet, RANGE("B3:F9"), &options);

    return workbook_close(workbook);
}
