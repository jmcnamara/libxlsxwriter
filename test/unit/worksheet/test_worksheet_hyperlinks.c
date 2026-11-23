/*
 * Tests for the lib_xlsx_writer library.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "../../../include/xlsxwriter/workbook.h"
#include "../../../include/xlsxwriter/worksheet.h"


/* Test that the hyperlink type is only set from a URL scheme prefix and not
 * from a scheme string that appears elsewhere in the URL. */
CTEST(worksheet, get_hyperlink_type) {

    ASSERT_EQUAL(HYPERLINK_INTERNAL, _get_hyperlink_type("internal:Sheet2!A1"));
    ASSERT_EQUAL(HYPERLINK_EXTERNAL, _get_hyperlink_type("external:c:\\temp\\foo.xlsx"));
    ASSERT_EQUAL(HYPERLINK_URL,      _get_hyperlink_type("https://www.perl.org/"));
    ASSERT_EQUAL(HYPERLINK_URL,      _get_hyperlink_type("mailto:jmcnamara@cpan.org"));

    ASSERT_EQUAL(HYPERLINK_URL,      _get_hyperlink_type("https://example.com/?q=internal:foo"));
    ASSERT_EQUAL(HYPERLINK_URL,      _get_hyperlink_type("https://example.com/?q=external:foo"));
    ASSERT_EQUAL(HYPERLINK_URL,      _get_hyperlink_type("internal"));
    ASSERT_EQUAL(HYPERLINK_URL,      _get_hyperlink_type(""));
}


/* Test that URLs containing a scheme string after the start are written as
 * standard URLs, with the full URL as the display string. Only a leading
 * "mailto:" is stripped from the display string. */
CTEST(worksheet, write_hyperlinks01) {
    char* got;
    char exp[] =
        "<hyperlinks>"
        "<hyperlink ref=\"A1\" r:id=\"rId1\"/>"
        "<hyperlink ref=\"A2\" r:id=\"rId2\"/>"
        "<hyperlink ref=\"A3\" r:id=\"rId3\"/>"
        "<hyperlink ref=\"A4\" r:id=\"rId4\"/>"
        "</hyperlinks>";
    FILE* testfile = lxw_tmpfile(NULL);
    lxw_row *row;
    lxw_cell *cell;
    lxw_rel_tuple *relationship;

    lxw_workbook *workbook = workbook_new(NULL);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    worksheet->file = testfile;

    worksheet_write_url(worksheet, 0, 0, "https://example.com/?q=internal:foo", NULL);
    worksheet_write_url(worksheet, 1, 0, "https://example.com/?q=external:foo", NULL);
    worksheet_write_url(worksheet, 2, 0, "https://example.com/mailto:foo", NULL);
    worksheet_write_url(worksheet, 3, 0, "mailto:jmcnamara@cpan.org", NULL);

    /* Check the cell display strings. */
    row = lxw_worksheet_find_row(worksheet, 0);
    cell = lxw_worksheet_find_cell_in_row(row, 0);
    ASSERT_STR("https://example.com/?q=internal:foo", cell->sst_string);

    row = lxw_worksheet_find_row(worksheet, 1);
    cell = lxw_worksheet_find_cell_in_row(row, 0);
    ASSERT_STR("https://example.com/?q=external:foo", cell->sst_string);

    row = lxw_worksheet_find_row(worksheet, 2);
    cell = lxw_worksheet_find_cell_in_row(row, 0);
    ASSERT_STR("https://example.com/mailto:foo", cell->sst_string);

    row = lxw_worksheet_find_row(worksheet, 3);
    cell = lxw_worksheet_find_cell_in_row(row, 0);
    ASSERT_STR("jmcnamara@cpan.org", cell->sst_string);

    _worksheet_write_hyperlinks(worksheet);

    /* Check the first relationship target is the full URL. */
    relationship = STAILQ_FIRST(worksheet->external_hyperlinks);
    ASSERT_STR("https://example.com/?q=internal:foo", relationship->target);

    RUN_XLSX_STREQ(exp, got);

    lxw_workbook_free(workbook);
}
