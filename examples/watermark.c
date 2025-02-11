/*
 * An example of adding a worksheet watermark image using libxlsxwriter. This
 * is based on the method of putting an image in the worksheet header as
 * suggested in the Microsoft documentation:
 * https://support.microsoft.com/en-us/office/add-a-watermark-in-excel-a372182a-d733-484e-825c-18ddf3edf009
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("watermark.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);


    /* Set a worksheet header with the watermark image. */
    lxw_header_footer_options header_options = {.image_center = "watermark.png"};
    worksheet_set_header_opt(worksheet, "&C&[Picture]", &header_options);

    workbook_close(workbook);

    return 0;
}
