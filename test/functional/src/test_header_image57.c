/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_header_image57.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_header_footer_options header_options = {.image_left = "images/red.jpg"};
    worksheet_set_header_opt(worksheet, "&L&G", &header_options);

    /* Replace a footer with one that doesn't use the previous center and right
     * images, to check that the old images are removed and not accessed after
     * being freed. */
    lxw_header_footer_options footer_options1 = {.image_center = "images/red.jpg",
                                                 .image_right  = "images/yellow.jpg"};
    worksheet_set_footer_opt(worksheet, "&C&G&R&G", &footer_options1);

    lxw_header_footer_options footer_options2 = {.image_left = "images/blue.jpg"};
    worksheet_set_footer_opt(worksheet, "&L&G", &footer_options2);

    return workbook_close(workbook);
}
