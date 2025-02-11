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

    lxw_workbook  *workbook  = workbook_new("test_data_validation07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    const char* list[] = {"coffee", "café", NULL};

    lxw_data_validation *data_validation = calloc(1, sizeof(lxw_data_validation));
    data_validation->validate = LXW_VALIDATION_TYPE_LIST;
    data_validation->value_list = list;

    worksheet_data_validation_cell(worksheet, CELL("C2"), data_validation);

    free(data_validation);

    return workbook_close(workbook);
}
