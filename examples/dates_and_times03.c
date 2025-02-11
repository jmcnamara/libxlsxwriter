/*
 * Example of writing dates and times in Excel using a Unix datetime and date
 * formatting.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    /* Create a new workbook and add a worksheet. */
    lxw_workbook  *workbook  = workbook_new("date_and_times03.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Add a format with date formatting. */
    lxw_format    *format    = workbook_add_format(workbook);
    format_set_num_format(format, "mmm d yyyy hh:mm AM/PM");

    /* Widen the first column to make the text clearer. */
    worksheet_set_column(worksheet, 0, 0, 20, NULL);

    /* Write some Unix datetimes with formatting. */

    /* 1970-01-01. The Unix epoch. */
    worksheet_write_unixtime(worksheet, 0, 0,             0, format);

    /* 2000-01-01. */
    worksheet_write_unixtime(worksheet, 1, 0,  1577836800LL, format);

    /* 1900-01-01. */
    worksheet_write_unixtime(worksheet, 2, 0, -2208988800LL, format);

    return workbook_close(workbook);
}
