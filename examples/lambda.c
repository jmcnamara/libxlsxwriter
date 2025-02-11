/*
 * An example of using the new Excel LAMBDA() function with the libxlsxwriter
 * library.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("lambda.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Note that the formula name is prefixed with "_xlfn." and that the
     * lambda function parameters are prefixed with "_xlpm.". These prefixes
     * won't show up in Excel.
     */
    worksheet_write_dynamic_formula(worksheet, CELL("A1"),
                                    "=_xlfn.LAMBDA(_xlpm.temp, (5/9) * (_xlpm.temp-32))(32)",
                                    NULL);

    /* Create the lambda function as a defined name and write it as a dynamic formula. */
    workbook_define_name(workbook,
                         "ToCelsius",
                         "=_xlfn.LAMBDA(_xlpm.temp, (5/9) * (_xlpm.temp-32))");

    worksheet_write_dynamic_formula(worksheet, CELL("A2"), "=ToCelsius(212)", NULL);


    workbook_close(workbook);

    return 0;
}
