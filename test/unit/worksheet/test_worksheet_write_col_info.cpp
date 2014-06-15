/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include <gtest/gtest.h>
#include "../helper.h"

#include "xlsxwriter/worksheet.h"
#include "xlsxwriter/format.h"

// Test the _write_col_info() function.
TEST(worksheet, write_col_info01) {

    char* got;
    char exp[] = "<col min=\"2\" max=\"4\" width=\"5.7109375\" customWidth=\"1\"/>";
    FILE* testfile = tmpfile();
    lxw_col_options col_options = {.firstcol  = 1,
                                   .lastcol   = 3,
                                   .width     = 5,
                                   .format    = NULL,
                                   .hidden    = 0,
                                   .level     = 0,
                                   .collapsed = 0};

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    _write_col_info(worksheet, &col_options);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}


TEST(worksheet, write_col_info02) {

    char* got;
    char exp[] = "<col min=\"6\" max=\"6\" width=\"8.7109375\" hidden=\"1\" customWidth=\"1\"/>";
    FILE* testfile = tmpfile();
    lxw_col_options col_options = {.firstcol  = 5,
                                   .lastcol   = 5,
                                   .width     = 8,
                                   .format    = NULL,
                                   .hidden    = 1,
                                   .level     = 0,
                                   .collapsed = 0};

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    _write_col_info(worksheet, &col_options);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}


TEST(worksheet, write_col_info03) {

    char* got;
    char exp[] = "<col min=\"8\" max=\"8\" width=\"9.140625\" style=\"1\"/>";
    FILE* testfile = tmpfile();

    lxw_format *format = _new_format();
    format->xf_index = 1;

    lxw_col_options col_options = {.firstcol  = 7,
                                   .lastcol   = 7,
                                   .width     = LXW_DEF_COL_WIDTH,
                                   .format    = format,
                                   .hidden    = 0,
                                   .level     = 0,
                                   .collapsed = 0};

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    _write_col_info(worksheet, &col_options);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}


TEST(worksheet, write_col_info04) {

    char* got;
    char exp[] = "<col min=\"9\" max=\"9\" width=\"9.140625\" style=\"1\"/>";
    FILE* testfile = tmpfile();

    lxw_format *format = _new_format();
    format->xf_index = 1;

    lxw_col_options col_options = {.firstcol  = 8,
                                   .lastcol   = 8,
                                   .width     = LXW_DEF_COL_WIDTH,
                                   .format    = format,
                                   .hidden    = 0,
                                   .level     = 0,
                                   .collapsed = 0};

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    _write_col_info(worksheet, &col_options);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}


TEST(worksheet, write_col_info05) {

    char* got;
    char exp[] = "<col min=\"10\" max=\"10\" width=\"2.7109375\" customWidth=\"1\"/>";
    FILE* testfile = tmpfile();
    lxw_col_options col_options = {.firstcol  = 9,
                                   .lastcol   = 9,
                                   .width     = 2,
                                   .format    = NULL,
                                   .hidden    = 0,
                                   .level     = 0,
                                   .collapsed = 0};

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    _write_col_info(worksheet, &col_options);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}


TEST(worksheet, write_col_info06) {

    char* got;
    char exp[] = "<col min=\"12\" max=\"12\" width=\"0\" hidden=\"1\" customWidth=\"1\"/>";
    FILE* testfile = tmpfile();
    lxw_col_options col_options = {.firstcol  = 11,
                                   .lastcol   = 11,
                                   .width     = LXW_DEF_COL_WIDTH,
                                   .format    = NULL,
                                   .hidden    = 1,
                                   .level     = 0,
                                   .collapsed = 0};

    lxw_worksheet *worksheet = _new_worksheet(NULL);
    worksheet->file = testfile;

    _write_col_info(worksheet, &col_options);

    RUN_XLSX_STREQ(exp, got);

    _free_worksheet(worksheet);
}
