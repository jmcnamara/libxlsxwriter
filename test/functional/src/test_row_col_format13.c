/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case to test worksheet set_row() and set_column().
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook     = workbook_new("test_row_col_format13.xlsx");
    lxw_worksheet *worksheet    = workbook_add_worksheet(workbook, NULL);
    lxw_row_col_options options = {1, 0, 0};
    lxw_format    *bold         = workbook_add_format(workbook);

    format_set_bold(bold);

    worksheet_set_column(worksheet, COLS("B:D"), 5, NULL);
    worksheet_set_column_opt(worksheet, COLS("F:F"), 8, NULL, &options);
    worksheet_set_column(worksheet, COLS("H:H"), LXW_DEF_COL_WIDTH, bold);
    worksheet_set_column(worksheet, COLS("J:J"), 2, NULL);
    worksheet_set_column_opt(worksheet, COLS("L:L"), LXW_DEF_COL_WIDTH, NULL, &options);

    return workbook_close(workbook);
}
