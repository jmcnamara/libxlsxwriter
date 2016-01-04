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

    lxw_workbook  *workbook  = workbook_new("test_data02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Tests for the row range. */
    worksheet_write_number(worksheet, 0,       0, 123, NULL);
    worksheet_write_number(worksheet, 1048575, 0, 456, NULL);

    /* These should be ignored. */
    worksheet_write_number(worksheet, -1,      0, 123, NULL);
    worksheet_write_number(worksheet, 1048576, 0, 456, NULL);

    return workbook_close(workbook);
}
