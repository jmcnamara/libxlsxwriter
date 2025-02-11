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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink24.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_unset_default_url_format(workbook);

    worksheet_write_url(worksheet, CELL("A1"), "http://www.example.com/some_long_url_that_is_255_characters_long_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_z#some_long_location_that_is_255_characters_long_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_abcdefgh_z", NULL);

    return workbook_close(workbook);
}
