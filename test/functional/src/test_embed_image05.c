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

    lxw_workbook  *workbook  = workbook_new("test_embed_image05.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_dynamic_array_formula(worksheet, 0, 0, 2, 0, "=LEN(B1:B3)", NULL);

    worksheet_embed_image(worksheet, 8, 4, "images/red.png");

    return workbook_close(workbook);
}
