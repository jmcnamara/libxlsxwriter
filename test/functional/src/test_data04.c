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

    lxw_workbook  *workbook  = workbook_new("test_data04.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0,       0, "Foo",  NULL);
    worksheet_write_string(worksheet, 0,       1, "Bar",  NULL);
    worksheet_write_string(worksheet, 1,       0, "Bing", NULL);
    worksheet_write_string(worksheet, 2,       0, "Buzz", NULL);
    worksheet_write_string(worksheet, 1048575, 0, "End",  NULL);

    return workbook_close(workbook);
}
