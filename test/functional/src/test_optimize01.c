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

    lxw_workbook  *workbook  = workbook_new_opt("test_optimize01.xlsx", &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
    /* For testing overwrite the 0, 0 cell to ensure the original is freed. */
    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);

    worksheet_write_number(worksheet, 1, 0, 123,     NULL);

    return workbook_close(workbook);
}
