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

    lxw_workbook  *workbook  = workbook_new("test_header_image02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_header_footer_options header_options = {.image_left   = "images/red.jpg",
                                                .image_center = "images/blue.jpg"};

    worksheet_set_header_opt(worksheet, "&L&G&C&G", &header_options);

    return workbook_close(workbook);
}
