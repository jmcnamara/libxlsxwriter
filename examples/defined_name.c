/*
 * Example of how to create defined names using libxlsxwriter. This method is
 * used to define a user friendly name to represent a value, a single cell or
 * a range of cells in a workbook.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook   = new_workbook("defined_name.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);

    /* Define some global/workbook names. */
    workbook_define_name(workbook, "Sales", "=!G1:H10");

    workbook_define_name(workbook, "Exchange_rate", "=0.96");
    workbook_define_name(workbook, "Sales",         "=Sheet1!$G$1:$H$10");

    /* Define a local/worksheet name. */
    workbook_define_name(workbook, "Sheet2!Sales",  "=Sheet2!$G$1:$G$10");

    /* Write some text to the first worksheet and a defined names in a formula. */
    worksheet_set_column(worksheet1, 0, 0, 45, NULL, NULL);

    worksheet_write_string(worksheet1, 0, 0,
                           "This worksheet contains some defined names.", NULL);

    worksheet_write_string(worksheet1, 1, 0,
                           "See Formulas -> Name Manager above.", NULL);

    worksheet_write_string(worksheet1, 2, 0,
                           "Example formula in cell B3 ->", NULL);

    worksheet_write_formula(worksheet1, 2, 1, "=Exchange_rate", NULL);

    /* Write some text to the second worksheet and a defined names in a formula. */
    worksheet_set_column(worksheet2, 0, 0, 45, NULL, NULL);

    worksheet_write_string(worksheet2, 0, 0,
                           "This worksheet contains some defined names.", NULL);

    worksheet_write_string(worksheet2, 1, 0,
                           "See Formulas -> Name Manager above.", NULL);

    worksheet_write_string(worksheet2, 2, 0,
                           "Example formula in cell B3 ->", NULL);

    worksheet_write_formula(worksheet2, 2, 1, "=Exchange_rate", NULL);


    return workbook_close(workbook);
}
