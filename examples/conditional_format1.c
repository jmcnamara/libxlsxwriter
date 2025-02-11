/*
 * An a simple example of how to add conditional formatting to an
 * libxlsxwriter file.
 *
 * See conditional_format.c for a more comprehensive example.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"


int main() {

    lxw_workbook  *workbook  = workbook_new("conditional_format_simple.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Write some sample data. */
    worksheet_write_number(worksheet, CELL("B1"), 34, NULL);
    worksheet_write_number(worksheet, CELL("B2"), 32, NULL);
    worksheet_write_number(worksheet, CELL("B3"), 31, NULL);
    worksheet_write_number(worksheet, CELL("B4"), 35, NULL);
    worksheet_write_number(worksheet, CELL("B5"), 36, NULL);
    worksheet_write_number(worksheet, CELL("B6"), 30, NULL);
    worksheet_write_number(worksheet, CELL("B7"), 38, NULL);
    worksheet_write_number(worksheet, CELL("B8"), 38, NULL);
    worksheet_write_number(worksheet, CELL("B9"), 32, NULL);

    /* Add a format with red text. */
    lxw_format *custom_format = workbook_add_format(workbook);
    format_set_font_color(custom_format, LXW_COLOR_RED);

    /* Create a conditional format object. A static object would also work. */
    lxw_conditional_format *conditional_format =
        (lxw_conditional_format *)calloc(1, sizeof(lxw_conditional_format));

    /* Set the format type: a cell conditional: */
    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;

    /* Set the criteria to use: */
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_LESS_THAN;

    /* Set the value to which the criteria will be applied: */
    conditional_format->value    = 33;

    /* Set the format to use if the criteria/value applies: */
    conditional_format->format   = custom_format;

    /* Now apply the format to data range. */
    worksheet_conditional_format_range(worksheet, RANGE("B1:B9"), conditional_format);

    /* Free the object and close the file. */
    free(conditional_format);
    return workbook_close(workbook);
}
