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

    lxw_workbook  *workbook  = workbook_new("test_embed_image11.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, 0, 0, 11, NULL);
    worksheet_set_row(worksheet, 0, 72, NULL);

    worksheet_embed_image(worksheet, 0, 0, "images/red.png");

    return workbook_close(workbook);
}
