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

    lxw_workbook  *workbook  = workbook_new("test_header_image56.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Replace a header with one that doesn't use the previous right image, to
     * check that the old image is removed and not accessed after being freed. */
    lxw_header_footer_options header_options1 = {.image_right = "images/yellow.jpg"};
    worksheet_set_header_opt(worksheet, "&R&G", &header_options1);

    lxw_header_footer_options header_options2 = {.image_left   = "images/red.jpg",
                                                 .image_center = "images/blue.jpg"};
    worksheet_set_header_opt(worksheet, "&L&G&C&G", &header_options2);

    return workbook_close(workbook);
}
