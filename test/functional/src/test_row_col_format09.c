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

    lxw_workbook  *workbook  = workbook_new("test_row_col_format09.xlsx");
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

    worksheet_set_row(worksheet, 4, 15, bold);
    worksheet_set_column(worksheet, 2, 2, 8.43, italic);

    worksheet_write_string(worksheet, 0, 2, "Foo", NULL);
    worksheet_write_string(worksheet, 4, 0, "Foo", NULL);
    worksheet_write_string(worksheet, 4, 2, "Foo", mixed);

    return workbook_close(workbook);
}
