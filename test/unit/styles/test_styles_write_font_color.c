/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_color() function.
CTEST(styles, write_color) {


    char* got;
    char exp[] = "<color theme=\"1\"/>";
    FILE* testfile = lxw_tmpfile(NULL);

    lxw_styles *styles = lxw_styles_new();
    styles->file = testfile;

    _write_font_color_theme(styles, 1);

    RUN_XLSX_STREQ(exp, got);

    lxw_styles_free(styles);
}

