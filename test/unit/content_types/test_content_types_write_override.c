/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/content_types.h"

// Test the _write_override() function.
CTEST(content_types, write_override) {

    char* got;
    char exp[] = "<Override PartName=\"/docProps/core.xml\" ContentType=\"app...\"/>";
    FILE* testfile = tmpfile();

    lxw_content_types *content_types = _new_content_types();
    content_types->file = testfile;

    _write_override(content_types, "/docProps/core.xml", "app...");

    RUN_XLSX_STREQ(exp, got);

    _free_content_types(content_types);
}

