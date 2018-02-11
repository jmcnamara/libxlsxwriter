/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_data_validation01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    char *list[] = {"Foo", "Bar", "Baz", NULL};

    lxw_data_validation *data_validation = calloc(1, sizeof(lxw_data_validation));
    data_validation->validate = LXW_VALIDATION_TYPE_LIST;
    data_validation->value_list = list;

    worksheet_data_validation_cell(worksheet, CELL("C2"), data_validation);

    free(data_validation);

    return workbook_close(workbook);
}
