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


/* Test renaming a worksheet by name. */
CTEST(workbook, rename_worksheet_basic) {
    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "Sheet1");
    lxw_error got = workbook_rename_worksheet(workbook, "Sheet1", "Sheet2");

    ASSERT_EQUAL(LXW_NO_ERROR, got);
    ASSERT_STR("Sheet2", worksheet->name);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet2") == worksheet);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet1") == NULL);

    lxw_workbook_free(workbook);
}

/* Test renaming a worksheet that doesn't exist. */
CTEST(workbook, rename_worksheet_not_found) {
    lxw_workbook *workbook = workbook_new(NULL);

    workbook_add_worksheet(workbook, "Sheet1");
    lxw_error got = workbook_rename_worksheet(workbook, "Sheet9", "Sheet2");

    ASSERT_EQUAL(LXW_ERROR_SHEETNAME_NOT_FOUND, got);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet1") != NULL);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet9") == NULL);

    lxw_workbook_free(workbook);
}

/* Test renaming a worksheet to a name that is already in use. */
CTEST(workbook, rename_worksheet_already_used) {
    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "Sheet1");
    workbook_add_worksheet(workbook, "Sheet2");
    lxw_error got = workbook_rename_worksheet(workbook, "Sheet1", "Sheet2");

    ASSERT_EQUAL(LXW_ERROR_SHEETNAME_ALREADY_USED, got);
    /* The original worksheet is left unchanged on error. */
    ASSERT_STR("Sheet1", worksheet->name);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet1") == worksheet);

    lxw_workbook_free(workbook);
}

/* Test renaming a worksheet to a name with an invalid character. */
CTEST(workbook, rename_worksheet_invalid_character) {
    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "Sheet1");
    lxw_error got = workbook_rename_worksheet(workbook, "Sheet1", "Sheet[1]");

    ASSERT_EQUAL(LXW_ERROR_INVALID_SHEETNAME_CHARACTER, got);
    ASSERT_STR("Sheet1", worksheet->name);

    lxw_workbook_free(workbook);
}

/* Test renaming a worksheet to an empty name. */
CTEST(workbook, rename_worksheet_empty) {
    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "Sheet1");
    lxw_error got = workbook_rename_worksheet(workbook, "Sheet1", "");

    ASSERT_EQUAL(LXW_ERROR_PARAMETER_IS_EMPTY, got);
    ASSERT_STR("Sheet1", worksheet->name);

    lxw_workbook_free(workbook);
}

/* Test renaming a worksheet with a NULL old name. */
CTEST(workbook, rename_worksheet_null_old_name) {
    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "Sheet1");
    lxw_error got = workbook_rename_worksheet(workbook, NULL, "Sheet2");

    ASSERT_EQUAL(LXW_ERROR_NULL_PARAMETER_IGNORED, got);
    ASSERT_STR("Sheet1", worksheet->name);

    lxw_workbook_free(workbook);
}

/* Test renaming a worksheet with a NULL new name. */
CTEST(workbook, rename_worksheet_null_new_name) {
    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "Sheet1");
    lxw_error got = workbook_rename_worksheet(workbook, "Sheet1", NULL);

    ASSERT_EQUAL(LXW_ERROR_NULL_PARAMETER_IGNORED, got);
    ASSERT_STR("Sheet1", worksheet->name);

    lxw_workbook_free(workbook);
}

/* Test a case-only rename which must be allowed (Sheet1 -> sheet1). */
CTEST(workbook, rename_worksheet_case_only) {
    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, "Sheet1");
    lxw_error got = workbook_rename_worksheet(workbook, "Sheet1", "sheet1");

    ASSERT_EQUAL(LXW_NO_ERROR, got);
    ASSERT_STR("sheet1", worksheet->name);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "sheet1") == worksheet);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "Sheet1") == worksheet);
    ASSERT_TRUE(workbook_get_worksheet_by_name(workbook, "SHEET1") == worksheet);

    lxw_workbook_free(workbook);
}

/* Test that renaming a worksheet doesn't change its position/index. */
CTEST(workbook, rename_worksheet_preserves_index) {
    lxw_workbook *workbook = workbook_new(NULL);

    lxw_worksheet *ws1 = workbook_add_worksheet(workbook, "Sheet1");
    workbook_add_worksheet(workbook, "Sheet2");
    lxw_worksheet *ws3 = workbook_add_worksheet(workbook, "Sheet3");

    uint16_t index = ws3->index;
    lxw_error got = workbook_rename_worksheet(workbook, "Sheet3", "Data");

    ASSERT_EQUAL(LXW_NO_ERROR, got);
    ASSERT_EQUAL(index, ws3->index);
    ASSERT_EQUAL(0, ws1->index);
    ASSERT_STR("Data", ws3->name);

    lxw_workbook_free(workbook);
}

