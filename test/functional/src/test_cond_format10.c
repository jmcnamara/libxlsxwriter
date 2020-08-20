/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2020, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_cond_format10.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format = workbook_add_format(workbook);
    format_set_bold(format);
    format_set_italic(format);

    worksheet_write_string(worksheet, CELL("A1"), "Hello", format);

    worksheet_write_number(worksheet, CELL("B3"), 10 , NULL);
    worksheet_write_number(worksheet, CELL("B4"), 20 , NULL);
    worksheet_write_number(worksheet, CELL("B5"), 30 , NULL);
    worksheet_write_number(worksheet, CELL("B6"), 40 , NULL);

    worksheet_conditional_tmp(worksheet, CELL("A1"), format);

/*    worksheet_conditional_formatting(worksheet, "B3:B6",
      :
            'type': "cell",
            'format': format,
            'criteria': "greater than",
            'value': 20
    );*/

    return workbook_close(workbook);
}
