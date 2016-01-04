/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_gh42_02.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    char string[] = {0xe5, 0x9b, 0xbe, 0x20, 0xe5, 0x9b, 0xbe, 0x00};

    worksheet_write_string(worksheet, 0, 0, string, NULL);

    workbook_close(workbook);

    return 0;
}

