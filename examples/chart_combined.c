/*
 * An example of creating combined charts using the libxlsxwriter library.
 *
 * A combined chart is one that shows two chart types, such as a column chart
 * combined with a line chart. They can share the same Y axis or have a
 * secondary Y axis.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

/*
 * Write some data to the worksheet.
 */
void write_worksheet_data(lxw_worksheet *worksheet, lxw_format *bold) {

    int row, col;
    uint8_t data[6][3] = {
        /* Three columns of data. */
        {2, 10, 30},
        {3, 40, 60},
        {4, 50, 70},
        {5, 20, 50},
        {6, 10, 40},
        {7, 50, 30}
    };

    worksheet_write_string(worksheet, CELL("A1"), "Number",  bold);
    worksheet_write_string(worksheet, CELL("B1"), "Batch 1", bold);
    worksheet_write_string(worksheet, CELL("C1"), "Batch 2", bold);

    for (row = 0; row < 6; row++)
        for (col = 0; col < 3; col++)
            worksheet_write_number(worksheet, row + 1, col, data[row][col], NULL);
}

/*
 * Create a worksheet with combined chart examples.
 */
int main() {

    lxw_workbook     *workbook  = workbook_new("chart_combined.xlsx");
    lxw_worksheet    *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart_series *series;

    /* Add a bold format to use to highlight the header cells. */
    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    /* Write some data for the chart. */
    write_worksheet_data(worksheet, bold);


    /*
     * Chart 1. Create a combined column and line chart with shared Y axis.
     *
     * In this example we will create a combined column and line chart.
     * They will share the same X and Y axes.
     */

    /* Create a column chart - this will be the primary chart. */
    lxw_chart *column_chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Add the first series to the primary chart. */
    series = chart_add_series(column_chart,
        "=Sheet1!$A$2:$A$7",
        "=Sheet1!$B$2:$B$7",
        0);  /* Primary Y axis */
    chart_series_set_name(series, "=Sheet1!$B$1");

    /* Create a line chart - this will be the secondary (combined) chart. */
    lxw_chart *line_chart = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* Add the first series to the secondary chart. */
    series = chart_add_series(line_chart,
        "=Sheet1!$A$2:$A$7",
        "=Sheet1!$C$2:$C$7",
        0);  /* Primary Y axis (shared with column chart) */
    chart_series_set_name(series, "=Sheet1!$C$1");

    /* Combine the charts. The line chart is overlaid on the column chart. */
    chart_combine(column_chart, line_chart);

    /* Add a chart title and axis labels. Note: these are set on the
     * primary chart. */
    chart_title_set_name(column_chart, "Combined chart - same Y axis");
    chart_axis_set_name(column_chart->x_axis, "Test number");
    chart_axis_set_name(column_chart->y_axis, "Sample length (mm)");

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("E2"), column_chart);


    /*
     * Chart 2. Create a combined column and line chart with secondary Y axis.
     *
     * In this example we create a similar combined column and line chart
     * except that the line chart has a secondary Y axis on the right side.
     */

    /* Create a column chart - this will be the primary chart. */
    lxw_chart *column_chart2 = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Add the first series to the primary chart. */
    series = chart_add_series(column_chart2,
        "=Sheet1!$A$2:$A$7",
        "=Sheet1!$B$2:$B$7",
        0);  /* Primary Y axis */
    chart_series_set_name(series, "=Sheet1!$B$1");

    /* Create a line chart - this will be the secondary (combined) chart. */
    lxw_chart *line_chart2 = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* Add the first series to the secondary chart.
     *
     * Set y2_axis = 1 to place this series on the secondary Y axis.
     * This tells the library to:
     * 1. Use secondary axis IDs for this series
     * 2. Write the secondary axis elements (Y2 on right, X2 hidden)
     */
    series = chart_add_series(line_chart2,
        "=Sheet1!$A$2:$A$7",
        "=Sheet1!$C$2:$C$7",
        1);  /* y2_axis = 1: Use secondary Y axis */
    chart_series_set_name(series, "=Sheet1!$C$1");

    /* Combine the charts. */
    chart_combine(column_chart2, line_chart2);

    /* Add a chart title and axis labels. */
    chart_title_set_name(column_chart2, "Combined chart - secondary Y axis");
    chart_axis_set_name(column_chart2->x_axis, "Test number");
    chart_axis_set_name(column_chart2->y_axis, "Sample length (mm)");

    /* Configure the secondary Y axis (accessed via the primary chart).
     * The secondary Y axis appears on the right side of the chart. */
    chart_axis_set_name(column_chart2->y2_axis, "Target length (mm)");

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("E18"), column_chart2);


    return workbook_close(workbook);
}
