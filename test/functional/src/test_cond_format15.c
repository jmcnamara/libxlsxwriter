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

    lxw_workbook  *workbook  = workbook_new("test_cond_format15.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format1 = workbook_add_format(workbook);
    format_set_bg_color(format1, LXW_COLOR_RED);

    lxw_format *format2 = workbook_add_format(workbook);
    format_set_bg_color(format2, 0x92D050);

    worksheet_write_number(worksheet, CELL("A1"), 10 , NULL);
    worksheet_write_number(worksheet, CELL("A2"), 20 , NULL);
    worksheet_write_number(worksheet, CELL("A3"), 30 , NULL);
    worksheet_write_number(worksheet, CELL("A4"), 40 , NULL);

    worksheet_conditional_tmp(worksheet, CELL("A1"), format1);
    worksheet_conditional_tmp(worksheet, CELL("A1"), format2);

/*
    worksheet_conditional_formatting(worksheet, "A1",
      :
            'type': "cell",
            'format': format1,
            'criteria': "less than",
            'value': 5,
            'stop_if_true': 1
    );

    worksheet_conditional_formatting(worksheet, "A1",
      :
            'type': "cell",
            'format': format2,
            'criteria': "greater than",
            'value': 20,
            'stop_if_true': 1
    );
*/

    return workbook_close(workbook);
}
