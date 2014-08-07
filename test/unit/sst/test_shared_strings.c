/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
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

    FILE* testfile = tmpfile();

    lxw_sst *sst = _new_sst();
    sst->file = testfile;

    _get_sst_index(sst, "neptune");
    _get_sst_index(sst, "neptune");
    _get_sst_index(sst, "neptune");
    _get_sst_index(sst, "mars");
    _get_sst_index(sst, "mars");
    _get_sst_index(sst, "venus");
    _get_sst_index(sst, "venus");

    _sst_assemble_xml_file(sst);

    RUN_XLSX_STREQ_SHORT(exp, got);

    _free_sst(sst);
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

    FILE* testfile = tmpfile();

    lxw_sst *sst = _new_sst();
    sst->file = testfile;

    // Test strings with whitespace that must be preserved.
    _get_sst_index(sst, "abcdefg");
    _get_sst_index(sst, "   abcdefg");
    _get_sst_index(sst, "abcdefg   ");

    _sst_assemble_xml_file(sst);

    RUN_XLSX_STREQ_SHORT(exp, got);

    _free_sst(sst);
}

