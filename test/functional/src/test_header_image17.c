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

    lxw_workbook  *workbook  = workbook_new("test_header_image17.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_header_footer_options header_options1 = {.image_left   = "images/red.jpg",
                                                 .image_center = "images/blue.jpg",
                                                 .image_right  = "images/red.jpg"};

    lxw_header_footer_options header_options2 = {.image_left   = "images/blue.jpg",
                                                 .image_center = "images/red.jpg",
                                                 .image_right  = "images/blue.jpg"};

    worksheet_set_header_opt(worksheet, "&L&G&C&G&R&G", &header_options1);
    worksheet_set_footer_opt(worksheet, "&L&G&C&G&R&G", &header_options2);

    return workbook_close(workbook);
}
