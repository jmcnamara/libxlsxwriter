/*
 * Simple C++ program to test compilation in includes with libxlsxwriter.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */


#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("hello_world.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
    worksheet_write_number(worksheet, 1, 0, 123, NULL);

    workbook_close(workbook);

    return 0;
}
