/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_scheme() function.
CTEST(styles, write_scheme) {


    char* got;
    char exp[] = "<scheme val=\"minor\"/>";
    FILE* testfile = tmpfile();

    lxw_styles *styles = _new_styles();
    styles->file = testfile;

    _write_font_scheme(styles, "minor");

    RUN_XLSX_STREQ(exp, got);

    _free_styles(styles);
}

