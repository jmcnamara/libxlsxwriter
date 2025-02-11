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

    lxw_workbook  *workbook  = workbook_new("test_header_image06.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);

    lxw_header_footer_options header_options1 = {.image_left = "images/red.jpg"};
    lxw_header_footer_options header_options2 = {.image_left = "images/blue.jpg"};

    worksheet_set_header_opt(worksheet1, "&L&G", &header_options1);
    worksheet_set_header_opt(worksheet2, "&L&G", &header_options2);

    return workbook_close(workbook);
}
