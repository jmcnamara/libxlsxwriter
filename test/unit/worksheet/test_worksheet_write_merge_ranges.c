/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2015, Brad Ediger, brad.ediger@madriska.com
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/worksheet.h"

// Test the _write_merge_ranges() function.
CTEST(worksheet, write_merge_ranges) {

    lxw_merge_range *range;
    char* got;
    char exp[] = "<mergeCells count=\"2\"><mergeCell ref=\"B2:D8\"/><mergeCell ref=\"E3:F9\"/></mergeCells>";
    FILE* testfile = tmpfile();

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    worksheet_merge_range(worksheet, 1, 1, 7, 3);
    worksheet_merge_range(worksheet, 2, 4, 8, 5);
    _worksheet_write_merge_ranges(worksheet);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}
