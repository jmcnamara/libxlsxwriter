/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_table51.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format1 = workbook_add_format(workbook);
    lxw_format *format2 = workbook_add_format(workbook);
    lxw_format *format3 = workbook_add_format(workbook);

    format_set_num_format(format1, "0.00;[Red]0.00");
    format_set_num_format(format2, "0.00_ ;\\-0.00\\ ");
    format_set_num_format(format3, "0.00_ ;[Red]\\-0.00\\ ");

    /* We manually set the indices to get the same order as the target file. */
    lxw_format_get_dxf_index(format3);
    lxw_format_get_dxf_index(format2);
    lxw_format_get_dxf_index(format1);

    worksheet_set_column(worksheet, COLS("C:F"), 10.288, NULL);

    /* Write the number data without formats before adding the table, to check
     * that the table column formats are applied to the existing cells. */
    worksheet_write_number(worksheet, 2, 3, 1234, NULL);
    worksheet_write_number(worksheet, 3, 3, 1256, NULL);
    worksheet_write_number(worksheet, 4, 3, 2234, NULL);
    worksheet_write_number(worksheet, 5, 3, 1324, NULL);

    worksheet_write_number(worksheet, 2, 4, 2000, NULL);
    worksheet_write_number(worksheet, 3, 4, 4000, NULL);
    worksheet_write_number(worksheet, 4, 4, 3000, NULL);
    worksheet_write_number(worksheet, 5, 4, 1000, NULL);

    worksheet_write_number(worksheet, 2, 5, 4321, NULL);
    worksheet_write_number(worksheet, 3, 5, 4320, NULL);
    worksheet_write_number(worksheet, 4, 5, 4332, NULL);
    worksheet_write_number(worksheet, 5, 5, 4333, NULL);

    lxw_table_column col1  = {0};
    lxw_table_column col2  = {.format = format1};
    lxw_table_column col3  = {.format = format2};
    lxw_table_column col4  = {.format = format3};
    lxw_table_column *columns[] = {&col1, &col2, &col3, &col4, NULL};

    lxw_table_options options = {.columns = columns};

    worksheet_add_table(worksheet, RANGE("C2:F6"), &options);

    /* The strings are written after the table so that the shared string table
     * order matches the Excel file. */
    worksheet_write_string(worksheet, 2, 2, "Foo", NULL);
    worksheet_write_string(worksheet, 3, 2, "Bar", NULL);
    worksheet_write_string(worksheet, 4, 2, "Baz", NULL);
    worksheet_write_string(worksheet, 5, 2, "Bop", NULL);

    return workbook_close(workbook);
}
