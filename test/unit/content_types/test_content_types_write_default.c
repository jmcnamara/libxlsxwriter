/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/content_types.h"

// Test the _write_default() function.
CTEST(content_types, write_default) {

    char* got;
    char exp[] = "<Default Extension=\"xml\" ContentType=\"application/xml\"/>";
    FILE* testfile = tmpfile();

    lxw_content_types *content_types = _new_content_types();
    content_types->file = testfile;

    _write_default(content_types, "xml", "application/xml");

    RUN_XLSX_STREQ(exp, got);

    _free_content_types(content_types);
}

