/*
 * Example of how to use the libxlsxwriter library to write simple
 * array formulas.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    /* Create a new workbook and add a worksheet. */
    lxw_workbook  *workbook  = workbook_new("array_formula.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Write some data for the formulas. */
    worksheet_write_number(worksheet, 0, 1, 500, NULL);
    worksheet_write_number(worksheet, 1, 1, 10, NULL);
    worksheet_write_number(worksheet, 4, 1, 1, NULL);
    worksheet_write_number(worksheet, 5, 1, 2, NULL);
    worksheet_write_number(worksheet, 6, 1, 3, NULL);

    worksheet_write_number(worksheet, 0, 2, 300, NULL);
    worksheet_write_number(worksheet, 1, 2, 15, NULL);
    worksheet_write_number(worksheet, 4, 2, 20234, NULL);
    worksheet_write_number(worksheet, 5, 2, 21003, NULL);
    worksheet_write_number(worksheet, 6, 2, 10000, NULL);

    /* Write an array formula that returns a single value. */
    worksheet_write_array_formula(worksheet, 0, 0, 0, 0, "{=SUM(B1:C1*B2:C2)}", NULL);

    /* Similar to above but using the RANGE macro. */
    worksheet_write_array_formula(worksheet, RANGE("A2:A2"), "{=SUM(B1:C1*B2:C2)}", NULL);

    /* Write an array formula that returns a range of values. */
    worksheet_write_array_formula(worksheet, 4, 0, 6, 0, "{=TREND(C5:C7,B5:B7)}", NULL);

    workbook_close(workbook);

    return 0;
}
