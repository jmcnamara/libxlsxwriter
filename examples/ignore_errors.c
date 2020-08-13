/*
 * An example of turning off worksheet cells errors/warnings using
 * libxlsxwriter.
 *
 * Copyright 2014-2020, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("ignore_errors.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Write strings that looks like numbers. This will cause an Excel warning. */
    worksheet_write_string(worksheet, CELL("C2"), "123", NULL);
    worksheet_write_string(worksheet, CELL("C3"), "123", NULL);

    /* Write a divide by zero formula. This will also cause an Excel warning. */
    worksheet_write_formula(worksheet, CELL("C5"), "=1/0", NULL);
    worksheet_write_formula(worksheet, CELL("C6"), "=1/0", NULL);

    /* Turn off some of the warnings: */
    worksheet_ignore_errors(worksheet, LXW_IGNORE_NUMBER_STORED_AS_TEXT, "C3");
    worksheet_ignore_errors(worksheet, LXW_IGNORE_EVAL_ERROR,            "C6");

    /* Write some descriptions for the cells and make the column wider for clarity. */
    worksheet_set_column(worksheet, 1, 1, 16, NULL);
    worksheet_write_string(worksheet, CELL("B2"), "Warning:",            NULL);
    worksheet_write_string(worksheet, CELL("B3"), "Warning turned off:", NULL);
    worksheet_write_string(worksheet, CELL("B5"), "Warning:",            NULL);
    worksheet_write_string(worksheet, CELL("B6"), "Warning turned off:", NULL);

    workbook_close(workbook);

    return 0;
}
