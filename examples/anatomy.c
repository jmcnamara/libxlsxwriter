/*
 * Anatomy of a simple libxlsxwriter program.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    /* Create a new workbook. */
    lxw_workbook  *workbook   = workbook_new("anatomy.xlsx");

    /* Add a worksheet with a user defined sheet name. */
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, "Demo");

    /* Add a worksheet with Excel's default sheet name: Sheet2. */
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);

    /* Add some cell formats. */
    lxw_format  *myformat1    = workbook_add_format(workbook);
    lxw_format  *myformat2    = workbook_add_format(workbook);

    /* Set the bold property for the first format. */
    format_set_bold(myformat1);

    /* Set a number format for the second format. */
    format_set_num_format(myformat2, "$#,##0.00");

    /* Widen the first column to make the text clearer. */
    worksheet_set_column(worksheet1, 0, 0, 20, NULL);

    /* Write some unformatted data. */
    worksheet_write_string(worksheet1, 0, 0, "Peach", NULL);
    worksheet_write_string(worksheet1, 1, 0, "Plum",  NULL);

    /* Write formatted data. */
    worksheet_write_string(worksheet1, 2, 0, "Pear",  myformat1);

    /* Formats can be reused. */
    worksheet_write_string(worksheet1, 3, 0, "Persimmon",  myformat1);


    /* Write some numbers. */
    worksheet_write_number(worksheet1, 5, 0, 123,       NULL);
    worksheet_write_number(worksheet1, 6, 0, 4567.555,  myformat2);


    /* Write to the second worksheet. */
    worksheet_write_string(worksheet2, 0, 0, "Some text", myformat1);


    /* Close the workbook, save the file and free any memory. */
    workbook_close(workbook);

    return 0;
}
