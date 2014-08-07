/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/content_types.h"

// Test _xml_declaration().
CTEST(content_types, xml_declaration) {

    char* got;
    char exp[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    FILE* testfile = tmpfile();

    lxw_content_types *content_types = _new_content_types();
    content_types->file = testfile;

    _content_types_xml_declaration(content_types);

    RUN_XLSX_STREQ(exp, got);

    _free_content_types(content_types);
}
