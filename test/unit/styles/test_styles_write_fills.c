/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_fills() function.
CTEST(styles, write_fills) {

    char* got;
    char exp[] = "<fills count=\"2\"><fill><patternFill patternType=\"none\"/></fill><fill><patternFill patternType=\"gray125\"/></fill></fills>";
    FILE* testfile = tmpfile();

    lxw_styles *styles = _new_styles();
    styles->fill_count = 2;
    styles->file = testfile;

    _write_fills(styles);

    RUN_XLSX_STREQ(exp, got);

    _free_styles(styles);
}

