/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/app.h"

// Test _xml_declaration().
CTEST(app, xml_declaration) {

    char* got;
    char exp[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    FILE* testfile = tmpfile();

    lxw_app *app = _new_app();
    app->file = testfile;

    _app_xml_declaration(app);

    RUN_XLSX_STREQ(exp, got);

    _free_app(app);
}
