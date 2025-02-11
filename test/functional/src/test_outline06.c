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

    lxw_workbook  *workbook  = workbook_new("test_outline06.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, "Outlined Rows");

    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    lxw_row_col_options options1 = {.hidden = 0, .level = 2, .collapsed = 0};
    lxw_row_col_options options2 = {.hidden = 0, .level = 1, .collapsed = 0};


    worksheet_outline_settings(worksheet1, LXW_FALSE, LXW_FALSE, LXW_FALSE, LXW_TRUE);

    worksheet_set_column(worksheet1, COLS("A:A"), 20, NULL);

    worksheet_set_row_opt(worksheet1, 1,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 2,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 3,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 4,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 5,  LXW_DEF_ROW_HEIGHT, NULL, &options2);

    worksheet_set_row_opt(worksheet1, 6,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 7,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 8,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 9,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 10, LXW_DEF_ROW_HEIGHT, NULL, &options2);

    worksheet_write_string(worksheet1, CELL("A1"), "Region", bold);
    worksheet_write_string(worksheet1, CELL("A2"), "North" , NULL);
    worksheet_write_string(worksheet1, CELL("A3"), "North" , NULL);
    worksheet_write_string(worksheet1, CELL("A4"), "North" , NULL);
    worksheet_write_string(worksheet1, CELL("A5"), "North" , NULL);
    worksheet_write_string(worksheet1, CELL("A6"), "North Total", bold);

    worksheet_write_string(worksheet1, CELL("B1"), "Sales", bold);
    worksheet_write_number(worksheet1, CELL("B2"), 1000 , NULL);
    worksheet_write_number(worksheet1, CELL("B3"), 1200 , NULL);
    worksheet_write_number(worksheet1, CELL("B4"), 900 , NULL);
    worksheet_write_number(worksheet1, CELL("B5"), 1200 , NULL);
    worksheet_write_formula_num(worksheet1, CELL("B6"), "=SUBTOTAL(9,B2:B5)", bold, 4300);

    worksheet_write_string(worksheet1, CELL("A7"), "South" , NULL);
    worksheet_write_string(worksheet1, CELL("A8"), "South" , NULL);
    worksheet_write_string(worksheet1, CELL("A9"), "South" , NULL);
    worksheet_write_string(worksheet1, CELL("A10"), "South" , NULL);
    worksheet_write_string(worksheet1, CELL("A11"), "South Total", bold);

    worksheet_write_number(worksheet1, CELL("B7"), 400 , NULL);
    worksheet_write_number(worksheet1, CELL("B8"), 600 , NULL);
    worksheet_write_number(worksheet1, CELL("B9"), 500 , NULL);
    worksheet_write_number(worksheet1, CELL("B10"), 600 , NULL);
    worksheet_write_formula_num(worksheet1, CELL("B11"), "=SUBTOTAL(9,B7:B10)", bold, 2100);

    worksheet_write_string(worksheet1, CELL("A12"), "Grand Total", bold);
    worksheet_write_formula_num(worksheet1, CELL("B12"), "=SUBTOTAL(9,B2:B10)", bold, 6400);

    return workbook_close(workbook);
}
