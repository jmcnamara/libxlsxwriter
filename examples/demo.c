/*
 * A simple example of some of the features of the libxlsxwriter library.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    /* Create a new workbook and add a worksheet. */
    lxw_workbook  *workbook  = workbook_new("demo.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Add a format. */
    lxw_format *format = workbook_add_format(workbook);

    /* Set the bold property for the format */
    format_set_bold(format);

    /* Change the column width for clarity. */
    worksheet_set_column(worksheet, 0, 0, 20, NULL);

    /* Write some simple text. */
    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);

    /* Text with formatting. */
    worksheet_write_string(worksheet, 1, 0, "World", format);

    /* Writer some numbers. */
    worksheet_write_number(worksheet, 2, 0, 123,     NULL);
    worksheet_write_number(worksheet, 3, 0, 123.456, NULL);

    /* Insert an image. */
    worksheet_insert_image(worksheet, 1, 2, "logo.png");

    workbook_close(workbook);

    return 0;
}
