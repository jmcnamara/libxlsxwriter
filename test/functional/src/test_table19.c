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

    lxw_workbook  *workbook  = workbook_new("test_table19.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("C:F"), 10.288, NULL);

    lxw_table_column col1  = {0};
    lxw_table_column col2  = {0};
    lxw_table_column col3  = {0};
    lxw_table_column col4  = {.header = " Column4 "};

    lxw_table_column *columns[] = {&col1, &col2, &col3, &col4, NULL};
    lxw_table_options options = {.columns = columns};


    worksheet_add_table(worksheet, RANGE("C3:F13"), &options);

    return workbook_close(workbook);
}
