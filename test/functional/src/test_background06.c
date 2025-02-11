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

    lxw_workbook  *workbook  = workbook_new("test_background06.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_insert_image(worksheet, CELL("E9"), "images/logo.jpg");
    worksheet_set_background(worksheet, "images/logo.jpg");

    lxw_header_footer_options header_options = {.image_center = "images/blue.jpg"};
    worksheet_set_header_opt(worksheet, "&C&G", &header_options);

    return workbook_close(workbook);
}
