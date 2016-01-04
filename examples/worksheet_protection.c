/*
 * Example of cell locking and formula hiding in an Excel worksheet using
 * libxlsxwriter.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("protection.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *unlocked = workbook_add_format(workbook);
    format_set_unlocked(unlocked);

    lxw_format *hidden = workbook_add_format(workbook);
    format_set_hidden(hidden);

    /* Widen the first column to make the text clearer. */
    worksheet_set_column(worksheet, 0, 0, 40, NULL);

    /* Turn worksheet protection on without a password. */
    worksheet_protect(worksheet, NULL, NULL);


    /* Write a locked, unlocked and hidden cell. */
    worksheet_write_string(worksheet, 0, 0, "B1 is locked. It cannot be edited.",       NULL);
    worksheet_write_string(worksheet, 1, 0, "B2 is unlocked. It can be edited.",        NULL);
    worksheet_write_string(worksheet, 2, 0, "B3 is hidden. The formula isn't visible.", NULL);

    worksheet_write_formula(worksheet, 0, 1, "=1+2", NULL);     /* Locked by default. */
    worksheet_write_formula(worksheet, 1, 1, "=1+2", unlocked);
    worksheet_write_formula(worksheet, 2, 1, "=1+2", hidden);

    workbook_close(workbook);

    return 0;
}
