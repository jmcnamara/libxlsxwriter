/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case to test data writing.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"


int main() {

    lxw_workbook  *workbook  = workbook_new("test_format52.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_format    *format;

    double value = 123.456;

    worksheet_set_column(worksheet, 0, 0, 12, NULL);

    format = workbook_add_format(workbook);
    format_set_num_format(format, "0.0");
    worksheet_write_number(worksheet, 0, 0, value, format);

    format = workbook_add_format(workbook);
    format_set_num_format(format, "0.000");
    worksheet_write_number(worksheet, 1, 0, value, format);

    format = workbook_add_format(workbook);
    format_set_num_format(format, "0.0000");
    worksheet_write_number(worksheet, 2, 0, value, format);

    format = workbook_add_format(workbook);
    format_set_num_format(format, "0.00000");
    worksheet_write_number(worksheet, 3, 0, value, format);

    format = workbook_add_format(workbook);
    format_set_num_format(format, "0.0");
    format_set_bold(format);
    worksheet_write_number(worksheet, 4, 0, value, format);

    format = workbook_add_format(workbook);
    format_set_num_format(format, "0.000");
    format_set_bold(format);
    worksheet_write_number(worksheet, 5, 0, value, format);

    format = workbook_add_format(workbook);
    format_set_num_format(format, "0.0000");
    format_set_bold(format);
    worksheet_write_number(worksheet, 6, 0, value, format);

    format = workbook_add_format(workbook);
    format_set_num_format(format, "0.00000");
    format_set_bold(format);
    worksheet_write_number(worksheet, 7, 0, value, format);

    return workbook_close(workbook);
}
