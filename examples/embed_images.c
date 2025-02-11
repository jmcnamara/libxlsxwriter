/*
 * An example of embedding images into a worksheet using the libxlsxwriter
 * library.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    /* Create a new workbook and add a worksheet. */
    lxw_workbook  *workbook  = workbook_new("embed_images.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Change some of the column widths for clarity. */
    worksheet_set_column(worksheet, COLS("A:B"), 30, NULL);

    /* Embed an image. */
    worksheet_write_string(worksheet, CELL("A2"), "Embed an image in a cell:", NULL);
    worksheet_embed_image(worksheet, CELL("B2"), "logo.png");

    /* Make a row bigger and embed the image. */
    worksheet_set_row(worksheet, 3, 72, NULL);
    worksheet_write_string(worksheet, CELL("A4"), "Embed an image in a cell:", NULL);
    worksheet_embed_image(worksheet, CELL("B4"), "logo.png");

    /* Make a row bigger and embed the image. */
    worksheet_set_row(worksheet, 5, 150, NULL);
    worksheet_write_string(worksheet, CELL("A6"), "Embed an image in a cell:", NULL);
    worksheet_embed_image(worksheet, CELL("B6"), "logo.png");

    workbook_close(workbook);

    return 0;
}
