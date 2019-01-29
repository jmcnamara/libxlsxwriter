/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/chartsheet.h"
#include "xlsxwriter/drawing.h"

// Test assembling a complete Chartsheet file.
CTEST(chartsheet, chartsheet) {

    char* got;
    char exp[] =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<chartsheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">"
          "<sheetPr/>"
          "<sheetViews>"
            "<sheetView workbookViewId=\"0\"/>"
          "</sheetViews>"
          "<pageMargins left=\"0.7\" right=\"0.7\" top=\"0.75\" bottom=\"0.75\" header=\"0.3\" footer=\"0.3\"/>"
          "<drawing r:id=\"rId1\"/>"
        "</chartsheet>";

    FILE* testfile = lxw_tmpfile(NULL);

    lxw_chartsheet *chartsheet = lxw_chartsheet_new(NULL);
    chartsheet->file = testfile;
    chartsheet->worksheet->drawing = lxw_drawing_new();

    lxw_chartsheet_assemble_xml_file(chartsheet);

    RUN_XLSX_STREQ_SHORT(exp, got);

    lxw_chartsheet_free(chartsheet);
}
