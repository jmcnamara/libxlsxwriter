/*
 * A simple formatting example using libxlsxwriter.
 *
 * This program demonstrates the indentation cell format.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("text_indent.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Create formats with different indentation levels. */
    lxw_format *indent1 = workbook_add_format(workbook);
    lxw_format *indent2 = workbook_add_format(workbook);

    format_set_indent(indent1, 1);
    format_set_indent(indent2, 2);

    /* Make the column wider for clarity. */
    worksheet_set_column(worksheet, 0, 0, 40, NULL);

    /* Write some indented text. */
    worksheet_write_string(worksheet, 0, 0, "This text is indented 1 level",  indent1);
    worksheet_write_string(worksheet, 1, 0, "This text is indented 2 levels", indent2);

    return workbook_close(workbook);
}
