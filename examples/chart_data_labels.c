/*
 * A demo of an various Excel chart data label features that are available via
 * a libxlsxwriter chart.
 *
 * Copyright 2014-2020, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

/*
 * Create a worksheet with examples charts.
 */
int main() {

    lxw_workbook     *workbook  = workbook_new("chart_data_labels.xlsx");
    lxw_worksheet    *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Add a bold format to use to highlight the header cells. */
    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    /* Some chart positioning options. */
    lxw_chart_options options = {.x_offset = 25,  .y_offset = 10};

    /* Write some data for the chart. */
    worksheet_write_string(worksheet, 0, 0, "Number",  bold);
    worksheet_write_number(worksheet, 1, 0, 2,         NULL);
    worksheet_write_number(worksheet, 2, 0, 3,         NULL);
    worksheet_write_number(worksheet, 3, 0, 4,         NULL);
    worksheet_write_number(worksheet, 4, 0, 5,         NULL);
    worksheet_write_number(worksheet, 5, 0, 6,         NULL);
    worksheet_write_number(worksheet, 6, 0, 7,         NULL);

    worksheet_write_string(worksheet, 0, 1, "Data",    bold);
    worksheet_write_number(worksheet, 1, 1, 20,        NULL);
    worksheet_write_number(worksheet, 2, 1, 10,        NULL);
    worksheet_write_number(worksheet, 3, 1, 20,        NULL);
    worksheet_write_number(worksheet, 4, 1, 30,        NULL);
    worksheet_write_number(worksheet, 5, 1, 40,        NULL);
    worksheet_write_number(worksheet, 6, 1, 30,        NULL);

    worksheet_write_string(worksheet, 0, 2, "Text",    bold);
    worksheet_write_string(worksheet, 1, 2, "Jan",     NULL);
    worksheet_write_string(worksheet, 2, 2, "Feb",     NULL);
    worksheet_write_string(worksheet, 3, 2, "Mar",     NULL);
    worksheet_write_string(worksheet, 4, 2, "Apr",     NULL);
    worksheet_write_string(worksheet, 5, 2, "May",     NULL);
    worksheet_write_string(worksheet, 6, 2, "Jun",     NULL);


    /*
     * Chart 1. Example with standard data labels.
     */
    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Add a chart title. */
    chart_title_set_name(chart, "Chart with standard data labels");

    /* Add a data series to the chart. */
    lxw_chart_series *series = chart_add_series(chart, "=Sheet1!$A$2:$A$7",
                                                       "=Sheet1!$B$2:$B$7");

    /* Add the series data labels. */
    chart_series_set_labels(series);

    /* Turn off the legend. */
    chart_legend_set_position(chart, LXW_CHART_LEGEND_NONE);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart_opt(worksheet, CELL("D2"), chart, &options);


    /*
     * Chart 2. Example with value and category data labels.
     */
    chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Add a chart title. */
    chart_title_set_name(chart, "Category and Value data labels");

    /* Add a data series to the chart. */
    series = chart_add_series(chart, "=Sheet1!$A$2:$A$7",
                                     "=Sheet1!$B$2:$B$7");

    /* Add the series data labels. */
    chart_series_set_labels(series);

    /* Turn on Value and Category labels. */
    chart_series_set_labels_options(series, LXW_FALSE, LXW_TRUE, LXW_TRUE);

    /* Turn off the legend. */
    chart_legend_set_position(chart, LXW_CHART_LEGEND_NONE);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart_opt(worksheet, CELL("D18"), chart, &options);


    /*
     * Chart 3. Example with standard data labels with different font.
     */
    chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Add a chart title. */
    chart_title_set_name(chart, "Data labels with user defined font");

    /* Add a data series to the chart. */
    series = chart_add_series(chart, "=Sheet1!$A$2:$A$7",
                                     "=Sheet1!$B$2:$B$7");

    /* Add the series data labels. */
    chart_series_set_labels(series);

    lxw_chart_font font1 = {.bold = LXW_TRUE, .color = LXW_COLOR_RED, .rotation = -30};
    chart_series_set_labels_font(series, &font1);

    /* Turn off the legend. */
    chart_legend_set_position(chart, LXW_CHART_LEGEND_NONE);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart_opt(worksheet, CELL("D34"), chart, &options);


    /*
     * Chart 4.Example with custom string data labels.
     */
    chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Add a chart title. */
    chart_title_set_name(chart, "Chart with custom string data labels");

    /* Add a data series to the chart. */
    series = chart_add_series(chart, "=Sheet1!$A$2:$A$7",
                                     "=Sheet1!$B$2:$B$7");

    /* Add the series data labels. */
    chart_series_set_labels(series);

    /* Create some custom labels. */
    lxw_chart_data_label data_label41 = {.value = "Amy"};
    lxw_chart_data_label data_label42 = {.value = "Bea"};
    lxw_chart_data_label data_label43 = {.value = "Eva"};
    lxw_chart_data_label data_label44 = {.value = "Fay"};
    lxw_chart_data_label data_label45 = {.value = "Liv"};
    lxw_chart_data_label data_label46 = {.value = "Una"};

    /* Create an array of label pointers. NULL indicates the end of the array. */
    lxw_chart_data_label *data_labels4[] = {
        &data_label41,
        &data_label42,
        &data_label43,
        &data_label44,
        &data_label45,
        &data_label46,
        NULL
    };

    /* Set the custom labels. */
    chart_series_set_labels_custom(series, data_labels4);

    /* Turn off the legend. */
    chart_legend_set_position(chart, LXW_CHART_LEGEND_NONE);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart_opt(worksheet, CELL("D50"), chart, &options);


    /*
     * Chart 5. Example with custom data labels from cells.
     */
    chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Add a chart title. */
    chart_title_set_name(chart, "Chart with custom data labels from cells");

    /* Add a data series to the chart. */
    series = chart_add_series(chart, "=Sheet1!$A$2:$A$7",
                                     "=Sheet1!$B$2:$B$7");

    /* Add the series data labels. */
    chart_series_set_labels(series);

    /* Create some custom labels. */
    lxw_chart_data_label data_label51 = {.value = "=Sheet1!$C$2"};
    lxw_chart_data_label data_label52 = {.value = "=Sheet1!$C$3"};
    lxw_chart_data_label data_label53 = {.value = "=Sheet1!$C$4"};
    lxw_chart_data_label data_label54 = {.value = "=Sheet1!$C$5"};
    lxw_chart_data_label data_label55 = {.value = "=Sheet1!$C$6"};
    lxw_chart_data_label data_label56 = {.value = "=Sheet1!$C$7"};

    /* Create an array of label pointers. NULL indicates the end of the array. */
    lxw_chart_data_label *data_labels5[] = {
        &data_label51,
        &data_label52,
        &data_label53,
        &data_label54,
        &data_label55,
        &data_label56,
        NULL
    };

    /* Set the custom labels. */
    chart_series_set_labels_custom(series, data_labels5);

    /* Turn off the legend. */
    chart_legend_set_position(chart, LXW_CHART_LEGEND_NONE);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart_opt(worksheet, CELL("D66"), chart, &options);


    /*
     * Chart 6. Example with custom and default data labels.
     */
    chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Add a chart title. */
    chart_title_set_name(chart, "Mixed custom and default data labels");

    /* Add a data series to the chart. */
    series = chart_add_series(chart, "=Sheet1!$A$2:$A$7",
                                     "=Sheet1!$B$2:$B$7");

    lxw_chart_font font2 = {.color = LXW_COLOR_RED};

    /* Add the series data labels. */
    chart_series_set_labels(series);

    /* Create some custom labels. */

    /* The following is used to get a mix of default and custom labels. The
     * items initialized with '{0}' and items without a custom label (points 5
     * and 6 which come after NULL) will get the default value. We also set a
     * font for the custom items as an extra example.
     */
    lxw_chart_data_label data_label61 = {.value = "=Sheet1!$C$2", .font = &font2};
    lxw_chart_data_label data_label62 = {0};
    lxw_chart_data_label data_label63 = {.value = "=Sheet1!$C$4", .font = &font2};
    lxw_chart_data_label data_label64 = {.value = "=Sheet1!$C$5", .font = &font2};

    /* Create an array of label pointers. NULL indicates the end of the array. */
    lxw_chart_data_label *data_labels6[] = {
        &data_label61,
        &data_label62,
        &data_label63,
        &data_label64,
        NULL
    };

    /* Set the custom labels. */
    chart_series_set_labels_custom(series, data_labels6);

    /* Turn off the legend. */
    chart_legend_set_position(chart, LXW_CHART_LEGEND_NONE);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart_opt(worksheet, CELL("D82"), chart, &options);


    /*
     * Chart 7. Example with deleted/hidden custom data labels.
     */
    chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    /* Add a chart title. */
    chart_title_set_name(chart, "Chart with deleted data labels");

    /* Add a data series to the chart. */
    series = chart_add_series(chart, "=Sheet1!$A$2:$A$7",
                                     "=Sheet1!$B$2:$B$7");

    /* Add the series data labels. */
    chart_series_set_labels(series);

    /* Create some custom labels. */
    lxw_chart_data_label hide = {.hide = LXW_TRUE};
    lxw_chart_data_label keep = {.hide = LXW_FALSE};

    /* An initialized struct like this would also work: */
    /* lxw_chart_data_label keep = {0}; */

    /* Create an array of label pointers. NULL indicates the end of the array. */
    lxw_chart_data_label *data_labels7[] = {
        &hide,
        &keep,
        &hide,
        &hide,
        &keep,
        &hide,
        NULL
    };

    /* Set the custom labels. */
    chart_series_set_labels_custom(series, data_labels7);

    /* Turn off the legend. */
    chart_legend_set_position(chart, LXW_CHART_LEGEND_NONE);

    /* Insert the chart into the worksheet. */
    worksheet_insert_chart_opt(worksheet, CELL("D98"), chart, &options);


    return workbook_close(workbook);
}
