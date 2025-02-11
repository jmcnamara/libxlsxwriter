/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_format24.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    lxw_format *format1 = workbook_add_format(workbook);

    /*
        'rotation': 270,
        'indent': 1,
        'align': "center",
        'valign': "top"
    */


    format_set_rotation(format1, 270);
    format_set_indent(format1, 1);
    format_set_align(format1, LXW_ALIGN_CENTER);
    format_set_align(format1, LXW_ALIGN_VERTICAL_TOP);

    worksheet_set_row(worksheet, 0, 75, NULL);

    worksheet_write_string(worksheet, 0, 0, "ABCD", format1);

    return workbook_close(workbook);
}
