/*
 * An example of writing cell comments to a worksheet using libxlsxwriter.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("comments1.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string( worksheet, 0, 0, "Hello" , NULL);

    worksheet_write_comment(worksheet, 0, 0, "This is a comment");

    return workbook_close(workbook);
}
