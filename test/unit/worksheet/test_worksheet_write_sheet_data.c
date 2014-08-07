/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/worksheet.h"

// Test the _write_sheet_data() function.
CTEST(worksheet, write_sheet_data) {

    char* got;
    char exp[] = "<sheetData/>";
    FILE* testfile = tmpfile();

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    _worksheet_write_sheet_data(worksheet);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}

