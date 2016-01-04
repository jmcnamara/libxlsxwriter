/*
 * An example of inserting images into a worksheet using the libxlsxwriter
 * library.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    /* Create a new workbook and add a worksheet. */
    lxw_workbook  *workbook  = workbook_new("images.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Change some of the column widths for clarity. */
    worksheet_set_column(worksheet, COLS("A:A"), 30, NULL);

    /* Insert an image. */
    worksheet_write_string(worksheet, CELL("A2"), "Insert an image in a cell:", NULL);

    worksheet_insert_image(worksheet, CELL("B2"), "logo.png");

    /* Insert an image offset in the cell. */
    worksheet_write_string(worksheet, CELL("A12"), "Insert an offset image:", NULL);

    lxw_image_options options1 = {.x_offset = 15, .y_offset = 10};
    worksheet_insert_image_opt(worksheet, CELL("B12"), "logo.png", &options1);

    /* Insert an image with scaling. */
    worksheet_write_string(worksheet, CELL("A23"), "Insert a scaled image:", NULL);

    lxw_image_options options2 = {.x_scale  = 0.5, .y_scale  = 0.5};
    worksheet_insert_image_opt(worksheet, CELL("B23"), "logo.png", &options2);

    workbook_close(workbook);

    return 0;
}
