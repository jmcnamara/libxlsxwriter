/*
 * A simple formatting example that demonstrates how to add diagonal
 * cell borders using the libxlsxwriter library.
 *
 * Copyright 2014-2020, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    /* Create a new workbook and add a worksheet. */
    lxw_workbook  *workbook  = workbook_new("diagonal_border.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Add some diagonal border formats. */
    lxw_format *format1 = workbook_add_format(workbook);
    format_set_diag_type(  format1, LXW_DIAGONAL_BORDER_UP);

    lxw_format *format2 = workbook_add_format(workbook);
    format_set_diag_type(  format2, LXW_DIAGONAL_BORDER_DOWN);

    lxw_format *format3 = workbook_add_format(workbook);
    format_set_diag_type(  format3, LXW_DIAGONAL_BORDER_UP_DOWN);

    lxw_format *format4 = workbook_add_format(workbook);
    format_set_diag_type(  format4, LXW_DIAGONAL_BORDER_UP_DOWN);
    format_set_diag_border(format4, LXW_BORDER_HAIR);
    format_set_diag_color( format4, LXW_COLOR_RED);

    worksheet_write_string(worksheet, CELL("B3"),  "Text", format1);
    worksheet_write_string(worksheet, CELL("B6"),  "Text", format2);
    worksheet_write_string(worksheet, CELL("B9"),  "Text", format3);
    worksheet_write_string(worksheet, CELL("B12"), "Text", format4);

    workbook_close(workbook);

    return 0;
}
