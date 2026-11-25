/*
 * Tests for the libxlsxwriter library.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "../../../include/xlsxwriter/workbook.h"
#include "../../../include/xlsxwriter/worksheet.h"


/* Test a valid Column and Line combined chart. */
CTEST(chart, combine_column_line) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_chart *chart1 = workbook_add_chart(workbook, LXW_CHART_COLUMN);
    lxw_chart *chart2 = workbook_add_chart(workbook, LXW_CHART_LINE);

    ASSERT_EQUAL(LXW_NO_ERROR, chart_combine(chart1, chart2));
    ASSERT_TRUE(chart1->combined == chart2);

    lxw_workbook_free(workbook);
}


/* Test a valid Doughnut and Pie combined chart. */
CTEST(chart, combine_doughnut_pie) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_chart *chart1 = workbook_add_chart(workbook, LXW_CHART_DOUGHNUT);
    lxw_chart *chart2 = workbook_add_chart(workbook, LXW_CHART_PIE);

    ASSERT_EQUAL(LXW_NO_ERROR, chart_combine(chart1, chart2));

    lxw_workbook_free(workbook);
}


/* Test a Scatter chart as the secondary chart, which is supported. */
CTEST(chart, combine_scatter_secondary) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_chart *chart1 = workbook_add_chart(workbook, LXW_CHART_COLUMN);
    lxw_chart *chart2 = workbook_add_chart(workbook, LXW_CHART_SCATTER);

    ASSERT_EQUAL(LXW_NO_ERROR, chart_combine(chart1, chart2));

    lxw_workbook_free(workbook);
}


/* Test NULL charts. */
CTEST(chart, combine_null) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_chart *chart1 = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    ASSERT_EQUAL(LXW_ERROR_NULL_PARAMETER_IGNORED,
                 chart_combine(chart1, NULL));
    ASSERT_EQUAL(LXW_ERROR_NULL_PARAMETER_IGNORED,
                 chart_combine(NULL, chart1));
    ASSERT_NULL(chart1->combined);

    lxw_workbook_free(workbook);
}


/* Test combining a chart with itself. */
CTEST(chart, combine_self) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_chart *chart1 = workbook_add_chart(workbook, LXW_CHART_COLUMN);

    ASSERT_EQUAL(LXW_ERROR_PARAMETER_VALIDATION,
                 chart_combine(chart1, chart1));
    ASSERT_NULL(chart1->combined);

    lxw_workbook_free(workbook);
}


/* Test that only two charts can be combined. */
CTEST(chart, combine_more_than_two) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_chart *chart1 = workbook_add_chart(workbook, LXW_CHART_COLUMN);
    lxw_chart *chart2 = workbook_add_chart(workbook, LXW_CHART_LINE);
    lxw_chart *chart3 = workbook_add_chart(workbook, LXW_CHART_AREA);

    ASSERT_EQUAL(LXW_NO_ERROR, chart_combine(chart1, chart2));

    /* Primary already has a combined chart. */
    ASSERT_EQUAL(LXW_ERROR_PARAMETER_VALIDATION,
                 chart_combine(chart1, chart3));

    /* Secondary chart is already combined into another chart. */
    ASSERT_EQUAL(LXW_ERROR_PARAMETER_VALIDATION,
                 chart_combine(chart3, chart2));

    /* Secondary chart already has a combined chart. */
    ASSERT_EQUAL(LXW_ERROR_PARAMETER_VALIDATION,
                 chart_combine(chart3, chart1));

    lxw_workbook_free(workbook);
}


/* Test that a chart that has been inserted can't be the secondary chart, and
 * that the secondary chart can't be inserted after combining. */
CTEST(chart, combine_inserted_chart) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    lxw_chart *chart1 = workbook_add_chart(workbook, LXW_CHART_COLUMN);
    lxw_chart *chart2 = workbook_add_chart(workbook, LXW_CHART_LINE);
    lxw_chart *chart3 = workbook_add_chart(workbook, LXW_CHART_LINE);

    chart_add_series(chart1, NULL, "=Sheet1!$A$1:$A$5");
    chart_add_series(chart2, NULL, "=Sheet1!$B$1:$B$5");
    chart_add_series(chart3, NULL, "=Sheet1!$C$1:$C$5");

    ASSERT_EQUAL(LXW_NO_ERROR,
                 worksheet_insert_chart(worksheet, 0, 0, chart2));

    ASSERT_EQUAL(LXW_ERROR_PARAMETER_VALIDATION,
                 chart_combine(chart1, chart2));

    ASSERT_EQUAL(LXW_NO_ERROR, chart_combine(chart1, chart3));

    ASSERT_EQUAL(LXW_ERROR_PARAMETER_VALIDATION,
                 worksheet_insert_chart(worksheet, 0, 10, chart3));

    lxw_workbook_free(workbook);
}


/* Test that a Scatter chart can't be the primary chart. */
CTEST(chart, combine_scatter_primary) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_chart *chart1 = workbook_add_chart(workbook, LXW_CHART_SCATTER);
    lxw_chart *chart2 = workbook_add_chart(workbook, LXW_CHART_LINE);

    ASSERT_EQUAL(LXW_ERROR_PARAMETER_VALIDATION,
                 chart_combine(chart1, chart2));

    lxw_workbook_free(workbook);
}


/* Test that Pie/Doughnut charts can't be combined with other chart types. */
CTEST(chart, combine_pie_mismatch) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_chart *chart1 = workbook_add_chart(workbook, LXW_CHART_PIE);
    lxw_chart *chart2 = workbook_add_chart(workbook, LXW_CHART_LINE);
    lxw_chart *chart3 = workbook_add_chart(workbook, LXW_CHART_COLUMN);
    lxw_chart *chart4 = workbook_add_chart(workbook, LXW_CHART_DOUGHNUT);

    ASSERT_EQUAL(LXW_ERROR_PARAMETER_VALIDATION,
                 chart_combine(chart1, chart2));

    ASSERT_EQUAL(LXW_ERROR_PARAMETER_VALIDATION,
                 chart_combine(chart3, chart4));

    lxw_workbook_free(workbook);
}


/* Test that the value/category only axis functions work on the secondary
 * axes. */
CTEST(chart, secondary_axis_types) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_chart *line = workbook_add_chart(workbook, LXW_CHART_LINE);
    lxw_chart *bar = workbook_add_chart(workbook, LXW_CHART_BAR);
    lxw_chart *scatter = workbook_add_chart(workbook, LXW_CHART_SCATTER);

    /* The y2 axis is a value axis, even for Bar charts. */
    chart_axis_set_log_base(line->y2_axis, 10);
    ASSERT_EQUAL(10, line->y2_axis->log_base);

    chart_axis_set_log_base(bar->y2_axis, 10);
    ASSERT_EQUAL(10, bar->y2_axis->log_base);

    /* The x2 axis is a category axis, except for Scatter charts. */
    ASSERT_TRUE(line->x2_axis->is_category);
    ASSERT_FALSE(line->x2_axis->is_value);

    chart_axis_set_log_base(scatter->x2_axis, 10);
    ASSERT_EQUAL(10, scatter->x2_axis->log_base);

    lxw_workbook_free(workbook);
}
