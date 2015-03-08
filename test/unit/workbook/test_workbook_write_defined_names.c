/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/workbook.h"

/* Test the _write_defined_names() method. */
CTEST(workbook, write_defined_names) {


    char* got;
    char exp[] = "<definedNames><definedName name=\"_xlnm.Print_Titles\" localSheetId=\"0\">Sheet1!$1:$1</definedName></definedNames>";
    FILE* testfile = tmpfile();
    lxw_defined_name *defined_name = calloc(1, sizeof(struct lxw_defined_name));

    strcpy(defined_name->name, "_xlnm.Print_Titles");
    strcpy(defined_name->range, "Sheet1!$1:$1");

    lxw_workbook *workbook = new_workbook(NULL);
    workbook->file = testfile;

    LIST_INSERT_HEAD(workbook->defined_names, defined_name, list_pointers);

    _write_defined_names(workbook);

    RUN_XLSX_STREQ(exp, got);

    _free_workbook(workbook);
}

