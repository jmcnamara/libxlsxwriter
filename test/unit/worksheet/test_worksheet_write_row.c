/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/worksheet.h"

// Test the _write_row() function.
CTEST(worksheet, write_row) {

    char* got;
    char exp[] = "<row r=\"1\"/>";
    FILE* testfile = tmpfile();

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    lxw_row *row = _get_row_list(worksheet->table, 0);

    _write_row(worksheet, row, NULL);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}
