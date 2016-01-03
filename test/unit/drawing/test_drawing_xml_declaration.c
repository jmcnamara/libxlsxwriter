/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/drawing.h"

// Test _xml_declaration().
CTEST(drawing, xml_declaration) {

    char* got;
    char exp[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    FILE* testfile = tmpfile();

    lxw_drawing *drawing = _new_drawing();
    drawing->file = testfile;

    _drawing_xml_declaration(drawing);

    RUN_XLSX_STREQ(exp, got);

    _free_drawing(drawing);
}
