/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include <gtest/gtest.h>
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_color() function.
TEST(styles, write_color) {


    char* got;
    char exp[] = "<color theme=\"1\"/>";
    FILE* testfile = tmpfile();

    lxw_styles *styles = _new_styles();
    styles->file = testfile;

    _write_font_color_theme(styles, 1);

    RUN_XLSX_STREQ(exp, got);

    _free_styles(styles);
}

