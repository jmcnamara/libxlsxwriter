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
 *     &P                  Information         Page number
 *     &N                                      Total number of pages
 *     &D                                      Date
 *     &T                                      Time
 *     &F                                      File name
 *     &A                                      Worksheet name
 *     &fontsize           Font                Font size
 *     &"font,style"                           Font name and style
 *     &U                                      Single underline
 *     &E                                      Double underline
 *     &S                                      Strikethrough
 *     &X                                      Superscript
 *     &Y                                      Subscript
 *     &[Picture]          Images              Image placeholder
 *     &G                                      Same as &[Picture]
 *     &&                  Miscellaneous       Literal ampersand &
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
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
    char header1[] = "&CHere is some centred text.";
    char footer1[] = "&LHere is some left aligned text.";

    worksheet_set_header(worksheet1, header1);
    worksheet_set_footer(worksheet1, footer1);

    worksheet_set_column(worksheet1, 0, 0, 50, NULL);
    worksheet_write_string(worksheet1, 0, 0, preview, NULL);


    /*
     * This is an example of some of the header/footer variables.
     */
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, "Variables");
    char header2[] = "&LPage &P of &N" "&CFilename: &F" "&RSheetname: &A";
    char footer2[] = "&LCurrent date: &D" "&RCurrent time: &T";
    lxw_row_t breaks[] = {20, 0};

    worksheet_set_header(worksheet2, header2);
    worksheet_set_footer(worksheet2, footer2);

    worksheet_set_column(worksheet2, 0, 0, 50, NULL);
    worksheet_write_string(worksheet2, 0, 0, preview, NULL);

    worksheet_set_h_pagebreaks(worksheet2, breaks);
    worksheet_write_string(worksheet2, 20, 0, "Next page", NULL);


    /*
     * This example shows how to use more than one font.
     */
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, "Mixed fonts");
    char header3[] = "&C&\"Courier New,Bold\"Hello &\"Arial,Italic\"World";
    char footer3[] = "&C&\"Symbol\"e&\"Arial\" = mc&X2";

    worksheet_set_header(worksheet3, header3);
    worksheet_set_footer(worksheet3, footer3);

    worksheet_set_column(worksheet3, 0, 0, 50, NULL);
    worksheet_write_string(worksheet3, 0, 0, preview, NULL);


    /*
     * Example of line wrapping.
     */
    lxw_worksheet *worksheet4 = workbook_add_worksheet(workbook, "Word wrap");
    char header4[] = "&CHeading 1\nHeading 2";

    worksheet_set_header(worksheet4, header4);

    worksheet_set_column(worksheet4, 0, 0, 50, NULL);
    worksheet_write_string(worksheet4, 0, 0, preview, NULL);


    /*
     * Example of inserting a literal ampersand &
     */
    lxw_worksheet *worksheet5 = workbook_add_worksheet(workbook, "Ampersand");
    char header5[] = "&CCuriouser && Curiouser - Attorneys at Law";

    worksheet_set_header(worksheet5, header5);

    worksheet_set_column(worksheet5, 0, 0, 50, NULL);
    worksheet_write_string(worksheet5, 0, 0, preview, NULL);


    workbook_close(workbook);

    return 0;
}
