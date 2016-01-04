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

    lxw_workbook  *workbook  = workbook_new("test_hyperlink04.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, "Data Sheet");

    (void)worksheet2;
    (void)worksheet3;

    worksheet_write_url_opt(worksheet1, CELL("A1"),  "internal:Sheet2!A1",       NULL, NULL,        NULL);
    worksheet_write_url_opt(worksheet1, CELL("A3"),  "internal:Sheet2!A1:A5",    NULL, NULL,        NULL);
    worksheet_write_url_opt(worksheet1, CELL("A5"),  "internal:'Data Sheet'!D5", NULL, "Some text", NULL);
    worksheet_write_url_opt(worksheet1, CELL("E12"), "internal:Sheet1!J1",       NULL, NULL,        NULL);
    worksheet_write_url_opt(worksheet1, CELL("G17"), "internal:Sheet2!A1",       NULL, "Some text", NULL);
    worksheet_write_url_opt(worksheet1, CELL("A18"), "internal:Sheet2!A1",       NULL, NULL,        "Tool Tip 1");
    worksheet_write_url_opt(worksheet1, CELL("A20"), "internal:Sheet2!A1",       NULL, "More text", "Tool Tip 2");

    return workbook_close(workbook);
}
