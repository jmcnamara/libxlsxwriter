/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test case for writing data in optimization mode.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_optimize24.xlsx");

    workbook->optimize = 1;

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_format    *bold      = workbook_add_format(workbook);

    format_set_bold(bold);

    worksheet_set_row(worksheet, 0, 20, bold, NULL);
    worksheet_write_string(worksheet, 0, 0, "Foo", NULL);

    return workbook_close(workbook);
}
