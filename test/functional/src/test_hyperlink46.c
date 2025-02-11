/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_hyperlink46.xlsx");

    /* Set shorter length for testing. */
    workbook->max_url_length = 255;

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet, CELL("A3"), "Bar" , NULL);

    /* This link is too long and should be ignored, with a warning. */
    worksheet_write_url(worksheet, CELL("A2"), "http://foo.com/this_is_a_long_hyperlink_that_exceeds_a_limit_of_255_characters_xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" , NULL);

    return workbook_close(workbook);
}
