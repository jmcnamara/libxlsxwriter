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

    lxw_workbook  *workbook  = workbook_new("test_background08.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Use different images for the worksheet image and the background, to
     * check that each one is linked to the correct image file. */
    worksheet_insert_image(worksheet, CELL("E9"), "images/red.png");
    worksheet_set_background(worksheet, "images/logo.jpg");

    return workbook_close(workbook);
}
