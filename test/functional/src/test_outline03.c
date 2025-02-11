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

    lxw_workbook  *workbook  = workbook_new("test_outline03.xlsx");
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, "Outline Columns");

    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    lxw_row_col_options options6 = {.hidden = 0, .level = 1, .collapsed = 0};

    worksheet_write_string(worksheet3, CELL("A1"), "Month", NULL);
    worksheet_write_string(worksheet3, CELL("B1"), "Jan",   NULL);
    worksheet_write_string(worksheet3, CELL("C1"), "Feb",   NULL);
    worksheet_write_string(worksheet3, CELL("D1"), "Mar",   NULL);
    worksheet_write_string(worksheet3, CELL("E1"), "Apr",   NULL);
    worksheet_write_string(worksheet3, CELL("F1"), "May",   NULL);
    worksheet_write_string(worksheet3, CELL("G1"), "Jun",   NULL);
    worksheet_write_string(worksheet3, CELL("H1"), "Total", NULL);

    worksheet_write_string(worksheet3, CELL("A2"), "North", NULL);
    worksheet_write_number(worksheet3, CELL("B2"), 50,      NULL);
    worksheet_write_number(worksheet3, CELL("C2"), 20,      NULL);
    worksheet_write_number(worksheet3, CELL("D2"), 15,      NULL);
    worksheet_write_number(worksheet3, CELL("E2"), 25,      NULL);
    worksheet_write_number(worksheet3, CELL("F2"), 65,      NULL);
    worksheet_write_number(worksheet3, CELL("G2"), 80,      NULL);
    worksheet_write_formula_num(worksheet3, CELL("H2"), "=SUM(B2:G2)", NULL, 255);

    worksheet_write_string(worksheet3, CELL("A3"), "South", NULL);
    worksheet_write_number(worksheet3, CELL("B3"), 10,      NULL);
    worksheet_write_number(worksheet3, CELL("C3"), 20,      NULL);
    worksheet_write_number(worksheet3, CELL("D3"), 30,      NULL);
    worksheet_write_number(worksheet3, CELL("E3"), 50,      NULL);
    worksheet_write_number(worksheet3, CELL("F3"), 50,      NULL);
    worksheet_write_number(worksheet3, CELL("G3"), 50,      NULL);
    worksheet_write_formula_num(worksheet3, CELL("H3"), "=SUM(B3:G3)", NULL, 210);

    worksheet_write_string(worksheet3, CELL("A4"), "East",  NULL);
    worksheet_write_number(worksheet3, CELL("B4"), 45,      NULL);
    worksheet_write_number(worksheet3, CELL("C4"), 75,      NULL);
    worksheet_write_number(worksheet3, CELL("D4"), 50,      NULL);
    worksheet_write_number(worksheet3, CELL("E4"), 15,      NULL);
    worksheet_write_number(worksheet3, CELL("F4"), 75,      NULL);
    worksheet_write_number(worksheet3, CELL("G4"), 100,     NULL);
    worksheet_write_formula_num(worksheet3, CELL("H4"), "=SUM(B4:G4)", NULL, 360);

    worksheet_write_string(worksheet3, CELL("A5"), "West",  NULL);
    worksheet_write_number(worksheet3, CELL("B5"), 15,      NULL);
    worksheet_write_number(worksheet3, CELL("C5"), 15,      NULL);
    worksheet_write_number(worksheet3, CELL("D5"), 55,      NULL);
    worksheet_write_number(worksheet3, CELL("E5"), 35,      NULL);
    worksheet_write_number(worksheet3, CELL("F5"), 20,      NULL);
    worksheet_write_number(worksheet3, CELL("G5"), 50,      NULL);
    worksheet_write_formula_num(worksheet3, CELL("H5"), "=SUM(B5:G5)", NULL, 190);

    worksheet_write_formula_num(worksheet3, CELL("H6"), "=SUM(H2:H5)", bold, 1015);


    worksheet_set_row(worksheet3, 0, LXW_DEF_ROW_HEIGHT, bold);

    worksheet_set_column(    worksheet3, COLS("A:A"), 10, bold);
    worksheet_set_column(    worksheet3, COLS("H:H"), 10, NULL);
    worksheet_set_column_opt(worksheet3, COLS("B:G"),  6, NULL, &options6);

    return workbook_close(workbook);
}
