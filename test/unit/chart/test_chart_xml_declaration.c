/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/chart.h"

// Test _xml_declaration().
CTEST(chart, xml_declaration) {

    char* got;
    char exp[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    FILE* testfile = lxw_tmpfile(NULL);

    lxw_chart *chart = lxw_chart_new(LXW_CHART_NONE);
    chart->file = testfile;

    _chart_xml_declaration(chart);

    RUN_XLSX_STREQ(exp, got);

    lxw_chart_free(chart);
}
