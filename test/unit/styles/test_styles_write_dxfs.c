/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_dxfs() function.
CTEST(styles, write_dxfs) {

    char* got;
    char exp[] = "<dxfs count=\"0\"/>";
    FILE* testfile = tmpfile();

    lxw_styles *styles = _new_styles();
    styles->file = testfile;

    _write_dxfs(styles);

    RUN_XLSX_STREQ(exp, got);

    _free_styles(styles);
}

