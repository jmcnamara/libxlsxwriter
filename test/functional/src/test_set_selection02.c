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

    lxw_workbook  *workbook  = workbook_new("test_set_selection02.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet4 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet5 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet6 = workbook_add_worksheet(workbook, NULL);

    worksheet_set_selection(worksheet1, 3, 2, 3, 2);     /* 1. Cell C4. */
    worksheet_set_selection(worksheet2, 3, 2, 6, 6);     /* 2. Cells C4 to G7. */
    worksheet_set_selection(worksheet3, 6, 6, 3, 2);     /* 3. Cells G7 to C.4 */
    worksheet_set_selection(worksheet4, RANGE("C4:C4")); /* Same as 1. */
    worksheet_set_selection(worksheet5, RANGE("C4:G7")); /* Same as 2. */
    worksheet_set_selection(worksheet6, RANGE("G7:C4")); /* Same as 3. */

    return workbook_close(workbook);
}
