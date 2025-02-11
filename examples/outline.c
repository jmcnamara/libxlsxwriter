/*
 * Example of how use libxlsxwriter to generate Excel outlines and grouping.
 *
 * Excel allows you to group rows or columns so that they can be hidden or
 * displayed with a single mouse click. This feature is referred to as
 * outlines.
 *
 * Outlines can reduce complex data down to a few salient sub-totals or
 * summaries.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook   = workbook_new("outline.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, "Outlined Rows");
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, "Collapsed Rows");
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, "Outline Columns");
    lxw_worksheet *worksheet4 = workbook_add_worksheet(workbook, "Outline levels");

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

    /* Set the column width for clarity. */
    worksheet_set_column(worksheet1, COLS("A:A"), 20, NULL);

    /* Set the row options with the outline level. */
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

    /* Add data and formulas to the worksheet. */
    worksheet_write_string(worksheet1, CELL("A1"), "Region", bold);
    worksheet_write_string(worksheet1, CELL("A2"), "North",  NULL);
    worksheet_write_string(worksheet1, CELL("A3"), "North",  NULL);
    worksheet_write_string(worksheet1, CELL("A4"), "North",  NULL);
    worksheet_write_string(worksheet1, CELL("A5"), "North",  NULL);
    worksheet_write_string(worksheet1, CELL("A6"), "North Total", bold);

    worksheet_write_string(worksheet1, CELL("B1"), "Sales", bold);
    worksheet_write_number(worksheet1, CELL("B2"), 1000,    NULL);
    worksheet_write_number(worksheet1, CELL("B3"), 1200,    NULL);
    worksheet_write_number(worksheet1, CELL("B4"), 900,     NULL);
    worksheet_write_number(worksheet1, CELL("B5"), 1200,    NULL);
    worksheet_write_formula(worksheet1, CELL("B6"), "=SUBTOTAL(9,B2:B5)", bold);

    worksheet_write_string(worksheet1, CELL("A7"), "South",  NULL);
    worksheet_write_string(worksheet1, CELL("A8"), "South",  NULL);
    worksheet_write_string(worksheet1, CELL("A9"), "South",  NULL);
    worksheet_write_string(worksheet1, CELL("A10"), "South", NULL);
    worksheet_write_string(worksheet1, CELL("A11"), "South Total", bold);

    worksheet_write_number(worksheet1, CELL("B7"),  400, NULL);
    worksheet_write_number(worksheet1, CELL("B8"),  600, NULL);
    worksheet_write_number(worksheet1, CELL("B9"),  500, NULL);
    worksheet_write_number(worksheet1, CELL("B10"), 600, NULL);
    worksheet_write_formula(worksheet1, CELL("B11"), "=SUBTOTAL(9,B7:B10)", bold);

    worksheet_write_string(worksheet1, CELL("A12"), "Grand Total", bold);
    worksheet_write_formula(worksheet1, CELL("B12"), "=SUBTOTAL(9,B2:B10)", bold);


   /*
    * Example 2: Create a worksheet with outlined rows. This is the same as
    * the previous example except that the rows are collapsed.  Note: We need
    * to indicate the row that contains the collapsed symbol '+' with the
    * optional parameter, 'collapsed'.
    */

    /* The option structs with the outline level and collapsed property set. */
    lxw_row_col_options options3 = {.hidden = 1, .level = 2, .collapsed = 0};
    lxw_row_col_options options4 = {.hidden = 1, .level = 1, .collapsed = 0};
    lxw_row_col_options options5 = {.hidden = 0, .level = 0, .collapsed = 1};

    /* Set the column width for clarity. */
    worksheet_set_column(worksheet2, COLS("A:A"), 20, NULL);

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

    /* Add data and formulas to the worksheet. */
    worksheet_write_string(worksheet2, CELL("A1"), "Region", bold);
    worksheet_write_string(worksheet2, CELL("A2"), "North",  NULL);
    worksheet_write_string(worksheet2, CELL("A3"), "North",  NULL);
    worksheet_write_string(worksheet2, CELL("A4"), "North",  NULL);
    worksheet_write_string(worksheet2, CELL("A5"), "North",  NULL);
    worksheet_write_string(worksheet2, CELL("A6"), "North Total", bold);

    worksheet_write_string(worksheet2, CELL("B1"), "Sales", bold);
    worksheet_write_number(worksheet2, CELL("B2"), 1000, NULL);
    worksheet_write_number(worksheet2, CELL("B3"), 1200, NULL);
    worksheet_write_number(worksheet2, CELL("B4"), 900,  NULL);
    worksheet_write_number(worksheet2, CELL("B5"), 1200, NULL);
    worksheet_write_formula(worksheet2, CELL("B6"), "=SUBTOTAL(9,B2:B5)", bold);

    worksheet_write_string(worksheet2, CELL("A7"),  "South", NULL);
    worksheet_write_string(worksheet2, CELL("A8"),  "South", NULL);
    worksheet_write_string(worksheet2, CELL("A9"),  "South", NULL);
    worksheet_write_string(worksheet2, CELL("A10"), "South", NULL);
    worksheet_write_string(worksheet2, CELL("A11"), "South Total", bold);

    worksheet_write_number(worksheet2, CELL("B7"),  400, NULL);
    worksheet_write_number(worksheet2, CELL("B8"),  600, NULL);
    worksheet_write_number(worksheet2, CELL("B9"),  500, NULL);
    worksheet_write_number(worksheet2, CELL("B10"), 600, NULL);
    worksheet_write_formula(worksheet2, CELL("B11"), "=SUBTOTAL(9,B7:B10)", bold);

    worksheet_write_string(worksheet2, CELL("A12"), "Grand Total", bold);
    worksheet_write_formula(worksheet2, CELL("B12"), "=SUBTOTAL(9,B2:B10)", bold);


    /*
     * Example 3: Create a worksheet with outlined columns.
     */
    lxw_row_col_options options6 = {.hidden = 0, .level = 1, .collapsed = 0};

    /* Add data and formulas to the worksheet. */
    worksheet_write_string(worksheet3, CELL("A1"), "Month", NULL);
    worksheet_write_string(worksheet3, CELL("B1"), "Jan",   NULL);
    worksheet_write_string(worksheet3, CELL("C1"), "Feb",   NULL);
    worksheet_write_string(worksheet3, CELL("D1"), "Mar",   NULL);
    worksheet_write_string(worksheet3, CELL("E1"), "Apr",   NULL);
    worksheet_write_string(worksheet3, CELL("F1"), "May",   NULL);
    worksheet_write_string(worksheet3, CELL("G1"), "Jun",   NULL);
    worksheet_write_string(worksheet3, CELL("H1"), "Total", NULL);

    worksheet_write_string(worksheet3, CELL("A2"), "North", NULL);
    worksheet_write_number(worksheet3, CELL("B2"), 50,      NULL);
    worksheet_write_number(worksheet3, CELL("C2"), 20,      NULL);
    worksheet_write_number(worksheet3, CELL("D2"), 15,      NULL);
    worksheet_write_number(worksheet3, CELL("E2"), 25,      NULL);
    worksheet_write_number(worksheet3, CELL("F2"), 65,      NULL);
    worksheet_write_number(worksheet3, CELL("G2"), 80,      NULL);
    worksheet_write_formula(worksheet3, CELL("H2"), "=SUM(B2:G2)", NULL);

    worksheet_write_string(worksheet3, CELL("A3"), "South", NULL);
    worksheet_write_number(worksheet3, CELL("B3"), 10,      NULL);
    worksheet_write_number(worksheet3, CELL("C3"), 20,      NULL);
    worksheet_write_number(worksheet3, CELL("D3"), 30,      NULL);
    worksheet_write_number(worksheet3, CELL("E3"), 50,      NULL);
    worksheet_write_number(worksheet3, CELL("F3"), 50,      NULL);
    worksheet_write_number(worksheet3, CELL("G3"), 50,      NULL);
    worksheet_write_formula(worksheet3, CELL("H3"), "=SUM(B3:G3)", NULL);

    worksheet_write_string(worksheet3, CELL("A4"), "East",  NULL);
    worksheet_write_number(worksheet3, CELL("B4"), 45,      NULL);
    worksheet_write_number(worksheet3, CELL("C4"), 75,      NULL);
    worksheet_write_number(worksheet3, CELL("D4"), 50,      NULL);
    worksheet_write_number(worksheet3, CELL("E4"), 15,      NULL);
    worksheet_write_number(worksheet3, CELL("F4"), 75,      NULL);
    worksheet_write_number(worksheet3, CELL("G4"), 100,     NULL);
    worksheet_write_formula(worksheet3, CELL("H4"), "=SUM(B4:G4)", NULL);

    worksheet_write_string(worksheet3, CELL("A5"), "West",  NULL);
    worksheet_write_number(worksheet3, CELL("B5"), 15,      NULL);
    worksheet_write_number(worksheet3, CELL("C5"), 15,      NULL);
    worksheet_write_number(worksheet3, CELL("D5"), 55,      NULL);
    worksheet_write_number(worksheet3, CELL("E5"), 35,      NULL);
    worksheet_write_number(worksheet3, CELL("F5"), 20,      NULL);
    worksheet_write_number(worksheet3, CELL("G5"), 50,      NULL);
    worksheet_write_formula(worksheet3, CELL("H5"), "=SUM(B5:G5)", NULL);

    worksheet_write_formula(worksheet3, CELL("H6"), "=SUM(H2:H5)", bold);

    /* Add bold format to the first row. */
    worksheet_set_row(worksheet3, 0, LXW_DEF_ROW_HEIGHT, bold);

    /* Set column formatting and the outline level. */
    worksheet_set_column(    worksheet3, COLS("A:A"), 10, bold);
    worksheet_set_column_opt(worksheet3, COLS("B:G"),  5, NULL, &options6);
    worksheet_set_column(    worksheet3, COLS("H:H"), 10, NULL);



    /*
     * Example 4: Show all possible outline levels.
     */
    lxw_row_col_options level1 = {.level = 1, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level2 = {.level = 2, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level3 = {.level = 3, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level4 = {.level = 4, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level5 = {.level = 5, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level6 = {.level = 6, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level7 = {.level = 7, .hidden = 0, .collapsed = 0};

    worksheet_write_string(worksheet4, 0,  0, "Level 1", NULL);
    worksheet_write_string(worksheet4, 1,  0, "Level 2", NULL);
    worksheet_write_string(worksheet4, 2,  0, "Level 3", NULL);
    worksheet_write_string(worksheet4, 3,  0, "Level 4", NULL);
    worksheet_write_string(worksheet4, 4,  0, "Level 5", NULL);
    worksheet_write_string(worksheet4, 5,  0, "Level 6", NULL);
    worksheet_write_string(worksheet4, 6,  0, "Level 7", NULL);
    worksheet_write_string(worksheet4, 7,  0, "Level 6", NULL);
    worksheet_write_string(worksheet4, 8,  0, "Level 5", NULL);
    worksheet_write_string(worksheet4, 9,  0, "Level 4", NULL);
    worksheet_write_string(worksheet4, 10, 0, "Level 3", NULL);
    worksheet_write_string(worksheet4, 11, 0, "Level 2", NULL);
    worksheet_write_string(worksheet4, 12, 0, "Level 1", NULL);

    worksheet_set_row_opt(worksheet4, 0,  LXW_DEF_ROW_HEIGHT, NULL, &level1);
    worksheet_set_row_opt(worksheet4, 1,  LXW_DEF_ROW_HEIGHT, NULL, &level2);
    worksheet_set_row_opt(worksheet4, 2,  LXW_DEF_ROW_HEIGHT, NULL, &level3);
    worksheet_set_row_opt(worksheet4, 3,  LXW_DEF_ROW_HEIGHT, NULL, &level4);
    worksheet_set_row_opt(worksheet4, 4,  LXW_DEF_ROW_HEIGHT, NULL, &level5);
    worksheet_set_row_opt(worksheet4, 5,  LXW_DEF_ROW_HEIGHT, NULL, &level6);
    worksheet_set_row_opt(worksheet4, 6,  LXW_DEF_ROW_HEIGHT, NULL, &level7);
    worksheet_set_row_opt(worksheet4, 7,  LXW_DEF_ROW_HEIGHT, NULL, &level6);
    worksheet_set_row_opt(worksheet4, 8,  LXW_DEF_ROW_HEIGHT, NULL, &level5);
    worksheet_set_row_opt(worksheet4, 9,  LXW_DEF_ROW_HEIGHT, NULL, &level4);
    worksheet_set_row_opt(worksheet4, 10, LXW_DEF_ROW_HEIGHT, NULL, &level3);
    worksheet_set_row_opt(worksheet4, 11, LXW_DEF_ROW_HEIGHT, NULL, &level2);
    worksheet_set_row_opt(worksheet4, 12, LXW_DEF_ROW_HEIGHT, NULL, &level1);


    workbook_close(workbook);

    return 0;
}
