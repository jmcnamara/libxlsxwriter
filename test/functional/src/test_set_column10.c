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

    lxw_workbook  *workbook  = workbook_new("test_set_column10.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column_pixels(worksheet, COLS("A:A"),   1,  NULL);
    worksheet_set_column_pixels(worksheet, COLS("B:B"),   2,  NULL);
    worksheet_set_column_pixels(worksheet, COLS("C:C"),   3,  NULL);
    worksheet_set_column_pixels(worksheet, COLS("D:D"),   4,  NULL);
    worksheet_set_column_pixels(worksheet, COLS("E:E"),   5,  NULL);
    worksheet_set_column_pixels(worksheet, COLS("F:F"),   6,  NULL);
    worksheet_set_column_pixels(worksheet, COLS("G:G"),   7,  NULL);
    worksheet_set_column_pixels(worksheet, COLS("H:H"),   8,  NULL);
    worksheet_set_column_pixels(worksheet, COLS("I:I"),   9,  NULL);
    worksheet_set_column_pixels(worksheet, COLS("J:J"),   10, NULL);
    worksheet_set_column_pixels(worksheet, COLS("K:K"),   11, NULL);
    worksheet_set_column_pixels(worksheet, COLS("L:L"),   12, NULL);
    worksheet_set_column_pixels(worksheet, COLS("M:M"),   13, NULL);
    worksheet_set_column_pixels(worksheet, COLS("N:N"),   14, NULL);
    worksheet_set_column_pixels(worksheet, COLS("O:O"),   15, NULL);
    worksheet_set_column_pixels(worksheet, COLS("P:P"),   16, NULL);
    worksheet_set_column_pixels(worksheet, COLS("Q:Q"),   17, NULL);
    worksheet_set_column_pixels(worksheet, COLS("R:R"),   18, NULL);
    worksheet_set_column_pixels(worksheet, COLS("S:S"),   19, NULL);
    worksheet_set_column_pixels(worksheet, COLS("T:T"),   20, NULL);
    worksheet_set_column_pixels(worksheet, COLS("U:U"),   21, NULL);
    worksheet_set_column_pixels(worksheet, COLS("V:V"),   22, NULL);
    worksheet_set_column_pixels(worksheet, COLS("W:W"),   23, NULL);
    worksheet_set_column_pixels(worksheet, COLS("X:X"),   24, NULL);
    worksheet_set_column_pixels(worksheet, COLS("Y:Y"),   25, NULL);
    worksheet_set_column_pixels(worksheet, COLS("Z:Z"),   26, NULL);
    worksheet_set_column_pixels(worksheet, COLS("AB:AB"), 65, NULL);
    worksheet_set_column_pixels(worksheet, COLS("AC:AC"), 66, NULL);
    worksheet_set_column_pixels(worksheet, COLS("AD:AD"), 67, NULL);
    worksheet_set_column_pixels(worksheet, COLS("AE:AE"), 68, NULL);
    worksheet_set_column_pixels(worksheet, COLS("AF:AF"), 69, NULL);
    worksheet_set_column_pixels(worksheet, COLS("AG:AG"), 70, NULL);

    return workbook_close(workbook);
}
