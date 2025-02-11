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

    lxw_workbook  *workbook  = workbook_new("test_header_image14.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_header_footer_options header_options = {.image_left   = "images/black_72e.png",
                                                .image_center = "images/black_150e.png",
                                                .image_right  = "images/black_300e.png"};

    worksheet_set_header_opt(worksheet, "&L&G&C&G&R&G", &header_options);

    return workbook_close(workbook);
}
