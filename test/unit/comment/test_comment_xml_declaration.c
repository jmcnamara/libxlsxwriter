/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "../../../include/xlsxwriter/comment.h"

// Test _xml_declaration().
CTEST(comment, xml_declaration) {

    char* got;
    char exp[] = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    FILE* testfile = tmpfile();

    lxw_comment *comment = lxw_comment_new();
    comment->file = testfile;

    _comment_xml_declaration(comment);

    RUN_XLSX_STREQ(exp, got);

    lxw_comment_free(comment);
}
