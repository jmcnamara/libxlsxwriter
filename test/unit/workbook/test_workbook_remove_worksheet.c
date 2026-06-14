/*
 * Tests for the libxlsxwriter library.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "../../../include/xlsxwriter/workbook.h"
#include "../../../include/xlsxwriter/shared_strings.h"


/* Test removing a worksheet by name. */
CTEST(workbook, remove_worksheet_basic) {
    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *ws1 = workbook_add_worksheet(workbook, "Sheet1");
    workbook_add_worksheet(workbook, "Sheet2");
    lxw_worksheet *ws3 = workbook_add_worksheet(workbook, "Sheet3");

    lxw_error got = workbook_remove_worksheet(workbook, "Sheet2");

    ASSERT_EQUAL(LXW_NO_ERROR, got);
    ASSERT_EQUAL(2, workbook->num_worksheets);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet1") == ws1);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet2") == NULL);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet3") == ws3);

    lxw_workbook_free(workbook);
}

/* Test that removing a middle worksheet renumbers the remaining indexes. */
CTEST(workbook, remove_worksheet_renumber_indexes) {
    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *ws1 = workbook_add_worksheet(workbook, "Sheet1");
    workbook_add_worksheet(workbook, "Sheet2");
    lxw_worksheet *ws3 = workbook_add_worksheet(workbook, "Sheet3");
    lxw_worksheet *ws4 = workbook_add_worksheet(workbook, "Sheet4");

    lxw_error got = workbook_remove_worksheet(workbook, "Sheet2");

    ASSERT_EQUAL(LXW_NO_ERROR, got);
    ASSERT_EQUAL(0, ws1->index);
    ASSERT_EQUAL(1, ws3->index);
    ASSERT_EQUAL(2, ws4->index);
    ASSERT_EQUAL(3, workbook->num_sheets);

    lxw_workbook_free(workbook);
}

/* Test that removing the first worksheet renumbers the remaining indexes. */
CTEST(workbook, remove_worksheet_first) {
    lxw_workbook *workbook = workbook_new(NULL);

    workbook_add_worksheet(workbook, "Sheet1");
    lxw_worksheet *ws2 = workbook_add_worksheet(workbook, "Sheet2");
    lxw_worksheet *ws3 = workbook_add_worksheet(workbook, "Sheet3");

    lxw_error got = workbook_remove_worksheet(workbook, "Sheet1");

    ASSERT_EQUAL(LXW_NO_ERROR, got);
    ASSERT_EQUAL(0, ws2->index);
    ASSERT_EQUAL(1, ws3->index);
    ASSERT_EQUAL(2, workbook->num_sheets);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet1") == NULL);

    lxw_workbook_free(workbook);
}

/* Test removing the only worksheet leaves the workbook empty. */
CTEST(workbook, remove_worksheet_only_sheet) {
    lxw_workbook *workbook = workbook_new(NULL);

    workbook_add_worksheet(workbook, "Sheet1");

    lxw_error got = workbook_remove_worksheet(workbook, "Sheet1");

    ASSERT_EQUAL(LXW_NO_ERROR, got);
    ASSERT_EQUAL(0, workbook->num_worksheets);
    ASSERT_EQUAL(0, workbook->num_sheets);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet1") == NULL);

    lxw_workbook_free(workbook);
}

/* Test removing a worksheet that doesn't exist. */
CTEST(workbook, remove_worksheet_not_found) {
    lxw_workbook *workbook = workbook_new(NULL);

    workbook_add_worksheet(workbook, "Sheet1");
    lxw_error got = workbook_remove_worksheet(workbook, "Sheet9");

    ASSERT_EQUAL(LXW_ERROR_SHEETNAME_NOT_FOUND, got);
    ASSERT_EQUAL(1, workbook->num_worksheets);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet1") != NULL);

    lxw_workbook_free(workbook);
}

/* Test removing a worksheet with a NULL name. */
CTEST(workbook, remove_worksheet_null) {
    lxw_workbook *workbook = workbook_new(NULL);

    workbook_add_worksheet(workbook, "Sheet1");
    lxw_error got = workbook_remove_worksheet(workbook, NULL);

    ASSERT_EQUAL(LXW_ERROR_NULL_PARAMETER_IGNORED, got);
    ASSERT_EQUAL(1, workbook->num_worksheets);

    lxw_workbook_free(workbook);
}

/* Test removing a worksheet that is before the active sheet shifts the
 * active sheet position so it keeps pointing at the same sheet. */
CTEST(workbook, remove_worksheet_before_active) {
    lxw_workbook *workbook = workbook_new(NULL);

    workbook_add_worksheet(workbook, "Sheet1");
    workbook_add_worksheet(workbook, "Sheet2");
    lxw_worksheet *ws3 = workbook_add_worksheet(workbook, "Sheet3");

    worksheet_activate(ws3);
    ASSERT_EQUAL(2, workbook->active_sheet);

    lxw_error got = workbook_remove_worksheet(workbook, "Sheet1");

    ASSERT_EQUAL(LXW_NO_ERROR, got);
    ASSERT_EQUAL(1, workbook->active_sheet);
    ASSERT_EQUAL(1, ws3->index);

    lxw_workbook_free(workbook);
}

/* Test removing the active worksheet resets the active sheet position. */
CTEST(workbook, remove_worksheet_active) {
    lxw_workbook *workbook = workbook_new(NULL);

    workbook_add_worksheet(workbook, "Sheet1");
    lxw_worksheet *ws2 = workbook_add_worksheet(workbook, "Sheet2");
    workbook_add_worksheet(workbook, "Sheet3");

    worksheet_activate(ws2);
    ASSERT_EQUAL(1, workbook->active_sheet);

    lxw_error got = workbook_remove_worksheet(workbook, "Sheet2");

    ASSERT_EQUAL(LXW_NO_ERROR, got);
    ASSERT_EQUAL(0, workbook->active_sheet);

    lxw_workbook_free(workbook);
}

