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

    lxw_workbook  *workbook  = workbook_new("test_set_column01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("A:A"),   0.08, NULL);
    worksheet_set_column(worksheet, COLS("B:B"),   0.17, NULL);
    worksheet_set_column(worksheet, COLS("C:C"),   0.25, NULL);
    worksheet_set_column(worksheet, COLS("D:D"),   0.33, NULL);
    worksheet_set_column(worksheet, COLS("E:E"),   0.42, NULL);
    worksheet_set_column(worksheet, COLS("F:F"),   0.5,  NULL);
    worksheet_set_column(worksheet, COLS("G:G"),   0.58, NULL);
    worksheet_set_column(worksheet, COLS("H:H"),   0.67, NULL);
    worksheet_set_column(worksheet, COLS("I:I"),   0.75, NULL);
    worksheet_set_column(worksheet, COLS("J:J"),   0.83, NULL);
    worksheet_set_column(worksheet, COLS("K:K"),   0.92, NULL);
    worksheet_set_column(worksheet, COLS("L:L"),   1,    NULL);
    worksheet_set_column(worksheet, COLS("M:M"),   1.14, NULL);
    worksheet_set_column(worksheet, COLS("N:N"),   1.29, NULL);
    worksheet_set_column(worksheet, COLS("O:O"),   1.43, NULL);
    worksheet_set_column(worksheet, COLS("P:P"),   1.57, NULL);
    worksheet_set_column(worksheet, COLS("Q:Q"),   1.71, NULL);
    worksheet_set_column(worksheet, COLS("R:R"),   1.86, NULL);
    worksheet_set_column(worksheet, COLS("S:S"),   2,    NULL);
    worksheet_set_column(worksheet, COLS("T:T"),   2.14, NULL);
    worksheet_set_column(worksheet, COLS("U:U"),   2.29, NULL);
    worksheet_set_column(worksheet, COLS("V:V"),   2.43, NULL);
    worksheet_set_column(worksheet, COLS("W:W"),   2.57, NULL);
    worksheet_set_column(worksheet, COLS("X:X"),   2.71, NULL);
    worksheet_set_column(worksheet, COLS("Y:Y"),   2.86, NULL);
    worksheet_set_column(worksheet, COLS("Z:Z"),   3,    NULL);
    worksheet_set_column(worksheet, COLS("AB:AB"), 8.57, NULL);
    worksheet_set_column(worksheet, COLS("AC:AC"), 8.71, NULL);
    worksheet_set_column(worksheet, COLS("AD:AD"), 8.86, NULL);
    worksheet_set_column(worksheet, COLS("AE:AE"), 9,    NULL);
    worksheet_set_column(worksheet, COLS("AF:AF"), 9.14, NULL);
    worksheet_set_column(worksheet, COLS("AG:AG"), 9.29, NULL);

    return workbook_close(workbook);
}
