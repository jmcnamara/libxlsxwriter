/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2019, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/workbook.h"
#include "xlsxwriter/shared_strings.h"

/* Test a valid sheet name. */
CTEST(workbook, validate_worksheet_name01) {

    const char* sheetname = "123456789_123456789_123456789_1";

    lxw_workbook *workbook = workbook_new(NULL);
    lxw_error exp = LXW_NO_ERROR;
    lxw_error got = workbook_validate_sheet_name(workbook, sheetname);

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}

/* Test a sheet name that is too long. */
CTEST(workbook, validate_worksheet_name02) {

    const char* sheetname = "123456789_123456789_123456789_12";

    lxw_workbook *workbook = workbook_new(NULL);
    lxw_error exp = LXW_ERROR_SHEETNAME_LENGTH_EXCEEDED;
    lxw_error got = workbook_validate_sheet_name(workbook, sheetname);

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}

/* Test a sheet name contains invalid characters. */
CTEST(workbook, validate_worksheet_name03) {

    const char* sheetname = "Sheet[1]";

    lxw_workbook *workbook = workbook_new(NULL);
    lxw_error exp = LXW_ERROR_INVALID_SHEETNAME_CHARACTER;
    lxw_error got = workbook_validate_sheet_name(workbook, sheetname);

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}

/* Test a sheet name that already exists. */
CTEST(workbook, validate_worksheet_name04) {

    const char* sheetname = "Sheet1";

    lxw_workbook *workbook = workbook_new(NULL);
    workbook_add_worksheet(workbook, sheetname);

    lxw_error exp = LXW_ERROR_SHEETNAME_ALREADY_USED;
    lxw_error got = workbook_validate_sheet_name(workbook, sheetname);

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}

/* Test a sheet name that starts with an apostrophe. */
CTEST(workbook, validate_worksheet_name05) {

    const char* sheetname = "'Sheet1";

    lxw_workbook *workbook = workbook_new(NULL);
    lxw_error exp = LXW_ERROR_SHEETNAME_START_END_APOSTROPHE;
    lxw_error got = workbook_validate_sheet_name(workbook, sheetname);

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}

/* Test a sheet name that ends with an apostrophe. */
CTEST(workbook, validate_worksheet_name06) {

    const char* sheetname = "Sheet1'";

    lxw_workbook *workbook = workbook_new(NULL);
    lxw_error exp = LXW_ERROR_SHEETNAME_START_END_APOSTROPHE;
    lxw_error got = workbook_validate_sheet_name(workbook, sheetname);

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}

/* Test a sheet that has the reserved name "History". */
CTEST(workbook, validate_worksheet_name07) {

    const char* sheetname = "History";

    lxw_workbook *workbook = workbook_new(NULL);
    lxw_error exp = LXW_ERROR_SHEETNAME_RESERVED;
    lxw_error got = workbook_validate_sheet_name(workbook, sheetname);

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}

/* Test a sheet that has the reserved name "History", case insensitive. */
CTEST(workbook, validate_worksheet_name08) {

    const char* sheetname = "history";

    lxw_workbook *workbook = workbook_new(NULL);
    lxw_error exp = LXW_ERROR_SHEETNAME_RESERVED;
    lxw_error got = workbook_validate_sheet_name(workbook, sheetname);

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}

/* Test a sheet that has the reserved name "History", case insensitive. */
CTEST(workbook, validate_worksheet_name09) {

    const char* sheetname = "HiStOrY";

    lxw_workbook *workbook = workbook_new(NULL);
    lxw_error exp = LXW_ERROR_SHEETNAME_RESERVED;
    lxw_error got = workbook_validate_sheet_name(workbook, sheetname);

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}

/* Test a sheet name that already exists, case insensitive. */
CTEST(workbook, validate_worksheet_name10) {

    const char* sheetname = "Sheet1";

    lxw_workbook *workbook = workbook_new(NULL);
    workbook_add_worksheet(workbook, sheetname);

    lxw_error exp = LXW_ERROR_SHEETNAME_ALREADY_USED;
    lxw_error got = workbook_validate_sheet_name(workbook, "sheet1");

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}

/* Test a sheet name that already exists, case insensitive. */
CTEST(workbook, validate_worksheet_name11) {

    const char* sheetname = "Café";

    lxw_workbook *workbook = workbook_new(NULL);
    workbook_add_worksheet(workbook, sheetname);

    lxw_error exp = LXW_ERROR_SHEETNAME_ALREADY_USED;
    lxw_error got = workbook_validate_sheet_name(workbook, "café");

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}

/* Test a sheet name that already exists, case insensitive. */
CTEST(workbook, validate_worksheet_name12) {

    const char* sheetname = "abcde";

    lxw_workbook *workbook = workbook_new(NULL);
    workbook_add_worksheet(workbook, sheetname);

    lxw_error exp = LXW_ERROR_SHEETNAME_ALREADY_USED;
    lxw_error got = workbook_validate_sheet_name(workbook, "ABCDE");

    ASSERT_EQUAL(exp, got);

    lxw_workbook_free(workbook);
}
