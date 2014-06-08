/*
 * A simple example of some of the features of the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    /* Create a new workbook. */
    lxw_workbook  *workbook  = new_workbook("hello_world.xlsx");

    /* Add a worksheet. */
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Add a format. */
    lxw_format    *format    = workbook_add_format(workbook);

    /* Set the bold property for the format */
    format_set_bold(format);

    /* Write some simple text. */
    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);

    /* Text with formatting. */
    worksheet_write_string(worksheet, 1, 0, "World", format);

    /* Writer some numbers. */
    worksheet_write_number(worksheet, 1, 0, 123,     NULL);
    worksheet_write_number(worksheet, 1, 0, 123.456, NULL);

    workbook_close(workbook);

    return 0;
}
