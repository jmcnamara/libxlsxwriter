/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/workbook.h"

// Test _xml_declaration().
CTEST(workbook, xml_declaration) {

    char* got;
    char exp[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    FILE* testfile = tmpfile();

    lxw_workbook *workbook = new_workbook(NULL);
    workbook->file = testfile;

    _workbook_xml_declaration(workbook);

    RUN_XLSX_STREQ(exp, got);

    _free_workbook(workbook);
}
