/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_default_fill() function.
CTEST(styles, write_default_fill) {

    char* got;
    char exp[] = "<fill><patternFill patternType=\"none\"/></fill>";
    FILE* testfile = tmpfile();

    lxw_styles *styles = _new_styles();
    styles->file = testfile;

    _write_default_fill(styles, "none");

    RUN_XLSX_STREQ(exp, got);

    _free_styles(styles);
}

