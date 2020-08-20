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

    lxw_workbook  *workbook  = workbook_new("test_cond_format09.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format = workbook_add_format(workbook);
    format_set_font_color(format, 0x9C6500);
    format_set_bg_color(format, 0xFFEB9C);
    format_set_font_condense(format);
    format_set_font_extend(format);

    worksheet_write_number(worksheet, CELL("A1"), 10 , NULL);
    worksheet_write_number(worksheet, CELL("A2"), 20 , NULL);
    worksheet_write_number(worksheet, CELL("A3"), 30 , NULL);
    worksheet_write_number(worksheet, CELL("A4"), 40 , NULL);

    worksheet_conditional_tmp(worksheet, CELL("A1"), format);

/*
    worksheet_conditional_formatting(worksheet, "A1",
      :
            'type': "cell",
            'format': format,
            'criteria': "greater than",
            'value': 5
    );
*/

    return workbook_close(workbook);
}
