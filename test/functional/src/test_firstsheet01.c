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

    lxw_workbook  *workbook  = workbook_new("test_firstsheet01.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet4 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet5 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet6 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet7 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet8 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet9 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet10 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet11 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet12 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet13 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet14 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet15 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet16 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet17 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet18 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet19 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet20 = workbook_add_worksheet(workbook, NULL);

    worksheet_set_first_sheet(worksheet8);
    worksheet_activate(worksheet20);

    /* Avoid warnings about unused variables. */
    (void)worksheet1;
    (void)worksheet2;
    (void)worksheet3;
    (void)worksheet4;
    (void)worksheet5;
    (void)worksheet6;
    (void)worksheet7;
    (void)worksheet9;
    (void)worksheet10;
    (void)worksheet11;
    (void)worksheet12;
    (void)worksheet13;
    (void)worksheet14;
    (void)worksheet15;
    (void)worksheet16;
    (void)worksheet17;
    (void)worksheet18;
    (void)worksheet19;

    return workbook_close(workbook);
}
