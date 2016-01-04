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

    lxw_workbook  *workbook  = workbook_new("test_row_col_format08.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format    *bold      = workbook_add_format(workbook);
    format_set_bold(bold);

    lxw_format    *mixed     = workbook_add_format(workbook);
    format_set_bold(mixed);
    format_set_italic(mixed);

    lxw_format    *italic    = workbook_add_format(workbook);
    format_set_italic(italic);

    /* Manually force the format index order for testing. */
    lxw_workbook_set_default_xf_indices(workbook);

    worksheet_set_row(worksheet, 0, 15, bold);
    worksheet_set_column(worksheet, 0, 0, 8.43, italic);

    worksheet_write_string(worksheet, 0, 0, "Foo", mixed);
    worksheet_write_string(worksheet, 0, 1, "Foo", NULL);
    worksheet_write_string(worksheet, 1, 0, "Foo", NULL);
    worksheet_write_string(worksheet, 1, 1, "Foo", NULL);


    return workbook_close(workbook);
}
