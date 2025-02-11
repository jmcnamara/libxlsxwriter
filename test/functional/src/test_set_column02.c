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

    lxw_workbook  *workbook  = workbook_new("test_set_column02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("A:A"),   0.083333333333333, NULL);
    worksheet_set_column(worksheet, COLS("B:B"),   0.166666666666667, NULL);
    worksheet_set_column(worksheet, COLS("C:C"),   0.250000000000000, NULL);
    worksheet_set_column(worksheet, COLS("D:D"),   0.333333333333333, NULL);
    worksheet_set_column(worksheet, COLS("E:E"),   0.416666666666667, NULL);
    worksheet_set_column(worksheet, COLS("F:F"),   0.500000000000000, NULL);
    worksheet_set_column(worksheet, COLS("G:G"),   0.583333333333333, NULL);
    worksheet_set_column(worksheet, COLS("H:H"),   0.666666666666666, NULL);
    worksheet_set_column(worksheet, COLS("I:I"),   0.750000000000000, NULL);
    worksheet_set_column(worksheet, COLS("J:J"),   0.833333333333333, NULL);
    worksheet_set_column(worksheet, COLS("K:K"),   0.916666666666666, NULL);
    worksheet_set_column(worksheet, COLS("L:L"),   1.000000000000000, NULL);
    worksheet_set_column(worksheet, COLS("M:M"),   1.142857142857140, NULL);
    worksheet_set_column(worksheet, COLS("N:N"),   1.285714285714290, NULL);
    worksheet_set_column(worksheet, COLS("O:O"),   1.428571428571430, NULL);
    worksheet_set_column(worksheet, COLS("P:P"),   1.571428571428570, NULL);
    worksheet_set_column(worksheet, COLS("Q:Q"),   1.714285714285710, NULL);
    worksheet_set_column(worksheet, COLS("R:R"),   1.857142857142860, NULL);
    worksheet_set_column(worksheet, COLS("S:S"),   2.000000000000000, NULL);
    worksheet_set_column(worksheet, COLS("T:T"),   2.142857142857140, NULL);
    worksheet_set_column(worksheet, COLS("U:U"),   2.285714285714290, NULL);
    worksheet_set_column(worksheet, COLS("V:V"),   2.428571428571430, NULL);
    worksheet_set_column(worksheet, COLS("W:W"),   2.571428571428570, NULL);
    worksheet_set_column(worksheet, COLS("X:X"),   2.714285714285710, NULL);
    worksheet_set_column(worksheet, COLS("Y:Y"),   2.857142857142860, NULL);
    worksheet_set_column(worksheet, COLS("Z:Z"),   3.000000000000000, NULL);
    worksheet_set_column(worksheet, COLS("AB:AB"), 8.571428571428570, NULL);
    worksheet_set_column(worksheet, COLS("AC:AC"), 8.711428571428570, NULL);
    worksheet_set_column(worksheet, COLS("AD:AD"), 8.857142857142860, NULL);
    worksheet_set_column(worksheet, COLS("AE:AE"), 9.000000000000000, NULL);
    worksheet_set_column(worksheet, COLS("AF:AF"), 9.142857142857140, NULL);
    worksheet_set_column(worksheet, COLS("AG:AG"), 9.285714285714290, NULL);

    return workbook_close(workbook);
}
