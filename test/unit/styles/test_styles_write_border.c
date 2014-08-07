/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_border() function.
CTEST(styles, write_border) {

    char* got;
    char exp[] = "<border><left/><right/><top/><bottom/><diagonal/></border>";
    FILE* testfile = tmpfile();

    lxw_styles *styles = _new_styles();
    lxw_format *format = _new_format();

    styles->file = testfile;

    _write_border(styles, format);

    RUN_XLSX_STREQ(exp, got);

    _free_styles(styles);
}

