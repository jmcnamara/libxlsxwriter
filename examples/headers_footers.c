/*
 * This program shows several examples of how to set up headers and
 * footers with libxlsxwriter.
 *
 * The control characters used in the header/footer strings are:
 *
 *     Control             Category            Description
 *     =======             ========            ===========
 *     &L                  Justification       Left
 *     &C                                      Center
 *     &R                                      Right
 *
 *     &P                  Information         Page number
 *     &N                                      Total number of pages
 *     &D                                      Date
 *     &T                                      Time
 *     &F                                      File name
 *     &A                                      Worksheet name
 *
 *     &fontsize           Font                Font size
 *     &"font,style"                           Font name and style
 *     &U                                      Single underline
 *     &E                                      Double underline
 *     &S                                      Strikethrough
 *     &X                                      Superscript
 *     &Y                                      Subscript
 *
 *     &[Picture]          Images              Image placeholder
 *     &G                                      Same as &[Picture]
 *
 *     &&                  Miscellaneous       Literal ampersand &
 *
 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"


int main() {

    lxw_workbook *workbook  = workbook_new("headers_footers.xlsx");

    char preview[] = "Select Print Preview to see the header and footer";

    /*
     * A simple example to start
     */
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, "Simple");
    char header1[] = "&CHere is some centered text.";
    char footer1[] = "&LHere is some left aligned text.";

    worksheet_set_header(worksheet1, header1);
    worksheet_set_footer(worksheet1, footer1);

    worksheet_set_column(worksheet1, 0, 0, 50, NULL);
    worksheet_write_string(worksheet1, 0, 0, preview, NULL);


    /*
     * A simple example to start
     */
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, "Image");
    lxw_header_footer_options header_options = {.image_left = "logo_small.png"};

    worksheet_set_header_opt(worksheet2, "&L&[Picture]", &header_options);

    worksheet_set_margins(worksheet2, -1, -1, 1.3, -1);
    worksheet_set_column(worksheet2, 0, 0, 50, NULL);
    worksheet_write_string(worksheet2, 0, 0, preview, NULL);


    /*
     * This is an example of some of the header/footer variables.
     */
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, "Variables");
    char header3[] = "&LPage &P of &N" "&CFilename: &F" "&RSheetname: &A";
    char footer3[] = "&LCurrent date: &D" "&RCurrent time: &T";
    lxw_row_t breaks[] = {20, 0};

    worksheet_set_header(worksheet3, header3);
    worksheet_set_footer(worksheet3, footer3);

    worksheet_set_column(worksheet3, 0, 0, 50, NULL);
    worksheet_write_string(worksheet3, 0, 0, preview, NULL);

    worksheet_set_h_pagebreaks(worksheet3, breaks);
    worksheet_write_string(worksheet3, 20, 0, "Next page", NULL);


    /*
     * This example shows how to use more than one font.
     */
    lxw_worksheet *worksheet4 = workbook_add_worksheet(workbook, "Mixed fonts");
    char header4[] = "&C&\"Courier New,Bold\"Hello &\"Arial,Italic\"World";
    char footer4[] = "&C&\"Symbol\"e&\"Arial\" = mc&X2";

    worksheet_set_header(worksheet4, header4);
    worksheet_set_footer(worksheet4, footer4);

    worksheet_set_column(worksheet4, 0, 0, 50, NULL);
    worksheet_write_string(worksheet4, 0, 0, preview, NULL);


    /*
     * Example of line wrapping.
     */
    lxw_worksheet *worksheet5 = workbook_add_worksheet(workbook, "Word wrap");
    char header5[] = "&CHeading 1\nHeading 2";

    worksheet_set_header(worksheet5, header5);

    worksheet_set_column(worksheet5, 0, 0, 50, NULL);
    worksheet_write_string(worksheet5, 0, 0, preview, NULL);


    /*
     * Example of inserting a literal ampersand &
     */
    lxw_worksheet *worksheet6 = workbook_add_worksheet(workbook, "Ampersand");
    char header6[] = "&CCuriouser && Curiouser - Attorneys at Law";

    worksheet_set_header(worksheet6, header6);

    worksheet_set_column(worksheet6, 0, 0, 50, NULL);
    worksheet_write_string(worksheet6, 0, 0, preview, NULL);


    workbook_close(workbook);

    return 0;
}
