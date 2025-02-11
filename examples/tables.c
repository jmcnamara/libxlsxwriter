/*
 * An example of how to add conditional formatting to an libxlsxwriter file.
 *
 * Conditional formatting allows you to apply a format to a cell or a
 * range of cells based on certain criteria.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

void write_worksheet_data(lxw_worksheet *worksheet, lxw_format *format);

int main() {

    lxw_workbook  *workbook    = workbook_new("tables.xlsx");
    lxw_worksheet *worksheet1  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet4  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet5  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet6  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet7  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet8  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet9  = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet10 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet11 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet12 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet13 = workbook_add_worksheet(workbook, NULL);

    lxw_format *currency_format = workbook_add_format(workbook);
    format_set_num_format(currency_format, "$#,##0");

    /*
     * Example 1. Default table with no data
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet1, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet1, CELL("B1"), "Default table with no data.", NULL);

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet1, RANGE("B3:F7"), NULL);

    /*
     * Example 2. Default table with data
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet2, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet2, CELL("B1"), "Default table with data.", NULL);

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet2, RANGE("B3:F7"), NULL);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet2, NULL);

    /*
     * Example 3. Table without default autofilter
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet3, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet3, CELL("B1"), "Table without default autofilter.", NULL);

    /* Set the table options. */
    lxw_table_options options3 = {.no_autofilter = LXW_TRUE};

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet3, RANGE("B3:F7"), &options3);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet3, NULL);


    /*
     * Example 4. Table without default header row
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet4, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet4, CELL("B1"), "Table without default header row.", NULL);

    /* Set the table options. */
    lxw_table_options options4 = {.no_header_row = LXW_TRUE};

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet4, RANGE("B4:F7"), &options4);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet4, NULL);


    /*
     * Example 5. Default table with "First Column" and "Last Column" options
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet5, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet5, CELL("B1"),
                           "Default table with \"First Column\" and \"Last Column\" options.",
                           NULL);

    /* Set the table options. */
    lxw_table_options options5 = {.first_column = LXW_TRUE, .last_column = LXW_TRUE};

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet5, RANGE("B3:F7"), &options5);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet5, NULL);


    /*
     * Example 6. Table with banded columns but without default banded rows
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet6, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet6, CELL("B1"),
                           "Table with banded columns but without default banded rows.",
                           NULL);

    /* Set the table options. */
    lxw_table_options options6 = {.no_banded_rows = LXW_TRUE, .banded_columns = LXW_TRUE};

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet6, RANGE("B3:F7"), &options6);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet6, NULL);


    /*
     * Example 7. Table with user defined column headers
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet7, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet7, CELL("B1"), "Table with user defined column headers.", NULL);


    /* Set the table options. */
    lxw_table_column col7_1 = {.header = "Product"};
    lxw_table_column col7_2 = {.header = "Quarter 1"};
    lxw_table_column col7_3 = {.header = "Quarter 2"};
    lxw_table_column col7_4 = {.header = "Quarter 3"};
    lxw_table_column col7_5 = {.header = "Quarter 4"};

    lxw_table_column *columns7[] = {&col7_1, &col7_2, &col7_3, &col7_4, &col7_5, NULL};

    lxw_table_options options7 = {.columns = columns7};

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet7, RANGE("B3:F7"), &options7);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet7, NULL);


    /*
     * Example 8. Table with user defined column headers
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet8, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet8, CELL("B1"), "Table with user defined column headers.", NULL);

    /* Set the table options. */
    lxw_table_column col8_1 = {.header = "Product"};
    lxw_table_column col8_2 = {.header = "Quarter 1"};
    lxw_table_column col8_3 = {.header = "Quarter 2"};
    lxw_table_column col8_4 = {.header = "Quarter 3"};
    lxw_table_column col8_5 = {.header = "Quarter 4"};
    lxw_table_column col8_6 = {.header = "Year",
                               .formula = "=SUM(Table8[@[Quarter 1]:[Quarter 4]])"};

    lxw_table_column *columns8[] = {&col8_1, &col8_2, &col8_3, &col8_4, &col8_5, &col8_6, NULL};

    lxw_table_options options8 = {.columns = columns8};

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet8, RANGE("B3:G7"), &options8);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet8, NULL);


    /*
     * Example 9. Table with totals row (but no caption or totals)
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet9, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet9, CELL("B1"),
                           "Table with totals row (but no caption or totals).",
                           NULL);


    /* Set the table options. */
    lxw_table_column col9_1 = {.header = "Product"};
    lxw_table_column col9_2 = {.header = "Quarter 1"};
    lxw_table_column col9_3 = {.header = "Quarter 2"};
    lxw_table_column col9_4 = {.header = "Quarter 3"};
    lxw_table_column col9_5 = {.header = "Quarter 4"};
    lxw_table_column col9_6 = {.header = "Year",
                               .formula = "=SUM(Table9[@[Quarter 1]:[Quarter 4]])"};

    lxw_table_column *columns9[] = {&col9_1, &col9_2, &col9_3, &col9_4, &col9_5, &col9_6, NULL};

    lxw_table_options options9 = {.total_row = LXW_TRUE, .columns = columns9};

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet9, RANGE("B3:G8"), &options9);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet9, NULL);


    /*
     * Example 10. Table with totals row with user captions and functions
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet10, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet10, CELL("B1"),
                           "Table with totals row with user captions and functions.",
                           NULL);

    /* Set the table options. */
    lxw_table_column col10_1 = {.header         = "Product",
                                .total_string   = "Totals"};

    lxw_table_column col10_2 = {.header         = "Quarter 1",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col10_3 = {.header         = "Quarter 2",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col10_4 = {.header         = "Quarter 3",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col10_5 = {.header         = "Quarter 4",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col10_6 = {.header         = "Year",
                                .formula        = "=SUM(Table10[@[Quarter 1]:[Quarter 4]])",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column *columns10[] = {&col10_1, &col10_2, &col10_3, &col10_4,
                                     &col10_5, &col10_6, NULL};

    lxw_table_options options10 = {.total_row = LXW_TRUE, .columns = columns10};

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet10, RANGE("B3:G8"), &options10);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet10, NULL);


    /*
     * Example 11. Table with alternative Excel style
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet11, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet11, CELL("B1"), "Table with alternative Excel style.", NULL);

    /* Set the table options. */
    lxw_table_column col11_1 = {.header         = "Product",
                                .total_string   = "Totals"};

    lxw_table_column col11_2 = {.header         = "Quarter 1",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col11_3 = {.header         = "Quarter 2",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col11_4 = {.header         = "Quarter 3",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col11_5 = {.header         = "Quarter 4",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col11_6 = {.header         = "Year",
                                .formula        = "=SUM(Table11[@[Quarter 1]:[Quarter 4]])",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column *columns11[] = {&col11_1, &col11_2, &col11_3, &col11_4,
                                     &col11_5, &col11_6, NULL};

    lxw_table_options options11 = {
        .style_type = LXW_TABLE_STYLE_TYPE_LIGHT,
        .style_type_number = 11,
        .total_row = LXW_TRUE,
        .columns = columns11
    };

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet11, RANGE("B3:G8"), &options11);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet11, NULL);

    /*
     * Example 12. Table with Excel style removed
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet12, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet12, CELL("B1"), "Table with Excel style removed.", NULL);

    /* Set the table options. */
    lxw_table_column col12_1 = {.header         = "Product",
                                .total_string   = "Totals"};

    lxw_table_column col12_2 = {.header         = "Quarter 1",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col12_3 = {.header         = "Quarter 2",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col12_4 = {.header         = "Quarter 3",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col12_5 = {.header         = "Quarter 4",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column col12_6 = {.header         = "Year",
                                .formula        = "=SUM(Table12[@[Quarter 1]:[Quarter 4]])",
                                .total_function = LXW_TABLE_FUNCTION_SUM};

    lxw_table_column *columns12[] = {&col12_1, &col12_2, &col12_3, &col12_4,
                                     &col12_5, &col12_6, NULL};

    lxw_table_options options12 = {
        .style_type = LXW_TABLE_STYLE_TYPE_LIGHT,
        .style_type_number = 0,
        .total_row = LXW_TRUE,
        .columns = columns12
    };

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet12, RANGE("B3:G8"), &options12);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet12, NULL);

    /*
     * Example 13. Table with column formats
     */

    /* Set the columns widths for clarity. */
    worksheet_set_column(worksheet13, COLS("B:G"), 12, NULL);

    /* Write the worksheet caption to explain the example. */
    worksheet_write_string(worksheet13, CELL("B1"), "Table with column formats.", NULL);

    /* Set the table options. */
    lxw_table_column col13_1 = {.header         = "Product",
                                .total_string   = "Totals"};

    lxw_table_column col13_2 = {.header         = "Quarter 1",
                                .total_function = LXW_TABLE_FUNCTION_SUM,
                                .format         = currency_format};

    lxw_table_column col13_3 = {.header         = "Quarter 2",
                                .total_function = LXW_TABLE_FUNCTION_SUM,
                                .format         = currency_format};

    lxw_table_column col13_4 = {.header         = "Quarter 3",
                                .total_function = LXW_TABLE_FUNCTION_SUM,
                                .format         = currency_format};

    lxw_table_column col13_5 = {.header         = "Quarter 4",
                                .total_function = LXW_TABLE_FUNCTION_SUM,
                                .format         = currency_format};

    lxw_table_column col13_6 = {.header         = "Year",
                                .formula        = "=SUM(Table13[@[Quarter 1]:[Quarter 4]])",
                                .total_function = LXW_TABLE_FUNCTION_SUM,
                                .format         = currency_format};

    lxw_table_column *columns13[] = {&col13_1, &col13_2, &col13_3, &col13_4,
                                     &col13_5, &col13_6, NULL};

    lxw_table_options options13 = {.total_row = LXW_TRUE, .columns = columns13};

    /* Add a table to the worksheet. */
    worksheet_add_table(worksheet13, RANGE("B3:G8"), &options13);

    /* Write the data into the worksheet cells. */
    write_worksheet_data(worksheet13, currency_format);


    return workbook_close(workbook);
}

/* Write some data to the worksheet. */
void write_worksheet_data(lxw_worksheet *worksheet, lxw_format *format) {

    worksheet_write_string(worksheet, CELL("B4"), "Apples",  NULL);
    worksheet_write_string(worksheet, CELL("B5"), "Pears",   NULL);
    worksheet_write_string(worksheet, CELL("B6"), "Bananas", NULL);
    worksheet_write_string(worksheet, CELL("B7"), "Oranges", NULL);

    worksheet_write_number(worksheet, CELL("C4"), 10000,  format);
    worksheet_write_number(worksheet, CELL("C5"),  2000,  format);
    worksheet_write_number(worksheet, CELL("C6"),  6000,  format);
    worksheet_write_number(worksheet, CELL("C7"),   500,  format);

    worksheet_write_number(worksheet, CELL("D4"),  5000,  format);
    worksheet_write_number(worksheet, CELL("D5"),  3000,  format);
    worksheet_write_number(worksheet, CELL("D6"),  6000,  format);
    worksheet_write_number(worksheet, CELL("D7"),   300,  format);

    worksheet_write_number(worksheet, CELL("E4"),  8000,  format);
    worksheet_write_number(worksheet, CELL("E5"),  4000,  format);
    worksheet_write_number(worksheet, CELL("E6"),  6500,  format);
    worksheet_write_number(worksheet, CELL("E7"),   200,  format);

    worksheet_write_number(worksheet, CELL("F4"),  6000,  format);
    worksheet_write_number(worksheet, CELL("F5"),  5000,  format);
    worksheet_write_number(worksheet, CELL("F6"),  6000,  format);
    worksheet_write_number(worksheet, CELL("F7"),   700,  format);

}
