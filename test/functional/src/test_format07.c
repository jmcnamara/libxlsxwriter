/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case to test data writing.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_format07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format    *format1    = workbook_add_format(workbook);
    lxw_format    *format2    = workbook_add_format(workbook);
    lxw_format    *format3    = workbook_add_format(workbook);


    format_set_num_format(format1, "0.000");
    format_set_num_format(format2, "0.00000");
    format_set_num_format(format3, "0.000000");


    worksheet_write_number(worksheet, 0, 0, 1.2222, NULL);
    worksheet_write_number(worksheet, 1, 0, 1.2222, format1);
    worksheet_write_number(worksheet, 2, 0, 1.2222, format2);
    worksheet_write_number(worksheet, 3, 0, 1.2222, format3);
    worksheet_write_number(worksheet, 4, 0, 1.2222, NULL);

    return workbook_close(workbook);
}
