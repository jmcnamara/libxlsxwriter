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

    lxw_workbook  *workbook  = workbook_new("test_comment56.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_number(worksheet, CELL("A1"),  1234, NULL);
    worksheet_write_number(worksheet, CELL("C7"),  1234, NULL);
    worksheet_write_number(worksheet, CELL("G14"), 1234, NULL);

    worksheet_write_comment(worksheet, CELL("A1"),  "Some text");
    worksheet_write_comment(worksheet, CELL("D1"),  "Some text");
    worksheet_write_comment(worksheet, CELL("C7"),  "Some text");
    worksheet_write_comment(worksheet, CELL("E10"), "Some text");
    worksheet_write_comment(worksheet, CELL("G14"), "Some text");

    /* Repeat above to check for overwrite leaks. */
    worksheet_write_string(worksheet, CELL("A1"),  "Foo", NULL);
    worksheet_write_string(worksheet, CELL("C7"),  "Bar", NULL);
    worksheet_write_string(worksheet, CELL("G14"), "Baz", NULL);

    worksheet_write_comment(worksheet, CELL("A1"),  "Some text");
    worksheet_write_comment(worksheet, CELL("D1"),  "Some text");
    worksheet_write_comment(worksheet, CELL("C7"),  "Some text");
    worksheet_write_comment(worksheet, CELL("E10"), "Some text");
    worksheet_write_comment(worksheet, CELL("G14"), "Some text");

    worksheet_set_comments_author(worksheet, "John");

    return workbook_close(workbook);
}
