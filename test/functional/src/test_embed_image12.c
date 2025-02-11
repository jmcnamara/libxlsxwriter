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

    lxw_workbook  *workbook  = workbook_new("test_embed_image12.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format1 = workbook_add_format(workbook);
    format_set_bg_color(  format1, 0xFFFF00);

    lxw_image_options image_options = {.cell_format = format1};

    worksheet_embed_image_opt(worksheet, 0, 0, "images/red.png", &image_options);

    return workbook_close(workbook);
}
