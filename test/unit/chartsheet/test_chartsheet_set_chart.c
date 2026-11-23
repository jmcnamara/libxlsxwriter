/*
 * Tests for the lib_xlsx_writer library.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "../../../include/xlsxwriter/workbook.h"
#include "../../../include/xlsxwriter/chartsheet.h"


/* Test that a scatter chart series without categories, where the ranges are
 * set with chart_series_set_values(), is rejected. */
CTEST(chartsheet, set_chart_scatter_no_categories) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_chartsheet *chartsheet = workbook_add_chartsheet(workbook, NULL);
    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_SCATTER);
    lxw_chart_series *series = chart_add_series(chart, NULL, NULL);

    chart_series_set_values(series, "Sheet1", 0, 1, 4, 1);

    ASSERT_EQUAL(LXW_ERROR_PARAMETER_VALIDATION,
                 chartsheet_set_chart(chartsheet, chart));

    lxw_workbook_free(workbook);
}


/* Test that a scatter chart series with categories and values set with the
 * chart_series_set_*() functions is accepted. */
CTEST(chartsheet, set_chart_scatter_with_categories) {
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_chartsheet *chartsheet = workbook_add_chartsheet(workbook, NULL);
    lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_SCATTER);
    lxw_chart_series *series = chart_add_series(chart, NULL, NULL);

    chart_series_set_categories(series, "Sheet1", 0, 0, 4, 0);
    chart_series_set_values(series, "Sheet1", 0, 1, 4, 1);

    ASSERT_EQUAL(LXW_NO_ERROR, chartsheet_set_chart(chartsheet, chart));

    lxw_workbook_free(workbook);
}
