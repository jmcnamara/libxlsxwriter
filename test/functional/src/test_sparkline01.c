/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_sparkline01.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Write data for the sparkline. */
    worksheet_write_number(worksheet, 0, 0, -2, NULL);
    worksheet_write_number(worksheet, 0, 1, 2, NULL);
    worksheet_write_number(worksheet, 0, 2, 3, NULL);
    worksheet_write_number(worksheet, 0, 3, -1, NULL);
    worksheet_write_number(worksheet, 0, 4, 0, NULL);

    /* Add a line sparkline. */
    lxw_sparkline_options options = {
        .range = "Sheet1!A1:E1",
    };

    worksheet_add_sparkline(worksheet, 0, 5, &options);

    return workbook_close(workbook);
}
