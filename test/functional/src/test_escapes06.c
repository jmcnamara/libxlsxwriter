/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_escapes06.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_format *num_format = workbook_add_format(workbook);

    format_set_num_format(num_format, "[Red]0.0%\\ \"a\"");

    worksheet_set_column(worksheet, 0, 0, 14, NULL);

    worksheet_write_number(worksheet, CELL("A1"), 123, num_format);

    return workbook_close(workbook);
}
