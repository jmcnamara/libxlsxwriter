/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_border() function.
CTEST(styles, write_border) {

    char* got;
    char exp[] = "<border><left/><right/><top/><bottom/><diagonal/></border>";
    FILE* testfile = lxw_tmpfile(NULL);

    lxw_styles *styles = lxw_styles_new();
    lxw_format *format = lxw_format_new();

    styles->file = testfile;

    _write_border(styles, format);

    RUN_XLSX_STREQ(exp, got);

    lxw_styles_free(styles);
}

