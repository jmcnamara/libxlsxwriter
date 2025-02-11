/*
 * An example of how to use libxlsxwriter to write functions that create
 * dynamic arrays. These functions are new to Excel 365. The examples mirror
 * the examples in the Excel documentation on these functions.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"


void write_worksheet_data(lxw_worksheet *worksheet, lxw_format *header);


int main() {

    lxw_workbook  *workbook    = workbook_new("dynamic_arrays.xlsx");
    lxw_worksheet *worksheet1  = workbook_add_worksheet(workbook, "Filter");
    lxw_worksheet *worksheet2  = workbook_add_worksheet(workbook, "Unique");
    lxw_worksheet *worksheet3  = workbook_add_worksheet(workbook, "Sort");
    lxw_worksheet *worksheet4  = workbook_add_worksheet(workbook, "Sortby");
    lxw_worksheet *worksheet5  = workbook_add_worksheet(workbook, "Xlookup");
    lxw_worksheet *worksheet6  = workbook_add_worksheet(workbook, "Xmatch");
    lxw_worksheet *worksheet7  = workbook_add_worksheet(workbook, "Randarray");
    lxw_worksheet *worksheet8  = workbook_add_worksheet(workbook, "Sequence");
    lxw_worksheet *worksheet9  = workbook_add_worksheet(workbook, "Spill ranges");
    lxw_worksheet *worksheet10 = workbook_add_worksheet(workbook, "Older functions");

    lxw_format *header1 = workbook_add_format(workbook);
    format_set_bg_color(header1, 0x74AC4C);
    format_set_font_color(header1, 0xFFFFFF);

    lxw_format *header2 = workbook_add_format(workbook);
    format_set_bg_color(header2, 0x528FD3);
    format_set_font_color(header2, 0xFFFFFF);


    /*
     * Example of using the FILTER() function.
     */
    worksheet_write_dynamic_formula(worksheet1, CELL("F2"),
                                    "=_xlfn._xlws.FILTER(A1:D17,C1:C17=K2)",
                                    NULL);

    /* Write the data the function will work on. */
    worksheet_write_string(worksheet1, CELL("K1"), "Product",   header2);
    worksheet_write_string(worksheet1, CELL("K2"), "Apple",     NULL   );
    worksheet_write_string(worksheet1, CELL("F1"), "Region",    header2);
    worksheet_write_string(worksheet1, CELL("G1"), "Sales Rep", header2);
    worksheet_write_string(worksheet1, CELL("H1"), "Product",   header2);
    worksheet_write_string(worksheet1, CELL("I1"), "Units",     header2);

    write_worksheet_data(worksheet1, header1);
    worksheet_set_column_pixels(worksheet1, COLS("E:E"), 20, NULL);
    worksheet_set_column_pixels(worksheet1, COLS("J:J"), 20, NULL);


    /*
     * Example of using the UNIQUE() function.
     */
    worksheet_write_dynamic_formula(worksheet2, CELL("F2"),
                                    "=_xlfn.UNIQUE(B2:B17)",
                                    NULL);

    /* A more complex example combining SORT and UNIQUE. */
    worksheet_write_dynamic_formula(worksheet2, CELL("H2"),
                                    "=_xlfn._xlws.SORT(_xlfn.UNIQUE(B2:B17))",
                                    NULL);

    /* Write the data the function will work on. */
    worksheet_write_string(worksheet2, CELL("F1"), "Sales Rep", header2);
    worksheet_write_string(worksheet2, CELL("H1"), "Sales Rep", header2);

    write_worksheet_data(worksheet2, header1);
    worksheet_set_column_pixels(worksheet2, COLS("E:E"), 20, NULL);
    worksheet_set_column_pixels(worksheet2, COLS("G:G"), 20, NULL);


    /*
     * Example of using the SORT() function.
     */
    worksheet_write_dynamic_formula(worksheet3, CELL("F2"),
                                    "=_xlfn._xlws.SORT(B2:B17)",
                                    NULL);

    /* A more complex example combining SORT and FILTER. */
    worksheet_write_dynamic_formula(worksheet3, CELL("H2"),
                                    "=_xlfn._xlws.SORT(_xlfn._xlws.FILTER(C2:D17,D2:D17>5000,\"\"),2,1)",
                                    NULL);

    /* Write the data the function will work on. */
    worksheet_write_string(worksheet3, CELL("F1"), "Sales Rep", header2);
    worksheet_write_string(worksheet3, CELL("H1"), "Product",   header2);
    worksheet_write_string(worksheet3, CELL("I1"), "Units",     header2);

    write_worksheet_data(worksheet3, header1);
    worksheet_set_column_pixels(worksheet3, COLS("E:E"), 20, NULL);
    worksheet_set_column_pixels(worksheet3, COLS("G:G"), 20, NULL);


    /*
     * Example of using the SORTBY() function.
     */
    worksheet_write_dynamic_formula(worksheet4, CELL("D2"),
                                    "=_xlfn.SORTBY(A2:B9,B2:B9)",
                                    NULL);

    /* Write the data the function will work on. */
    worksheet_write_string(worksheet4, CELL("A1"), "Name", header1);
    worksheet_write_string(worksheet4, CELL("B1"), "Age",  header1);

    worksheet_write_string(worksheet4, CELL("A2"), "Tom",    NULL);
    worksheet_write_string(worksheet4, CELL("A3"), "Fred",   NULL);
    worksheet_write_string(worksheet4, CELL("A4"), "Amy",    NULL);
    worksheet_write_string(worksheet4, CELL("A5"), "Sal",    NULL);
    worksheet_write_string(worksheet4, CELL("A6"), "Fritz",  NULL);
    worksheet_write_string(worksheet4, CELL("A7"), "Srivan", NULL);
    worksheet_write_string(worksheet4, CELL("A8"), "Xi",     NULL);
    worksheet_write_string(worksheet4, CELL("A9"), "Hector", NULL);

    worksheet_write_number(worksheet4, CELL("B2"), 52, NULL);
    worksheet_write_number(worksheet4, CELL("B3"), 65, NULL);
    worksheet_write_number(worksheet4, CELL("B4"), 22, NULL);
    worksheet_write_number(worksheet4, CELL("B5"), 73, NULL);
    worksheet_write_number(worksheet4, CELL("B6"), 19, NULL);
    worksheet_write_number(worksheet4, CELL("B7"), 39, NULL);
    worksheet_write_number(worksheet4, CELL("B8"), 19, NULL);
    worksheet_write_number(worksheet4, CELL("B9"), 66, NULL);

    worksheet_write_string(worksheet4, CELL("D1"), "Name", header2);
    worksheet_write_string(worksheet4, CELL("E1"), "Age",  header2);

    worksheet_set_column_pixels(worksheet4, COLS("C:C"), 20, NULL);


    /*
     * Example of using the XLOOKUP() function.
     */
    worksheet_write_dynamic_formula(worksheet5, CELL("F1"),
                                    "=_xlfn.XLOOKUP(E1,A2:A9,C2:C9)",
                                    NULL);

    /* Write the data the function will work on. */
    worksheet_write_string(worksheet5, CELL("A1"), "Country", header1);
    worksheet_write_string(worksheet5, CELL("B1"), "Abr",     header1);
    worksheet_write_string(worksheet5, CELL("C1"), "Prefix",  header1);

    worksheet_write_string(worksheet5, CELL("A2"), "China",         NULL);
    worksheet_write_string(worksheet5, CELL("A3"), "India",         NULL);
    worksheet_write_string(worksheet5, CELL("A4"), "United States", NULL);
    worksheet_write_string(worksheet5, CELL("A5"), "Indonesia",     NULL);
    worksheet_write_string(worksheet5, CELL("A6"), "Brazil",        NULL);
    worksheet_write_string(worksheet5, CELL("A7"), "Pakistan",      NULL);
    worksheet_write_string(worksheet5, CELL("A8"), "Nigeria",       NULL);
    worksheet_write_string(worksheet5, CELL("A9"), "Bangladesh",    NULL);

    worksheet_write_string(worksheet5, CELL("B2"), "CN", NULL);
    worksheet_write_string(worksheet5, CELL("B3"), "IN", NULL);
    worksheet_write_string(worksheet5, CELL("B4"), "US", NULL);
    worksheet_write_string(worksheet5, CELL("B5"), "ID", NULL);
    worksheet_write_string(worksheet5, CELL("B6"), "BR", NULL);
    worksheet_write_string(worksheet5, CELL("B7"), "PK", NULL);
    worksheet_write_string(worksheet5, CELL("B8"), "NG", NULL);
    worksheet_write_string(worksheet5, CELL("B9"), "BD", NULL);

    worksheet_write_number(worksheet5, CELL("C2"), 86,  NULL);
    worksheet_write_number(worksheet5, CELL("C3"), 91,  NULL);
    worksheet_write_number(worksheet5, CELL("C4"), 1,   NULL);
    worksheet_write_number(worksheet5, CELL("C5"), 62,  NULL);
    worksheet_write_number(worksheet5, CELL("C6"), 55,  NULL);
    worksheet_write_number(worksheet5, CELL("C7"), 92,  NULL);
    worksheet_write_number(worksheet5, CELL("C8"), 234, NULL);
    worksheet_write_number(worksheet5, CELL("C9"), 880, NULL);

    worksheet_write_string(worksheet5, CELL("E1"), "Brazil", header2);

    worksheet_set_column_pixels(worksheet5, COLS("A:A"), 100, NULL);
    worksheet_set_column_pixels(worksheet5, COLS("D:D"), 20,  NULL);


    /*
     * Example of using the XMATCH() function.
     */
    worksheet_write_dynamic_formula(worksheet6, CELL("D2"),
                                    "=_xlfn.XMATCH(C2,A2:A6)",
                                    NULL);

    /* Write the data the function will work on. */
    worksheet_write_string(worksheet6, CELL("A1"), "Product", header1);

    worksheet_write_string(worksheet6, CELL("A2"), "Apple",  NULL);
    worksheet_write_string(worksheet6, CELL("A3"), "Grape",  NULL);
    worksheet_write_string(worksheet6, CELL("A4"), "Pear",   NULL);
    worksheet_write_string(worksheet6, CELL("A5"), "Banana", NULL);
    worksheet_write_string(worksheet6, CELL("A6"), "Cherry", NULL);

    worksheet_write_string(worksheet6, CELL("C1"), "Product",  header2);
    worksheet_write_string(worksheet6, CELL("D1"), "Position", header2);
    worksheet_write_string(worksheet6, CELL("C2"), "Grape",    NULL);

    worksheet_set_column_pixels(worksheet6, COLS("B:B"), 20,  NULL);


    /*
     * Example of using the RANDARRAY() function.
     */
    worksheet_write_dynamic_formula(worksheet7, CELL("A1"),
                                    "=_xlfn.RANDARRAY(5,3,1,100, TRUE)",
                                    NULL);

    /*
     * Example of using the SEQUENCE() function.
     */
    worksheet_write_dynamic_formula(worksheet8, CELL("A1"),
                                    "=_xlfn.SEQUENCE(4,5)",
                                    NULL);


    /*
     * Example of using the Spill range operator.
     */
    worksheet_write_dynamic_formula(worksheet9, CELL("H2"),
                                    "=_xlfn.ANCHORARRAY(F2)",
                                    NULL);

    worksheet_write_dynamic_formula(worksheet9, CELL("J2"),
                                    "=COUNTA(_xlfn.ANCHORARRAY(F2))",
                                    NULL);

    /* Write the data the function will work on. */
    worksheet_write_dynamic_formula(worksheet9, CELL("F2"),
                                    "=_xlfn.UNIQUE(B2:B17)",
                                    NULL);

    worksheet_write_string(worksheet9, CELL("F1"), "Unique", header2);
    worksheet_write_string(worksheet9, CELL("H1"), "Spill",  header2);
    worksheet_write_string(worksheet9, CELL("J1"), "Spill",  header2);

    write_worksheet_data(worksheet9, header1);
    worksheet_set_column_pixels(worksheet9, COLS("E:E"), 20,  NULL);
    worksheet_set_column_pixels(worksheet9, COLS("G:G"), 20,  NULL);
    worksheet_set_column_pixels(worksheet9, COLS("I:I"), 20,  NULL);


    /*
     * Example of using dynamic ranges with older Excel functions.
     */
    worksheet_write_dynamic_array_formula(worksheet10, RANGE("B1:B3"),
                                          "=LEN(A1:A3)",
                                          NULL);

    /* Write the data the function will work on. */
    worksheet_write_string(worksheet10, CELL("A1"), "Foo",    NULL);
    worksheet_write_string(worksheet10, CELL("A2"), "Food",   NULL);
    worksheet_write_string(worksheet10, CELL("A3"), "Frood",  NULL);


    return workbook_close(workbook);
}


/* A simple function and data structure to populate some of the worksheets. */
struct worksheet_data {
    char col1[10];
    char col2[10];
    char col3[10];
    int  col4;
};

void write_worksheet_data(lxw_worksheet *worksheet, lxw_format *header) {

    struct worksheet_data data[160] = {
        {"East",  "Tom",    "Apple",  6380},
        {"West",  "Fred",   "Grape",  5619},
        {"North", "Amy",    "Pear",   4565},
        {"South", "Sal",    "Banana", 5323},
        {"East",  "Fritz",  "Apple",  4394},
        {"West",  "Sravan", "Grape",  7195},
        {"North", "Xi",     "Pear",   5231},
        {"South", "Hector", "Banana", 2427},
        {"East",  "Tom",    "Banana", 4213},
        {"West",  "Fred",   "Pear",   3239},
        {"North", "Amy",    "Grape",  6520},
        {"South", "Sal",    "Apple",  1310},
        {"East",  "Fritz",  "Banana", 6274},
        {"West",  "Sravan", "Pear",   4894},
        {"North", "Xi",     "Grape",  7580},
        {"South", "Hector", "Apple",  9814},
    };

    worksheet_write_string(worksheet, CELL("A1"), "Region",    header);
    worksheet_write_string(worksheet, CELL("B1"), "Sales Rep", header);
    worksheet_write_string(worksheet, CELL("C1"), "Product",   header);
    worksheet_write_string(worksheet, CELL("D1"), "Units",     header);

    for (int row = 0; row < 16; row++) {
        worksheet_write_string(worksheet, row + 1, 0, data[row].col1, NULL);
        worksheet_write_string(worksheet, row + 1, 1, data[row].col2, NULL);
        worksheet_write_string(worksheet, row + 1, 2, data[row].col3, NULL);
        worksheet_write_number(worksheet, row + 1, 3, data[row].col4, NULL);
    }
}
