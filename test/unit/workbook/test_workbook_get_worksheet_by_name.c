/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/workbook.h"
#include "xlsxwriter/shared_strings.h"

/* Test finding a worksheet that does exist (implicit naming). */
CTEST(workbook, get_worksheet_by_name01) {
    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *exp = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *got = workbook_get_worksheet_by_name(workbook, "Sheet1");

    ASSERT_TRUE(exp == got);

    lxw_workbook_free(workbook);
}

/* Test finding a worksheet that does exist (explicit naming). */
CTEST(workbook, get_worksheet_by_name02) {

    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *exp = workbook_add_worksheet(workbook, "FOO");
    lxw_worksheet *got = workbook_get_worksheet_by_name(workbook, "FOO");

    ASSERT_TRUE(exp == got);

    lxw_workbook_free(workbook);
}

/* Test finding a worksheet that doesn't exist. */
CTEST(workbook, get_worksheet_by_name03) {

    lxw_workbook *workbook = workbook_new(NULL);

    workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *got = workbook_get_worksheet_by_name(workbook, "FOO");

    ASSERT_TRUE(NULL == got);

    lxw_workbook_free(workbook);
}

/* Test finding a worksheet when no worksheets exist. */
CTEST(workbook, get_worksheet_by_name04) {

    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *got = workbook_get_worksheet_by_name(workbook, "FOO");

    ASSERT_TRUE(NULL == got);

    lxw_workbook_free(workbook);
}

/* Test finding a worksheet with a NULL name. */
CTEST(workbook, get_worksheet_by_name05) {

    lxw_workbook *workbook = workbook_new(NULL);

    workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *got = workbook_get_worksheet_by_name(workbook, NULL);

    ASSERT_TRUE(NULL == got);

    lxw_workbook_free(workbook);
}

