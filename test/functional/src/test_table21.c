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

    lxw_workbook  *workbook  = workbook_new("test_table21.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("C:D"), 10.288, NULL);

    worksheet_write_string(worksheet, CELL("A1"), "Column", NULL);

    lxw_table_column col1 = {.header = "Column"};
    lxw_table_column *columns[] = {&col1, NULL};

    lxw_table_options options = {.columns = columns};

    worksheet_add_table(worksheet, RANGE("C3:D13"), &options);

    return workbook_close(workbook);
}
