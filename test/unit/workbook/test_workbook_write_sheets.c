/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/workbook.h"

// Test the _write_sheets() function.
CTEST(workbook, write_sheets) {


    char* got;
    char exp[] = "<sheets><sheet name=\"Sheet1\" sheetId=\"1\" r:id=\"rId1\"/></sheets>";
    FILE* testfile = tmpfile();

    lxw_workbook *workbook = new_workbook(NULL);
    workbook->file = testfile;

    workbook_add_worksheet(workbook, NULL);

    _write_sheets(workbook);

    RUN_XLSX_STREQ(exp, got);

    _free_workbook(workbook);
}

