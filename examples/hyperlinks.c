/*
 * Example of writing urls/hyperlinks with the libxlsxwriter library.
 *
 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    /* Create a new workbook. */
    lxw_workbook *workbook   = workbook_new("hyperlinks.xlsx");

    /* Add a worksheet. */
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Get the default url format (used in the overwriting examples below). */
    lxw_format *url_format = workbook_get_default_url_format(workbook);

    /* Create a user defined link format. */
    lxw_format *red_format = workbook_add_format(workbook);
    format_set_underline (red_format, LXW_UNDERLINE_SINGLE);
    format_set_font_color(red_format, LXW_COLOR_RED);

    /* Widen the first column to make the text clearer. */
    worksheet_set_column(worksheet, 0, 0, 30, NULL);

    /* Write a hyperlink. A default blue underline will be used if the format is NULL. */
    worksheet_write_url(worksheet,    0, 0, "http://libxlsxwriter.github.io", NULL);

    /* Write a hyperlink but overwrite the displayed string. Note, we need to
     * specify the format for the string to match the default hyperlink. */
    worksheet_write_url   (worksheet, 2, 0, "http://libxlsxwriter.github.io", NULL);
    worksheet_write_string(worksheet, 2, 0, "Read the documentation.",        url_format);

    /* Write a hyperlink with a different format. */
    worksheet_write_url(worksheet,    4, 0, "http://libxlsxwriter.github.io", red_format);

    /* Write a mail hyperlink. */
    worksheet_write_url   (worksheet, 6, 0, "mailto:jmcnamara@cpan.org",      NULL);

    /* Write a mail hyperlink and overwrite the displayed string. We again
     * specify the format for the string to match the default hyperlink. */
    worksheet_write_url   (worksheet, 8, 0, "mailto:jmcnamara@cpan.org",      NULL);
    worksheet_write_string(worksheet, 8, 0, "Drop me a line.",                url_format);


    /* Close the workbook, save the file and free any memory. */
    workbook_close(workbook);

    return 0;
}
