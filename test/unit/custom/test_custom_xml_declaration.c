/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/custom.h"

// Test _xml_declaration().
CTEST(custom, xml_declaration) {

    char* got;
    char exp[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    FILE* testfile = lxw_tmpfile(NULL);

    lxw_custom *custom = lxw_custom_new();
    custom->file = testfile;

    _custom_xml_declaration(custom);

    RUN_XLSX_STREQ(exp, got);

    lxw_custom_free(custom);
}
