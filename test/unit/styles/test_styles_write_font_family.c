/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_family() function.
CTEST(styles, write_family) {


    char* got;
    char exp[] = "<family val=\"2\"/>";
    FILE* testfile = tmpfile();

    lxw_styles *styles = _new_styles();
    styles->file = testfile;

    _write_font_family(styles, 2);

    RUN_XLSX_STREQ(exp, got);

    _free_styles(styles);
}

