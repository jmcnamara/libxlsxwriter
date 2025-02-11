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

    lxw_workbook  *workbook  = workbook_new("test_header_image08.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, CELL("A1"), "Foo", NULL);
    worksheet_write_comment(worksheet, CELL("B2"), "Some text");

    worksheet_set_comments_author(worksheet, "John");

    lxw_header_footer_options header_options = {.image_left = "images/red.jpg"};

    worksheet_set_header_opt(worksheet, "&L&G", &header_options);

    return workbook_close(workbook);
}
