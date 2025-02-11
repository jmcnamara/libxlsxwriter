/*
 * An example of setting a worksheet background image with libxlsxwriter.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("background.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_background(worksheet, "logo.png");

    workbook_close(workbook);

    return 0;
}
