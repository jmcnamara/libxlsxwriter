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

    lxw_workbook  *workbook  = workbook_new("test_header_image10.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);

    lxw_header_footer_options header_options = {.image_left = "images/red.jpg"};

    worksheet_set_header_opt(worksheet1, "&L&G", &header_options);

    worksheet_write_string(worksheet2, CELL("A1"), "Foo", NULL);
    worksheet_write_comment(worksheet2, CELL("B2"), "Some text");

    worksheet_set_comments_author(worksheet2, "John");

    return workbook_close(workbook);
}
