/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/table.h"

// Test _xml_declaration().
CTEST(table, xml_declaration) {

    char* got;
    char exp[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    FILE* testfile = tmpfile();

    lxw_table *table = lxw_table_new();
    table->file = testfile;

    _table_xml_declaration(table);

    RUN_XLSX_STREQ(exp, got);

    lxw_table_free(table);
}
