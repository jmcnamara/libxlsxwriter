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

    lxw_workbook  *workbook  = workbook_new("test_image49.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);

    worksheet_insert_image(worksheet1, CELL("A1"), "images/blue.png");
    worksheet_insert_image(worksheet1, CELL("B3"), "images/red.jpg");
    worksheet_insert_image(worksheet1, CELL("D5"), "images/yellow.jpg");
    worksheet_insert_image(worksheet1, CELL("F9"), "images/grey.png");

    worksheet_insert_image(worksheet2, CELL("A1"), "images/blue.png");
    worksheet_insert_image(worksheet2, CELL("B3"), "images/red.jpg");
    worksheet_insert_image(worksheet2, CELL("D5"), "images/yellow.jpg");
    worksheet_insert_image(worksheet2, CELL("F9"), "images/grey.png");

    worksheet_insert_image(worksheet3, CELL("A1"), "images/blue.png");
    worksheet_insert_image(worksheet3, CELL("B3"), "images/red.jpg");
    worksheet_insert_image(worksheet3, CELL("D5"), "images/yellow.jpg");
    worksheet_insert_image(worksheet3, CELL("F9"), "images/grey.png");

    return workbook_close(workbook);
}
