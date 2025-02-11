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

    lxw_workbook  *workbook  = workbook_new("test_header_image09.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet1, CELL("A1"), "Foo", NULL);
    worksheet_write_comment(worksheet1, CELL("B2"), "Some text");

    worksheet_set_comments_author(worksheet1, "John");

    lxw_header_footer_options header_options = {.image_left = "images/red.jpg"};

    worksheet_set_header_opt(worksheet2, "&L&G", &header_options);

    return workbook_close(workbook);
}
