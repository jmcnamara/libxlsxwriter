/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/utility.h"

// Test _xl_get_row().
CTEST(utility, _xl_get_row) {

    ASSERT_EQUAL(0,       lxw_get_row("A1"));
    ASSERT_EQUAL(1,       lxw_get_row("B2"));
    ASSERT_EQUAL(2,       lxw_get_row("C3"));
    ASSERT_EQUAL(9,       lxw_get_row("J10"));
    ASSERT_EQUAL(24,      lxw_get_row("Y25"));
    ASSERT_EQUAL(25,      lxw_get_row("Z26"));
    ASSERT_EQUAL(26,      lxw_get_row("AA27"));
    ASSERT_EQUAL(254,     lxw_get_row("IU255"));
    ASSERT_EQUAL(255,     lxw_get_row("IV256"));
    ASSERT_EQUAL(256,     lxw_get_row("IW257"));
    ASSERT_EQUAL(16383,   lxw_get_row("XFD16384"));
    ASSERT_EQUAL(16384,   lxw_get_row("XFE16385"));
    ASSERT_EQUAL(1048576, lxw_get_row("XFE1048577"));
}

