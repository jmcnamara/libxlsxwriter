/*
 * An example of creating a Pareto chart with libxlsxwriter.
 *
 * A Pareto chart is a type of chart that combines a column chart with a line
 * chart. The columns represent individual values in descending order, and the
 * line represents the cumulative percentage.
 *
 * This example uses combined charts with a secondary Y axis.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("chart_pareto.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Add formats for the worksheet data. */
    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    lxw_format *percent_format = workbook_add_format(workbook);
    format_set_num_format(percent_format, "0.0%");

    /* Widen the columns for visibility. */
    worksheet_set_column(worksheet, COLS("A:A"), 15, NULL);
    worksheet_set_column(worksheet, COLS("B:C"), 10, NULL);

    /* Add the worksheet data that the charts will refer to. */
    worksheet_write_string(worksheet, 0, 0, "Reason",     bold);
    worksheet_write_string(worksheet, 0, 1, "Number",     bold);
    worksheet_write_string(worksheet, 0, 2, "Percentage", bold);

    /* Reasons for lateness data. */
    worksheet_write_string(worksheet, 1, 0, "Traffic", NULL);
    worksheet_write_string(worksheet, 2, 0, "Child care", NULL);
    worksheet_write_string(worksheet, 3, 0, "Public Transport", NULL);
    worksheet_write_string(worksheet, 4, 0, "Weather", NULL);
    worksheet_write_string(worksheet, 5, 0, "Overslept", NULL);
    worksheet_write_string(worksheet, 6, 0, "Emergency", NULL);

    /* Number of occurrences. */
    worksheet_write_number(worksheet, 1, 1, 60, NULL);
    worksheet_write_number(worksheet, 2, 1, 40, NULL);
    worksheet_write_number(worksheet, 3, 1, 20, NULL);
    worksheet_write_number(worksheet, 4, 1, 15, NULL);
    worksheet_write_number(worksheet, 5, 1, 10, NULL);
    worksheet_write_number(worksheet, 6, 1,  5, NULL);

    /* Cumulative percentages. */
    worksheet_write_number(worksheet, 1, 2, 0.400, percent_format);
    worksheet_write_number(worksheet, 2, 2, 0.667, percent_format);
    worksheet_write_number(worksheet, 3, 2, 0.800, percent_format);
    worksheet_write_number(worksheet, 4, 2, 0.900, percent_format);
    worksheet_write_number(worksheet, 5, 2, 0.967, percent_format);
    worksheet_write_number(worksheet, 6, 2, 1.000, percent_format);

    /* Create a new column chart. This will be the primary chart. */
    lxw_chart *column_chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Add a series for the column chart. */
    chart_add_series(column_chart,
                     "=Sheet1!$A$2:$A$7",
                     "=Sheet1!$B$2:$B$7", 0);

    /* Add a chart title. */
    chart_title_set_name(column_chart, "Reasons for lateness");

    /* Turn off the chart legend. */
    chart_legend_set_position(column_chart, LXW_CHART_LEGEND_NONE);

    /* Set the title and scale of the Y axes. */
    chart_axis_set_name(column_chart->y_axis, "Respondents (number)");
    chart_axis_set_min(column_chart->y_axis, 0);
    chart_axis_set_max(column_chart->y_axis, 120);
    chart_axis_set_max(column_chart->y2_axis, 1);

    /* Create a new line chart. This will be the secondary chart. */
    lxw_chart *line_chart = workbook_add_chart(workbook, LXW_CHART_LINE);

    /* Mark the line chart as secondary so it uses the secondary axes. */
    line_chart->is_secondary = LXW_TRUE;

    /* Add a series for the line chart, on the secondary axis. */
    lxw_chart_series *series = chart_add_series(line_chart,
                                                "=Sheet1!$A$2:$A$7",
                                                "=Sheet1!$C$2:$C$7", 1);

    /* Add markers to the line series. */
    chart_series_set_marker_type(series, LXW_CHART_MARKER_AUTOMATIC);

    /* Combine the column and line charts. */
    chart_combine(column_chart, line_chart);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart(worksheet, CELL("F2"), column_chart);

    return workbook_close(workbook);
}
