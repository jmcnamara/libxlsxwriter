/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/worksheet.h"

/* 1. Test the _write_sheet_views() method with selection set. */
CTEST(worksheet, set_selection01) {
    char* got;
    char exp[] = "<sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"/></sheetViews>";
    FILE* testfile = tmpfile();

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    worksheet_select(worksheet);
    worksheet_set_selection(worksheet, 0, 0, 0, 0);
    _worksheet_write_sheet_views(worksheet);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}


/* 2. Test the _write_sheet_views() method with selection set. */
CTEST(worksheet, set_selection02) {
    char* got;
    char exp[] = "<sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"><selection activeCell=\"A2\" sqref=\"A2\"/></sheetView></sheetViews>";
    FILE* testfile = tmpfile();

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    worksheet_select(worksheet);
    worksheet_set_selection(worksheet, 1, 0, 1, 0);
    _worksheet_write_sheet_views(worksheet);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}


/* 3. Test the _write_sheet_views() method with selection set. */
CTEST(worksheet, set_selection03) {
    char* got;
    char exp[] = "<sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"><selection activeCell=\"B1\" sqref=\"B1\"/></sheetView></sheetViews>";
    FILE* testfile = tmpfile();

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    worksheet_select(worksheet);
    worksheet_set_selection(worksheet, RANGE("B1:B1"));
    _worksheet_write_sheet_views(worksheet);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}


/* 4. Test the _write_sheet_views() method with selection set. */
CTEST(worksheet, set_selection04) {
    char* got;
    char exp[] = "<sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"><selection activeCell=\"D3\" sqref=\"D3\"/></sheetView></sheetViews>";
    FILE* testfile = tmpfile();

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    worksheet_select(worksheet);
    worksheet_set_selection(worksheet, RANGE("D3:D3"));
    _worksheet_write_sheet_views(worksheet);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}


/* 5. Test the _write_sheet_views() method with selection set. */
CTEST(worksheet, set_selection05) {
    char* got;
    char exp[] = "<sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"><selection activeCell=\"D3\" sqref=\"D3:F4\"/></sheetView></sheetViews>";
    FILE* testfile = tmpfile();

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    worksheet_select(worksheet);
    worksheet_set_selection(worksheet, RANGE("D3:F4"));
    _worksheet_write_sheet_views(worksheet);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}


/* 6. Test the _write_sheet_views() method with selection set. */
CTEST(worksheet, set_selection06) {
    char* got;
    char exp[] = "<sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"><selection activeCell=\"F4\" sqref=\"D3:F4\"/></sheetView></sheetViews>";
    FILE* testfile = tmpfile();

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    worksheet_select(worksheet);
    worksheet_set_selection(worksheet, RANGE("F4:D3"));
    _worksheet_write_sheet_views(worksheet);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}


/* 7. Test the _write_sheet_views() method with selection set. */
CTEST(worksheet, set_selection07) {
    char* got;
    char exp[] = "<sheetViews><sheetView tabSelected=\"1\" workbookViewId=\"0\"><selection activeCell=\"A2\" sqref=\"A2\"/></sheetView></sheetViews>";
    FILE* testfile = tmpfile();

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    worksheet_select(worksheet);
    worksheet_set_selection(worksheet, RANGE("A2:A2"));
    _worksheet_write_sheet_views(worksheet);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}
