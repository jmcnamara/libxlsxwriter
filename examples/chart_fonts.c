/*
 * An example of a simple Excel chart with user defined fonts using the
 * libxlsxwriter library.
 *
 * Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

/* Write some data to the worksheet. */
void write_worksheet_data(lxw_worksheet *worksheet) {

    uint8_t data[5][3] = {
        /* Three columns of data. */
        {1,   2,   3},
        {2,   4,   6},
        {3,   6,   9},
        {4,   8,  12},
        {5,  10,  15}
    };

    int row, col;
    for (row = 0; row < 5; row++)
        for (col = 0; col < 3; col++)
            worksheet_write_number(worksheet, row, col, data[row][col], NULL);
}

/* Create a worksheet with a chart. */
int main() {

    lxw_workbook  *workbook  = new_workbook("chart_fonts.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Write some data for the chart. */
    write_worksheet_data(worksheet);

    /* Create a chart object. */
    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Configure the chart. In simplest case we just add some value data
     * series. The NULL categories will default to 1 to 5 like in Excel.
     */
    chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$5");

    /* Create some fonts to use in the chart.  */
    lxw_chart_font font1 = {.name = "Calibri", .color = LXW_COLOR_BLUE};
    lxw_chart_font font2 = {.name = "Courier", .color = 0x92D050};
    lxw_chart_font font3 = {.name = "Arial",   .color = 0x00B0F0};
    lxw_chart_font font4 = {.name = "Century", .color = LXW_COLOR_RED};
    lxw_chart_font font5 = {.rotation = -30};
    lxw_chart_font font6 = {.bold      = LXW_TRUE,
                            .italic    = LXW_TRUE,
                            .underline = LXW_TRUE,
                            .color     = 0x7030A0};

    /* Write the chart title with a font. */
    chart_title_set_name(chart, "Test Results");
    chart_title_set_name_font(chart, &font1);

    /* Write the Y axis with a font. */
    chart_axis_set_name(chart->y_axis, "Units");
    chart_axis_set_name_font(chart->y_axis, &font2);
    chart_axis_set_num_font(chart->y_axis, &font3);

    /* Write the X axis with a font. */
    chart_axis_set_name(chart->x_axis, "Month");
    chart_axis_set_name_font(chart->x_axis, &font4);
    chart_axis_set_num_font(chart->x_axis, &font5);


    /* Display the chart legend at the bottom of the chart. */
    chart_legend_set_position(chart, LXW_CHART_LEGEND_BOTTOM);
    chart_legend_set_font(chart, &font6);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("E9"), chart);

    return workbook_close(workbook);
}
