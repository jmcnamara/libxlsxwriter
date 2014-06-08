/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include <gtest/gtest.h>
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_style_sheet() function.
TEST(styles, write_style_sheet) {


    char* got;
    char exp[] = "<styleSheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">";
    FILE* testfile = tmpfile();

    lxw_styles *styles = _new_styles();
    styles->file = testfile;

    _write_style_sheet(styles);

    RUN_XLSX_STREQ(exp, got);

    _free_styles(styles);
}

