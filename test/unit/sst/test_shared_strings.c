/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/shared_strings.h"

// Test assembling a complete SharedStrings file.
CTEST(sst, sst01) {

    char* got;
    char exp[] =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<sst xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" count=\"7\" uniqueCount=\"3\">"
          "<si>"
            "<t>neptune</t>"
          "</si>"
          "<si>"
            "<t>mars</t>"
          "</si>"
          "<si>"
            "<t>venus</t>"
          "</si>"
        "</sst>";

    FILE* testfile = lxw_tmpfile(NULL);

    lxw_sst *sst = lxw_sst_new();
    sst->file = testfile;

    lxw_get_sst_index(sst, "neptune");
    lxw_get_sst_index(sst, "neptune");
    lxw_get_sst_index(sst, "neptune");
    lxw_get_sst_index(sst, "mars");
    lxw_get_sst_index(sst, "mars");
    lxw_get_sst_index(sst, "venus");
    lxw_get_sst_index(sst, "venus");

    lxw_sst_assemble_xml_file(sst);

    RUN_XLSX_STREQ_SHORT(exp, got);

    lxw_sst_free(sst);
}

// Test assembling a complete SharedStrings file.
CTEST(sst, sst02) {

    char* got;
    char exp[] =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<sst xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" count=\"3\" uniqueCount=\"3\">"
          "<si>"
            "<t>abcdefg</t>"
          "</si>"
          "<si>"
            "<t xml:space=\"preserve\">   abcdefg</t>"
          "</si>"
          "<si>"
            "<t xml:space=\"preserve\">abcdefg   </t>"
          "</si>"
        "</sst>";

    FILE* testfile = lxw_tmpfile(NULL);

    lxw_sst *sst = lxw_sst_new();
    sst->file = testfile;

    // Test strings with whitespace that must be preserved.
    lxw_get_sst_index(sst, "abcdefg");
    lxw_get_sst_index(sst, "   abcdefg");
    lxw_get_sst_index(sst, "abcdefg   ");

    lxw_sst_assemble_xml_file(sst);

    RUN_XLSX_STREQ_SHORT(exp, got);

    lxw_sst_free(sst);
}

