/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook   = workbook_new("test_embed_image14.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);

    /* An image on a worksheet before the embedded image shouldn't affect the
     * embedded image's cell metadata index or its image file. */
    worksheet_insert_image(worksheet1, CELL("E9"), "images/red.png");

    worksheet_embed_image(worksheet2, CELL("A1"), "images/blue.png");

    return workbook_close(workbook);
}
