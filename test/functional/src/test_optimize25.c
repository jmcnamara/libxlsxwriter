/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test case for writing data in optimization mode.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook_options options = {1, NULL};

    lxw_workbook  *workbook  = workbook_new_opt("test_optimize25.xlsx", &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_format    *bold      = workbook_add_format(workbook);

    format_set_bold(bold);

    worksheet_set_row(worksheet, 0, 20, bold);
    worksheet_write_string(worksheet, 2, 0, "Foo", NULL);

    return workbook_close(workbook);
}
