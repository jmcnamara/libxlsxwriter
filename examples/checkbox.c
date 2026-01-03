/*
 * An example of inserting checkboxes into a worksheet using the
 * libxlsxwriter library.
 *
 * Checkboxes in Excel are a feature of Excel 365 versions from 2024 onwards.
 * They are boolean values displayed as checkboxes in the cells.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("checkbox.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Widen the column for clarity. */
    worksheet_set_column(worksheet, 0, 0, 15, NULL);

    /* Write some checkboxes with different states. */
    worksheet_insert_checkbox(worksheet, 0, 0, LXW_FALSE);
    worksheet_insert_checkbox(worksheet, 1, 0, LXW_TRUE);
    worksheet_insert_checkbox(worksheet, 2, 0, LXW_FALSE);
    worksheet_insert_checkbox(worksheet, 3, 0, LXW_TRUE);
    worksheet_insert_checkbox(worksheet, 4, 0, LXW_TRUE);

    workbook_close(workbook);

    return 0;
}
