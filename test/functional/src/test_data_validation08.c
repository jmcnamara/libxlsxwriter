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

    lxw_workbook  *workbook  = workbook_new("test_data_validation08.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_data_validation *data_validation = calloc(1, sizeof(lxw_data_validation));
    data_validation->validate = LXW_VALIDATION_TYPE_ANY;
    data_validation->input_title = "This is the input title";
    data_validation->input_message = "This is the input message";

    worksheet_data_validation_cell(worksheet, CELL("C2"), data_validation);

    free(data_validation);

    return workbook_close(workbook);
}
