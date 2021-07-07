/*
 * An example of using the new Excel LAMBDA() function with the libxlsxwriter
 * library. Note, this function is only currently available if you are
 * subscribed to the Microsoft Office Beta Channel program.
 *
 * Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("lambda.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0,
                           "Note: Lambda functions currently only work with "
                           "the Beta Channel versions of Excel 365", NULL);

    /* Note that the formula name is prefixed with "_xlfn." and that the
     * lambda function parameters are prefixed with "_xlpm.". These prefixes
     * won't show up in Excel.
     */
    worksheet_write_dynamic_formula(worksheet, CELL("A2"),
                                    "=_xlfn.LAMBDA(_xlpm.temp, (5/9) * (_xlpm.temp-32))(32)",
                                    NULL);

    /* Create the lambda function as a defined name and write it as a dynamic formula. */
    workbook_define_name(workbook,
                         "ToCelsius",
                         "=_xlfn.LAMBDA(_xlpm.temp, (5/9) * (_xlpm.temp-32))");

    worksheet_write_dynamic_formula(worksheet, CELL("A3"), "=ToCelsius(212)", NULL);


    workbook_close(workbook);

    return 0;
}
