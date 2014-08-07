/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/styles.h"

// Test the _write_font() function.
CTEST(styles, write_fonts01) {

    char* got;
    char exp[] = "<fonts count=\"1\"><font><sz val=\"11\"/><color theme=\"1\"/><name val=\"Calibri\"/><family val=\"2\"/><scheme val=\"minor\"/></font></fonts>";
    FILE* testfile = tmpfile();

    lxw_styles *styles = _new_styles();
    lxw_format *format = _new_format();

    format->has_font = 1;

    STAILQ_INSERT_TAIL(styles->xf_formats, format, list_pointers);

    styles->file = testfile;
    styles->font_count = 1;

    _write_fonts(styles);

    RUN_XLSX_STREQ(exp, got);

    _free_styles(styles);
}
