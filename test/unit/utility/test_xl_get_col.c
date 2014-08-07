/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/utility.h"

// Test _xl_get_col().
CTEST(utility, _xl_get_col) {

    ASSERT_EQUAL(0,     lxw_get_col("A1"));
    ASSERT_EQUAL(1,     lxw_get_col("B1"));
    ASSERT_EQUAL(2,     lxw_get_col("C1"));
    ASSERT_EQUAL(9,     lxw_get_col("J1"));
    ASSERT_EQUAL(24,    lxw_get_col("Y1"));
    ASSERT_EQUAL(25,    lxw_get_col("Z1"));
    ASSERT_EQUAL(26,    lxw_get_col("AA1"));
    ASSERT_EQUAL(254,   lxw_get_col("IU1"));
    ASSERT_EQUAL(255,   lxw_get_col("IV1"));
    ASSERT_EQUAL(256,   lxw_get_col("IW1"));
    ASSERT_EQUAL(16383, lxw_get_col("XFD1"));
    ASSERT_EQUAL(16384, lxw_get_col("XFE1"));
}


// Test _xl_get_col_2().
CTEST(utility, _xl_get_col_2) {

    ASSERT_EQUAL(0,     lxw_get_col_2("AAA:A"));
    ASSERT_EQUAL(1,     lxw_get_col_2("AAA:B"));
    ASSERT_EQUAL(2,     lxw_get_col_2("AAA:C"));
    ASSERT_EQUAL(9,     lxw_get_col_2("AAA:J"));
    ASSERT_EQUAL(24,    lxw_get_col_2("AAA:Y"));
    ASSERT_EQUAL(25,    lxw_get_col_2("AAA:Z"));
    ASSERT_EQUAL(26,    lxw_get_col_2("AAA:AA"));
    ASSERT_EQUAL(254,   lxw_get_col_2("AAA:IU"));
    ASSERT_EQUAL(255,   lxw_get_col_2("AAA:IV"));
    ASSERT_EQUAL(256,   lxw_get_col_2("AAA:IW"));
    ASSERT_EQUAL(16383, lxw_get_col_2("AAA:XFD"));
    ASSERT_EQUAL(16384, lxw_get_col_2("AAA:XFE"));
}
