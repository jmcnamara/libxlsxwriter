/*
 * An example of inserting textboxes into a worksheet using the
 * libxlsxwriter library.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("textbox.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Insert a simple textbox with default size. */
    worksheet_insert_textbox(worksheet, 1, 1, "This is a textbox");

    /* Insert a textbox with custom size. */
    lxw_textbox_options options = {.width = 256, .height = 100};
    worksheet_insert_textbox_opt(worksheet, 8, 1, "A larger textbox", &options);

    /* Insert a textbox with offset. */
    lxw_textbox_options options2 = {.x_offset = 10, .y_offset = 10};
    worksheet_insert_textbox_opt(worksheet, 15, 1, "Offset textbox", &options2);

    workbook_close(workbook);

    return 0;
}
