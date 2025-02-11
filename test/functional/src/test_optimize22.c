/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test case for writing data in optimization mode.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook_options options = {LXW_TRUE, NULL, LXW_FALSE, NULL, NULL};

    lxw_workbook  *workbook  = workbook_new_opt("test_optimize22.xlsx", &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_format    *bold      = workbook_add_format(workbook);

    format_set_bold(bold);

    worksheet_set_column(worksheet, 0, 0, 36, bold);

    return workbook_close(workbook);
}
