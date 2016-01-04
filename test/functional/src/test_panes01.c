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

    lxw_workbook  *workbook  = workbook_new("test_panes01.xlsx");
    lxw_worksheet *worksheet01 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet02 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet03 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet04 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet05 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet06 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet07 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet08 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet09 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet10 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet11 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet12 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet13 = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet01, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet02, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet03, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet04, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet05, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet06, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet07, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet08, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet09, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet10, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet11, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet12, CELL("A1"), "Foo" , NULL);
    worksheet_write_string(worksheet13, CELL("A1"), "Foo" , NULL);

    worksheet_freeze_panes(worksheet01, CELL("A2"));
    worksheet_freeze_panes(worksheet02, CELL("A3"));
    worksheet_freeze_panes(worksheet03, CELL("B1"));
    worksheet_freeze_panes(worksheet04, CELL("C1"));
    worksheet_freeze_panes(worksheet05, CELL("B2"));
    worksheet_freeze_panes(worksheet06, CELL("G4"));
    worksheet_freeze_panes_opt(worksheet07, 3, 6, 3, 6, 1);
    worksheet_split_panes(worksheet08, 15, 0);
    worksheet_split_panes(worksheet09, 30, 0);
    worksheet_split_panes(worksheet10, 0, 8.46);
    worksheet_split_panes(worksheet11, 0, 17.57);
    worksheet_split_panes(worksheet12, 15, 8.46);
    worksheet_split_panes(worksheet13, 45, 54.14);

    return workbook_close(workbook);
}
