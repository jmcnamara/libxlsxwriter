/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/workbook.h"

// Test the _write_calc_pr() function.
CTEST(workbook, write_calc_pr) {


    char* got;
    char exp[] = "<calcPr calcId=\"124519\" fullCalcOnLoad=\"1\"/>";
    FILE* testfile = tmpfile();

    lxw_workbook *workbook = new_workbook(NULL);
    workbook->file = testfile;

    _write_calc_pr(workbook);

    RUN_XLSX_STREQ(exp, got);

    _free_workbook(workbook);
}

