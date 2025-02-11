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

    lxw_workbook  *workbook  = workbook_new("test_header_image01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);


    /*
     *  Test error conditions. These functions should be ignored with warnings.
     */

    /* Test NULL string. */
    worksheet_set_header(worksheet, NULL);
    worksheet_set_footer(worksheet, NULL);


    /* Test long string. */
    char *long_string = "This is a long string longer than the Excel limit of 255 characters used for testing                                                                                                                                                                                    end.";
    worksheet_set_header(worksheet, long_string);
    worksheet_set_footer(worksheet, long_string);


    /* Test &G placeholder without image. */
    lxw_header_footer_options options2 = {0};
    worksheet_set_header_opt(worksheet, "&L&G", NULL);
    worksheet_set_footer_opt(worksheet, "&L&G", NULL);
    worksheet_set_header_opt(worksheet, "&L&G", &options2);
    worksheet_set_footer_opt(worksheet, "&L&G", &options2);

    /* Test too many images/not enough placeholders. */
    lxw_header_footer_options options3 = {.image_left = "images/red.jpg", .image_right = "images/red.jpg"};
    worksheet_set_header_opt(worksheet, "&L&G", &options3);
    worksheet_set_footer_opt(worksheet, "&L&G", &options3);


    /* Write a non-error header. */
    lxw_header_footer_options header_options = {.image_left = "images/red.jpg"};

    worksheet_set_header_opt(worksheet, "&L&G", &header_options);


    return workbook_close(workbook);
}
