/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include <gtest/gtest.h>
#include "../helper.h"

#include "xlsxwriter/utility.h"

// Test _xl_get_col().
TEST(utility, _xl_get_col) {

    EXPECT_EQ(0,     lxw_get_col("A1"));
    EXPECT_EQ(1,     lxw_get_col("B1"));
    EXPECT_EQ(2,     lxw_get_col("C1"));
    EXPECT_EQ(9,     lxw_get_col("J1"));
    EXPECT_EQ(24,    lxw_get_col("Y1"));
    EXPECT_EQ(25,    lxw_get_col("Z1"));
    EXPECT_EQ(26,    lxw_get_col("AA1"));
    EXPECT_EQ(254,   lxw_get_col("IU1"));
    EXPECT_EQ(255,   lxw_get_col("IV1"));
    EXPECT_EQ(256,   lxw_get_col("IW1"));
    EXPECT_EQ(16383, lxw_get_col("XFD1"));
    EXPECT_EQ(16384, lxw_get_col("XFE1"));
}


// Test _xl_get_col_2().
TEST(utility, _xl_get_col_2) {

    EXPECT_EQ(0,     lxw_get_col_2("AAA:A"));
    EXPECT_EQ(1,     lxw_get_col_2("AAA:B"));
    EXPECT_EQ(2,     lxw_get_col_2("AAA:C"));
    EXPECT_EQ(9,     lxw_get_col_2("AAA:J"));
    EXPECT_EQ(24,    lxw_get_col_2("AAA:Y"));
    EXPECT_EQ(25,    lxw_get_col_2("AAA:Z"));
    EXPECT_EQ(26,    lxw_get_col_2("AAA:AA"));
    EXPECT_EQ(254,   lxw_get_col_2("AAA:IU"));
    EXPECT_EQ(255,   lxw_get_col_2("AAA:IV"));
    EXPECT_EQ(256,   lxw_get_col_2("AAA:IW"));
    EXPECT_EQ(16383, lxw_get_col_2("AAA:XFD"));
    EXPECT_EQ(16384, lxw_get_col_2("AAA:XFE"));
}
