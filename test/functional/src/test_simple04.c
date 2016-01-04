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

    lxw_datetime datetime1 = {0,    0,  0, 12, 0, 0};
    lxw_datetime datetime2 = {2013, 1, 27,  0, 0, 0};

    /* Use deprecated constructor for testing. */
    lxw_workbook  *workbook  = new_workbook("test_simple04.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format    *format1   = workbook_add_format(workbook);
    lxw_format    *format2   = workbook_add_format(workbook);
    format_set_num_format_index(format1, 20);
    format_set_num_format_index(format2, 14);

    worksheet_set_column(worksheet, 0, 0, 12, NULL);

    worksheet_write_datetime(worksheet, 0, 0, &datetime1, format1);
    worksheet_write_datetime(worksheet, 1, 0, &datetime2, format2);

    return workbook_close(workbook);
}
