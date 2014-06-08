/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include <gtest/gtest.h>
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_name() function.
TEST(styles, write_name) {


    char* got;
    char exp[] = "<name val=\"Calibri\"/>";
    FILE* testfile = tmpfile();

    lxw_styles *styles = _new_styles();
    styles->file = testfile;

    _write_font_name(styles, "Calibri");

    RUN_XLSX_STREQ(exp, got);

    _free_styles(styles);
}

