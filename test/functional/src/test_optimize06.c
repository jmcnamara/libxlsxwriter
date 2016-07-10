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

    lxw_workbook_options options = {1, NULL};

    lxw_workbook  *workbook  = workbook_new_opt("test_optimize06.xlsx", &options);

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    uint8_t i;
    char c[] = {0x00, 0x00};

    worksheet_write_string(worksheet, 0, 0, "_x0000_", NULL);

    for (i = 1; i <= 127; i++) {
        (*c)++;
        if (i != 34) {
            worksheet_write_string(worksheet, i, 0, c, NULL);

        }
    }

    return workbook_close(workbook);
}
