/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/worksheet.h"


/* 1. Test the _write_sheet_protection() method. */
CTEST(worksheet, write_write_sheet_protection01) {
    char* got;
    char exp[] = "<sheetProtection sheet=\"1\" objects=\"1\" scenarios=\"1\"/>";
    FILE* testfile = tmpfile();

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    //worksheet_protect(worksheet, $password, \%options);
    worksheet->protection.is_configured = 1;
    _worksheet_write_sheet_protection(worksheet);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}
