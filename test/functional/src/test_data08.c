/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test case for cell merging.
 *
 * Copyright 2015, Brad Ediger, brad.ediger@madriska.com
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = new_workbook("test_data08.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Merged cell", NULL);
    worksheet_write_string(worksheet, 0, 3, "Another", NULL);
    worksheet_merge_range(worksheet, 0, 0, 1, 2);

    return workbook_close(workbook);
}
