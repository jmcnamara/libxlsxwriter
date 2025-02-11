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

    lxw_workbook  *workbook  = workbook_new("test_background07.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_insert_image(worksheet, CELL("E9"), "images/logo.jpg");
    worksheet_set_background(worksheet, "images/logo.jpg");

    lxw_header_footer_options header_options = {.image_center = "images/blue.jpg"};
    worksheet_set_header_opt(worksheet, "&C&G", &header_options);

    worksheet_write_string(worksheet, CELL("A1"), "Foo" , NULL);
    worksheet_write_comment(worksheet, CELL("B2"), "Some text");
    worksheet_set_comments_author(worksheet, "John");

    return workbook_close(workbook);
}
