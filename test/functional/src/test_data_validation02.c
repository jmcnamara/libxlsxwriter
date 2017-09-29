/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_data_validation02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    char *list[] = {"Foo", "Bar", "Baz", NULL};

    lxw_data_validation *data_validation = calloc(1, sizeof(lxw_data_validation));
    data_validation->validate = LXW_VALIDATION_TYPE_LIST;
    data_validation->value_list = list;
    data_validation->input_title = "This is the input title";
    data_validation->input_message = "This is the input message";

    worksheet_data_validation_cell(worksheet, CELL("C2"), data_validation);

    free(data_validation);

    return workbook_close(workbook);
}
