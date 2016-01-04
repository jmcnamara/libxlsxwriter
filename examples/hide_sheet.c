/*
 * Example of how to hide a worksheet using libxlsxwriter.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook   = workbook_new("hide_sheet.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);

    /* Hide Sheet2. It won't be visible until it is unhidden in Excel. */
    worksheet_hide(worksheet2);

    worksheet_write_string(worksheet1, 0, 0, "Sheet2 is hidden", NULL);
    worksheet_write_string(worksheet2, 0, 0, "Now it's my turn to find you!", NULL);
    worksheet_write_string(worksheet3, 0, 0, "Sheet2 is hidden", NULL);

    /* Make the first column wider to make the text clearer. */
    worksheet_set_column(worksheet1, 0, 0, 30, NULL);
    worksheet_set_column(worksheet2, 0, 0, 30, NULL);
    worksheet_set_column(worksheet3, 0, 0, 30, NULL);

    workbook_close(workbook);

    return 0;
}
