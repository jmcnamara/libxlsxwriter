/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_embed_image13.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);

    worksheet_embed_image(worksheet1, 0, 0, "images/red.png");
    worksheet_embed_image(worksheet1, 2, 0, "images/blue.png");
    worksheet_embed_image(worksheet1, 4, 0, "images/yellow.png");

    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);

    worksheet_embed_image(worksheet2, 0, 0, "images/yellow.png");
    worksheet_embed_image(worksheet2, 2, 0, "images/red.png");
    worksheet_embed_image(worksheet2, 4, 0, "images/blue.png");

    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);

    worksheet_embed_image(worksheet3, 0, 0, "images/blue.png");
    worksheet_embed_image(worksheet3, 2, 0, "images/yellow.png");
    worksheet_embed_image(worksheet3, 4, 0, "images/red.png");

    return workbook_close(workbook);
}
