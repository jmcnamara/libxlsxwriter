/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_fills() function.
CTEST(styles, write_fills) {

    char* got;
    char exp[] = "<fills count=\"2\"><fill><patternFill patternType=\"none\"/></fill><fill><patternFill patternType=\"gray125\"/></fill></fills>";
    FILE* testfile = lxw_tmpfile(NULL);

    lxw_styles *styles = lxw_styles_new();
    styles->fill_count = 2;
    styles->file = testfile;

    _write_fills(styles);

    RUN_XLSX_STREQ(exp, got);

    lxw_styles_free(styles);
}

