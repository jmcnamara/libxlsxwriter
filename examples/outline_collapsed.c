/*
 * Example of how use libxlsxwriter to generate Excel outlines and grouping.
 *
 * These examples focus mainly on collapsed outlines. See also the outlines.c
 * example program for more general examples.
 *
 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"


/* This function will generate the same data and sub-totals on each worksheet.
 * Used in the examples 1-4.
 */
void create_row_example_data(lxw_worksheet *worksheet, lxw_format *bold) {

    /* Set the column width for clarity. */
    worksheet_set_column(worksheet, COLS("A:A"), 20, NULL);

    /* Add data and formulas to the worksheet. */
    worksheet_write_string(worksheet, CELL("A1"), "Region", bold);
    worksheet_write_string(worksheet, CELL("A2"), "North",  NULL);
    worksheet_write_string(worksheet, CELL("A3"), "North",  NULL);
    worksheet_write_string(worksheet, CELL("A4"), "North",  NULL);
    worksheet_write_string(worksheet, CELL("A5"), "North",  NULL);
    worksheet_write_string(worksheet, CELL("A6"), "North Total", bold);

    worksheet_write_string(worksheet, CELL("B1"), "Sales", bold);
    worksheet_write_number(worksheet, CELL("B2"), 1000,    NULL);
    worksheet_write_number(worksheet, CELL("B3"), 1200,    NULL);
    worksheet_write_number(worksheet, CELL("B4"), 900,     NULL);
    worksheet_write_number(worksheet, CELL("B5"), 1200,    NULL);
    worksheet_write_formula(worksheet, CELL("B6"), "=SUBTOTAL(9,B2:B5)", bold);

    worksheet_write_string(worksheet, CELL("A7"), "South",  NULL);
    worksheet_write_string(worksheet, CELL("A8"), "South",  NULL);
    worksheet_write_string(worksheet, CELL("A9"), "South",  NULL);
    worksheet_write_string(worksheet, CELL("A10"), "South", NULL);
    worksheet_write_string(worksheet, CELL("A11"), "South Total", bold);

    worksheet_write_number(worksheet, CELL("B7"),  400, NULL);
    worksheet_write_number(worksheet, CELL("B8"),  600, NULL);
    worksheet_write_number(worksheet, CELL("B9"),  500, NULL);
    worksheet_write_number(worksheet, CELL("B10"), 600, NULL);
    worksheet_write_formula(worksheet, CELL("B11"), "=SUBTOTAL(9,B7:B10)", bold);

    worksheet_write_string(worksheet, CELL("A12"), "Grand Total", bold);
    worksheet_write_formula(worksheet, CELL("B12"), "=SUBTOTAL(9,B2:B10)", bold);
}


/* This function will generate the same data and sub-totals on each worksheet.
 * Used in the examples 5-6.
 */
void create_col_example_data(lxw_worksheet *worksheet, lxw_format *bold) {

    /* Add data and formulas to the worksheet. */
    worksheet_write_string(worksheet, CELL("A1"), "Month", NULL);
    worksheet_write_string(worksheet, CELL("B1"), "Jan",   NULL);
    worksheet_write_string(worksheet, CELL("C1"), "Feb",   NULL);
    worksheet_write_string(worksheet, CELL("D1"), "Mar",   NULL);
    worksheet_write_string(worksheet, CELL("E1"), "Apr",   NULL);
    worksheet_write_string(worksheet, CELL("F1"), "May",   NULL);
    worksheet_write_string(worksheet, CELL("G1"), "Jun",   NULL);
    worksheet_write_string(worksheet, CELL("H1"), "Total", NULL);

    worksheet_write_string(worksheet, CELL("A2"), "North", NULL);
    worksheet_write_number(worksheet, CELL("B2"), 50,      NULL);
    worksheet_write_number(worksheet, CELL("C2"), 20,      NULL);
    worksheet_write_number(worksheet, CELL("D2"), 15,      NULL);
    worksheet_write_number(worksheet, CELL("E2"), 25,      NULL);
    worksheet_write_number(worksheet, CELL("F2"), 65,      NULL);
    worksheet_write_number(worksheet, CELL("G2"), 80,      NULL);
    worksheet_write_formula(worksheet, CELL("H2"), "=SUM(B2:G2)", NULL);

    worksheet_write_string(worksheet, CELL("A3"), "South", NULL);
    worksheet_write_number(worksheet, CELL("B3"), 10,      NULL);
    worksheet_write_number(worksheet, CELL("C3"), 20,      NULL);
    worksheet_write_number(worksheet, CELL("D3"), 30,      NULL);
    worksheet_write_number(worksheet, CELL("E3"), 50,      NULL);
    worksheet_write_number(worksheet, CELL("F3"), 50,      NULL);
    worksheet_write_number(worksheet, CELL("G3"), 50,      NULL);
    worksheet_write_formula(worksheet, CELL("H3"), "=SUM(B3:G3)", NULL);

    worksheet_write_string(worksheet, CELL("A4"), "East",  NULL);
    worksheet_write_number(worksheet, CELL("B4"), 45,      NULL);
    worksheet_write_number(worksheet, CELL("C4"), 75,      NULL);
    worksheet_write_number(worksheet, CELL("D4"), 50,      NULL);
    worksheet_write_number(worksheet, CELL("E4"), 15,      NULL);
    worksheet_write_number(worksheet, CELL("F4"), 75,      NULL);
    worksheet_write_number(worksheet, CELL("G4"), 100,     NULL);
    worksheet_write_formula(worksheet, CELL("H4"), "=SUM(B4:G4)", NULL);

    worksheet_write_string(worksheet, CELL("A5"), "West",  NULL);
    worksheet_write_number(worksheet, CELL("B5"), 15,      NULL);
    worksheet_write_number(worksheet, CELL("C5"), 15,      NULL);
    worksheet_write_number(worksheet, CELL("D5"), 55,      NULL);
    worksheet_write_number(worksheet, CELL("E5"), 35,      NULL);
    worksheet_write_number(worksheet, CELL("F5"), 20,      NULL);
    worksheet_write_number(worksheet, CELL("G5"), 50,      NULL);
    worksheet_write_formula(worksheet, CELL("H5"), "=SUM(B5:G5)", NULL);

    worksheet_write_formula(worksheet, CELL("H6"), "=SUM(H2:H5)", bold);
}

int main() {

    lxw_workbook  *workbook   = workbook_new("outline_collapsed.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, "Outlined Rows");
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, "Collapsed Rows 1");
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, "Collapsed Rows 2");
    lxw_worksheet *worksheet4 = workbook_add_worksheet(workbook, "Collapsed Rows 3");
    lxw_worksheet *worksheet5 = workbook_add_worksheet(workbook, "Outline Columns");
    lxw_worksheet *worksheet6 = workbook_add_worksheet(workbook, "Collapsed Columns");

    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

   /*
    * Example 1: Create a worksheet with outlined rows. It also includes
    * SUBTOTAL() functions so that it looks like the type of automatic
    * outlines that are generated when you use the 'Sub Totals' option.
    *
    * For outlines the important parameters are 'hidden' and 'level'. Rows
    * with the same 'level' are grouped together. The group will be collapsed
    * if 'hidden' is non-zero.
    */

    /* The option structs with the outline level set. */
    lxw_row_col_options options1 = {.hidden = 0, .level = 2, .collapsed = 0};
    lxw_row_col_options options2 = {.hidden = 0, .level = 1, .collapsed = 0};

    /* Set the row outline properties set. */
    worksheet_set_row_opt(worksheet1, 1,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 2,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 3,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 4,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 5,  LXW_DEF_ROW_HEIGHT, NULL, &options2);

    worksheet_set_row_opt(worksheet1, 6,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 7,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 8,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 9,  LXW_DEF_ROW_HEIGHT, NULL, &options1);
    worksheet_set_row_opt(worksheet1, 10, LXW_DEF_ROW_HEIGHT, NULL, &options2);

    /* Write the sub-total data that is common to the row examples. */
    create_row_example_data(worksheet1, bold);


   /*
    * Example 2: Create a worksheet with collapsed outlined rows.
    * This is the same as the example 1  except that the all rows are collapsed.
    */

    /* The option structs with the outline properties set. */
    lxw_row_col_options options3 = {.hidden = 1, .level = 2, .collapsed = 0};
    lxw_row_col_options options4 = {.hidden = 1, .level = 1, .collapsed = 0};
    lxw_row_col_options options5 = {.hidden = 0, .level = 0, .collapsed = 1};

    /* Set the row options with the outline level. */
    worksheet_set_row_opt(worksheet2, 1,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 2,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 3,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 4,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 5,  LXW_DEF_ROW_HEIGHT, NULL, &options4);

    worksheet_set_row_opt(worksheet2, 6,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 7,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 8,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 9,  LXW_DEF_ROW_HEIGHT, NULL, &options3);
    worksheet_set_row_opt(worksheet2, 10, LXW_DEF_ROW_HEIGHT, NULL, &options4);
    worksheet_set_row_opt(worksheet2, 11, LXW_DEF_ROW_HEIGHT, NULL, &options5);

    /* Write the sub-total data that is common to the row examples. */
    create_row_example_data(worksheet2, bold);


    /*
     * Example 3: Create a worksheet with collapsed outlined rows. Same as the
     * example 1 except that the two sub-totals are collapsed.
     */
    lxw_row_col_options options6 = {.hidden = 1, .level = 2, .collapsed = 0};
    lxw_row_col_options options7 = {.hidden = 0, .level = 1, .collapsed = 1};

    /* Set the row options with the outline level. */
    worksheet_set_row_opt(worksheet3, 1,  LXW_DEF_ROW_HEIGHT, NULL, &options6);
    worksheet_set_row_opt(worksheet3, 2,  LXW_DEF_ROW_HEIGHT, NULL, &options6);
    worksheet_set_row_opt(worksheet3, 3,  LXW_DEF_ROW_HEIGHT, NULL, &options6);
    worksheet_set_row_opt(worksheet3, 4,  LXW_DEF_ROW_HEIGHT, NULL, &options6);
    worksheet_set_row_opt(worksheet3, 5,  LXW_DEF_ROW_HEIGHT, NULL, &options7);

    worksheet_set_row_opt(worksheet3, 6,  LXW_DEF_ROW_HEIGHT, NULL, &options6);
    worksheet_set_row_opt(worksheet3, 7,  LXW_DEF_ROW_HEIGHT, NULL, &options6);
    worksheet_set_row_opt(worksheet3, 8,  LXW_DEF_ROW_HEIGHT, NULL, &options6);
    worksheet_set_row_opt(worksheet3, 9,  LXW_DEF_ROW_HEIGHT, NULL, &options6);
    worksheet_set_row_opt(worksheet3, 10, LXW_DEF_ROW_HEIGHT, NULL, &options7);

    /* Write the sub-total data that is common to the row examples. */
    create_row_example_data(worksheet3, bold);


    /*
     * Example 4: Create a worksheet with outlined rows. Same as the example 1
     * except that the two sub-totals are collapsed.
     */
    lxw_row_col_options options8  = {.hidden = 1, .level = 2, .collapsed = 0};
    lxw_row_col_options options9  = {.hidden = 1, .level = 1, .collapsed = 1};
    lxw_row_col_options options10 = {.hidden = 0, .level = 0, .collapsed = 1};

    /* Set the row options with the outline level. */
    worksheet_set_row_opt(worksheet4, 1,  LXW_DEF_ROW_HEIGHT, NULL, &options8);
    worksheet_set_row_opt(worksheet4, 2,  LXW_DEF_ROW_HEIGHT, NULL, &options8);
    worksheet_set_row_opt(worksheet4, 3,  LXW_DEF_ROW_HEIGHT, NULL, &options8);
    worksheet_set_row_opt(worksheet4, 4,  LXW_DEF_ROW_HEIGHT, NULL, &options8);
    worksheet_set_row_opt(worksheet4, 5,  LXW_DEF_ROW_HEIGHT, NULL, &options9);

    worksheet_set_row_opt(worksheet4, 6,  LXW_DEF_ROW_HEIGHT, NULL, &options8);
    worksheet_set_row_opt(worksheet4, 7,  LXW_DEF_ROW_HEIGHT, NULL, &options8);
    worksheet_set_row_opt(worksheet4, 8,  LXW_DEF_ROW_HEIGHT, NULL, &options8);
    worksheet_set_row_opt(worksheet4, 9,  LXW_DEF_ROW_HEIGHT, NULL, &options8);
    worksheet_set_row_opt(worksheet4, 10, LXW_DEF_ROW_HEIGHT, NULL, &options9);

    worksheet_set_row_opt(worksheet4, 11, LXW_DEF_ROW_HEIGHT, NULL, &options10);

    /* Write the sub-total data that is common to the row examples. */
    create_row_example_data(worksheet4, bold);


    /*
     * Example 5: Create a worksheet with outlined columns.
     */

    lxw_row_col_options options11 = {.hidden = 0, .level = 1, .collapsed = 0};

    /* Write the sub-total data that is common to the column examples. */
    create_col_example_data(worksheet5, bold);

    /* Add bold format to the first row. */
    worksheet_set_row(worksheet5, 0, LXW_DEF_ROW_HEIGHT, bold);

    /* Set column formatting and the outline level. */
    worksheet_set_column(    worksheet5, COLS("A:A"), 10, bold);
    worksheet_set_column_opt(worksheet5, COLS("B:G"),  5, NULL, &options11);
    worksheet_set_column(    worksheet5, COLS("H:H"), 10, NULL);


    /*
     * Example 6: Create a worksheet with outlined columns.
     */

    lxw_row_col_options options12 = {.hidden = 1, .level = 1, .collapsed = 0};
    lxw_row_col_options options13 = {.hidden = 0, .level = 0, .collapsed = 1};

    /* Write the sub-total data that is common to the column examples. */
    create_col_example_data(worksheet6, bold);

    /* Add bold format to the first row. */
    worksheet_set_row(worksheet6, 0, LXW_DEF_ROW_HEIGHT, bold);

    /* Set column formatting and the outline level. */
    worksheet_set_column(    worksheet6, COLS("A:A"), 10, bold);
    worksheet_set_column_opt(worksheet6, COLS("B:G"),  5, NULL, &options12);
    worksheet_set_column_opt(worksheet6, COLS("H:H"), 10, NULL, &options13);


    workbook_close(workbook);

    return 0;
}
