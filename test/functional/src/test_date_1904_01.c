/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main()
{
    lxw_workbook *workbook = workbook_new("test_date_1904_01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format = workbook_add_format(workbook);
    format_set_num_format_index(format, 14);

    lxw_datetime datetime1 = {1900,  1,  1, 0, 0, 0.0};
    lxw_datetime datetime2 = {1902,  9, 26, 0, 0, 0.0};
    lxw_datetime datetime3 = {1913,  9,  8, 0, 0, 0.0};
    lxw_datetime datetime4 = {1927,  5, 18, 0, 0, 0.0};
    lxw_datetime datetime5 = {2173, 10, 14, 0, 0, 0.0};
    lxw_datetime datetime6 = {4637, 11, 26, 0, 0, 0.0};

    worksheet_set_column(worksheet, 0, 0, 12, NULL);

    worksheet_write_datetime(worksheet, CELL("A1"), &datetime1, format);
    worksheet_write_datetime(worksheet, CELL("A2"), &datetime2, format);
    worksheet_write_datetime(worksheet, CELL("A3"), &datetime3, format);
    worksheet_write_datetime(worksheet, CELL("A4"), &datetime4, format);
    worksheet_write_datetime(worksheet, CELL("A5"), &datetime5, format);
    worksheet_write_datetime(worksheet, CELL("A6"), &datetime6, format);

    return workbook_close(workbook);
}
