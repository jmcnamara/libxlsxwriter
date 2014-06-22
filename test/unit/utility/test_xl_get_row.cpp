/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include <gtest/gtest.h>
#include "../helper.h"

#include "xlsxwriter/utility.h"

// Test _xl_get_row().
TEST(utility, _xl_get_row) {

    EXPECT_EQ(0,       xl_get_row("A1"));
    EXPECT_EQ(1,       xl_get_row("B2"));
    EXPECT_EQ(2,       xl_get_row("C3"));
    EXPECT_EQ(9,       xl_get_row("J10"));
    EXPECT_EQ(24,      xl_get_row("Y25"));
    EXPECT_EQ(25,      xl_get_row("Z26"));
    EXPECT_EQ(26,      xl_get_row("AA27"));
    EXPECT_EQ(254,     xl_get_row("IU255"));
    EXPECT_EQ(255,     xl_get_row("IV256"));
    EXPECT_EQ(256,     xl_get_row("IW257"));
    EXPECT_EQ(16383,   xl_get_row("XFD16384"));
    EXPECT_EQ(16384,   xl_get_row("XFE16385"));
    EXPECT_EQ(1048576, xl_get_row("XFE1048577"));
}

