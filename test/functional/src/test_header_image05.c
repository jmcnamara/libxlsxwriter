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

    lxw_workbook  *workbook  = workbook_new("test_header_image05.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_header_footer_options header_options = {.image_left = "images/red.jpg"};
    lxw_header_footer_options footer_options = {.image_left = "images/blue.jpg"};

    worksheet_set_header_opt(worksheet, "&L&G", &header_options);
    worksheet_set_footer_opt(worksheet, "&L&G", &footer_options);

    return workbook_close(workbook);
}
