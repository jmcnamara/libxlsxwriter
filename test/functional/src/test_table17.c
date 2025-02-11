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

    lxw_workbook  *workbook  = workbook_new("test_table17.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("B:K"), 10.288, NULL);

    worksheet_write_string(worksheet, CELL("A1"), "Column1", NULL);
    worksheet_write_string(worksheet, CELL("B1"), "Column2", NULL);
    worksheet_write_string(worksheet, CELL("C1"), "Column3", NULL);
    worksheet_write_string(worksheet, CELL("D1"), "Column4", NULL);
    worksheet_write_string(worksheet, CELL("E1"), "Column5", NULL);
    worksheet_write_string(worksheet, CELL("F1"), "Column6", NULL);
    worksheet_write_string(worksheet, CELL("G1"), "Column7", NULL);
    worksheet_write_string(worksheet, CELL("H1"), "Column8", NULL);
    worksheet_write_string(worksheet, CELL("I1"), "Column9", NULL);
    worksheet_write_string(worksheet, CELL("J1"), "Column10", NULL);
    worksheet_write_string(worksheet, CELL("K1"), "Total",   NULL);

    worksheet_write_number(worksheet, 3, 1, 0, NULL);
    worksheet_write_number(worksheet, 3, 2, 0, NULL);
    worksheet_write_number(worksheet, 3, 3, 0, NULL);
    worksheet_write_number(worksheet, 3, 6, 4, NULL);
    worksheet_write_number(worksheet, 3, 7, 0, NULL);
    worksheet_write_number(worksheet, 3, 8, 1, NULL);
    worksheet_write_number(worksheet, 3, 9, 0, NULL);
    worksheet_write_number(worksheet, 3, 10, 0, NULL);

    worksheet_write_number(worksheet, 4, 1, 0, NULL);
    worksheet_write_number(worksheet, 4, 2, 0, NULL);
    worksheet_write_number(worksheet, 4, 3, 0, NULL);
    worksheet_write_number(worksheet, 4, 6, 5, NULL);
    worksheet_write_number(worksheet, 4, 7, 0, NULL);
    worksheet_write_number(worksheet, 4, 8, 2, NULL);
    worksheet_write_number(worksheet, 4, 9, 0, NULL);
    worksheet_write_number(worksheet, 4, 10, 0, NULL);


    lxw_table_column col1  = {.total_string = "Total"};
    lxw_table_column col2  = {0};
    lxw_table_column col3  = {.total_function = LXW_TABLE_FUNCTION_AVERAGE};
    lxw_table_column col4  = {.total_function = LXW_TABLE_FUNCTION_COUNT};
    lxw_table_column col5  = {.total_function = LXW_TABLE_FUNCTION_COUNT_NUMS};
    lxw_table_column col6  = {.total_function = LXW_TABLE_FUNCTION_MAX, .total_value = 5};
    lxw_table_column col7  = {.total_function = LXW_TABLE_FUNCTION_MIN};
    lxw_table_column col8  = {.total_function = LXW_TABLE_FUNCTION_SUM, .total_value = 3};
    lxw_table_column col9  = {.total_function = LXW_TABLE_FUNCTION_STD_DEV};
    lxw_table_column col10 = {.total_function = LXW_TABLE_FUNCTION_VAR};

    lxw_table_column *columns[] = {&col1, &col2, &col3, &col4, &col5, &col6, &col7, &col8, &col9, &col10, NULL};

    lxw_table_options options = {.total_row = LXW_TRUE, .columns = columns};


    worksheet_add_table(worksheet, RANGE("B3:K6"), &options);

    return workbook_close(workbook);
}
