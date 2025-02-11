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

    lxw_workbook  *workbook  = workbook_new("test_table26.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("C:D"), 10.288, NULL);
    worksheet_set_column(worksheet, COLS("F:G"), 10.288, NULL);

    /* Add some valid tables. */
    worksheet_add_table(worksheet, RANGE("C2:D3"), NULL);
    lxw_table_options options1 = {.name = "Table2", .no_header_row = LXW_TRUE};
    worksheet_add_table(worksheet, RANGE("F3:G3"), &options1);

    /*
     * Test incorrect tables. These should be ignored with a warning.
     * */

    /* Add a table with an incorrect range. */
    worksheet_add_table(worksheet, RANGE("I2:J2"), NULL);

    /* Check incorrect table names. */
    lxw_table_options options2 = {.name = "Has space"};
    worksheet_add_table(worksheet, RANGE("F3:G33"), &options2);

    lxw_table_options options3 = {.name = "Table["};
    worksheet_add_table(worksheet, RANGE("F3:G33"), &options3);

    lxw_table_options options4 = {.name = "This_is_a_long_table_name_that_exceeds_a_limit_of_255_characters_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"};
    worksheet_add_table(worksheet, RANGE("F3:G33"), &options4);

    lxw_table_options options5 = {.name = "c"};
    worksheet_add_table(worksheet, RANGE("F3:G33"), &options5);

    lxw_table_options options6 = {.name = "R"};
    worksheet_add_table(worksheet, RANGE("F3:G33"), &options6);

    lxw_table_options options7 = {.name = ".Table"};
    worksheet_add_table(worksheet, RANGE("F3:G33"), &options7);

    lxw_table_options options8 = {.name = "1Table"};
    worksheet_add_table(worksheet, RANGE("F3:G33"), &options8);


    return workbook_close(workbook);
}
