/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/core.h"

// Test _xml_declaration().
CTEST(core, xml_declaration) {

    char* got;
    char exp[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    FILE* testfile = lxw_tmpfile(NULL);

    lxw_core *core = lxw_core_new();
    core->file = testfile;

    _core_xml_declaration(core);

    RUN_XLSX_STREQ(exp, got);

    lxw_core_free(core);
}
