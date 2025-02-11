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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink18.xlsx");

    /* Set shorter length for testing. */
    workbook->max_url_length = 255;

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    workbook_unset_default_url_format(workbook);

    worksheet_write_url(worksheet, CELL("A1"), "http://google.com/00000000001111111111222222222233333333334444444444555555555566666666666777777777778888888888999999999990000000000111111111122222222223333333333444444444455555555556666666666677777777777888888888899999999999000000000011111111112222222222x", NULL);

    /* This longer url should be ignored. */
    worksheet_write_url(worksheet, CELL("A1"), "http://google.com/00000000001111111111222222222233333333334444444444555555555566666666666777777777778888888888999999999990000000000111111111122222222223333333333444444444455555555556666666666677777777777888888888899999999999000000000011111111112222222222xy", NULL);



    return workbook_close(workbook);
}
