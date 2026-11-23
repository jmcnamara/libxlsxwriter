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

    lxw_workbook  *workbook   = workbook_new("test_header_image22.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);

    lxw_header_footer_options header_options = {.image_left = "images/blue.png"};

    worksheet_set_paper(worksheet1, 9);
    worksheet1->vertical_dpi = 200;

    /* A header image doesn't create a drawing part, so it shouldn't affect
     * the drawing number used for the image on the following worksheet. */
    worksheet_set_header_opt(worksheet1, "&L&G", &header_options);

    worksheet_insert_image(worksheet2, CELL("A1"), "images/red.png");

    return workbook_close(workbook);
}
