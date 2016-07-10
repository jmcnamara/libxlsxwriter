/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/relationships.h"

// Test _xml_declaration().
CTEST(relationships, xml_declaration) {

    char* got;
    char exp[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    FILE* testfile = lxw_tmpfile(NULL);

    lxw_relationships *relationships = lxw_relationships_new();
    relationships->file = testfile;

    _relationships_xml_declaration(relationships);

    RUN_XLSX_STREQ(exp, got);

    lxw_free_relationships(relationships);
}
