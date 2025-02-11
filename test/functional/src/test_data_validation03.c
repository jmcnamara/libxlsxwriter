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

    lxw_workbook  *workbook  = workbook_new("test_data_validation03.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    const char *list1[] = {"Foo", "Bar", "Baz", NULL};
    const char *list2[] = {
        "Foobar", "Foobas", "Foobat", "Foobau", "Foobav", "Foobaw", "Foobax",
        "Foobay", "Foobaz", "Foobba", "Foobbb", "Foobbc", "Foobbd", "Foobbe",
        "Foobbf", "Foobbg", "Foobbh", "Foobbi", "Foobbj", "Foobbk", "Foobbl",
        "Foobbm", "Foobbn", "Foobbo", "Foobbp", "Foobbq", "Foobbr", "Foobbs",
        "Foobbt", "Foobbu", "Foobbv", "Foobbw", "Foobbx", "Foobby", "Foobbz",
        "Foobca", "End", NULL};

    lxw_data_validation *data_validation = calloc(1, sizeof(lxw_data_validation));

    data_validation->validate = LXW_VALIDATION_TYPE_LIST;
    data_validation->value_list = list1;
    data_validation->input_title = "This is the input title";
    data_validation->input_message = "This is the input message";
    worksheet_data_validation_cell(worksheet, CELL("C2"), data_validation);

    data_validation->validate = LXW_VALIDATION_TYPE_LIST;
    data_validation->value_list = list2;
    data_validation->input_title = "This is the longest input title1";
    data_validation->input_message = "This is the longest input message aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    worksheet_data_validation_cell(worksheet, CELL("D6"), data_validation);

    free(data_validation);

    return workbook_close(workbook);
}
