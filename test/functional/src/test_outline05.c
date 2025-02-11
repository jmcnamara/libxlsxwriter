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

    lxw_workbook  *workbook  = workbook_new("test_outline05.xlsx");
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, "Collapsed Rows");

    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    lxw_row_col_options options3 = {.hidden = 1, .level = 2, .collapsed = 0};
    lxw_row_col_options options4 = {.hidden = 1, .level = 1, .collapsed = 1};
    lxw_row_col_options options5 = {.hidden = 0, .level = 0, .collapsed = 1};

    worksheet_set_column(worksheet2, COLS("A:A"), 20, NULL);
    worksheet_set_selection(worksheet2, RANGE("A14:A14"));

    worksheet_set_row_opt(worksheet2, 1,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 2,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 3,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 4,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 5,  LXW_DEF_ROW_HEIGHT, NULL, &options4);

    worksheet_set_row_opt(worksheet2, 6,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 7,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 8,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 9,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 10, LXW_DEF_ROW_HEIGHT, NULL, &options4);
    worksheet_set_row_opt(worksheet2, 11, LXW_DEF_ROW_HEIGHT, NULL, &options5);

    worksheet_write_string(worksheet2, CELL("A1"), "Region", bold);
    worksheet_write_string(worksheet2, CELL("A2"), "North" , NULL);
    worksheet_write_string(worksheet2, CELL("A3"), "North" , NULL);
    worksheet_write_string(worksheet2, CELL("A4"), "North" , NULL);
    worksheet_write_string(worksheet2, CELL("A5"), "North" , NULL);
    worksheet_write_string(worksheet2, CELL("A6"), "North Total", bold);

    worksheet_write_string(worksheet2, CELL("B1"), "Sales", bold);
    worksheet_write_number(worksheet2, CELL("B2"), 1000 , NULL);
    worksheet_write_number(worksheet2, CELL("B3"), 1200 , NULL);
    worksheet_write_number(worksheet2, CELL("B4"), 900 , NULL);
    worksheet_write_number(worksheet2, CELL("B5"), 1200 , NULL);
    worksheet_write_formula_num(worksheet2, CELL("B6"), "=SUBTOTAL(9,B2:B5)", bold, 4300);

    worksheet_write_string(worksheet2, CELL("A7"), "South" , NULL);
    worksheet_write_string(worksheet2, CELL("A8"), "South" , NULL);
    worksheet_write_string(worksheet2, CELL("A9"), "South" , NULL);
    worksheet_write_string(worksheet2, CELL("A10"), "South" , NULL);
    worksheet_write_string(worksheet2, CELL("A11"), "South Total", bold);


    worksheet_write_number(worksheet2, CELL("B7"), 400 , NULL);
    worksheet_write_number(worksheet2, CELL("B8"), 600 , NULL);
    worksheet_write_number(worksheet2, CELL("B9"), 500 , NULL);
    worksheet_write_number(worksheet2, CELL("B10"), 600 , NULL);
    worksheet_write_formula_num(worksheet2, CELL("B11"), "=SUBTOTAL(9,B7:B10)", bold, 2100);

    worksheet_write_string(worksheet2, CELL("A12"), "Grand Total", bold);
    worksheet_write_formula_num(worksheet2, CELL("B12"), "=SUBTOTAL(9,B2:B10)", bold, 6400);

    return workbook_close(workbook);
}
