/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/utility.h"

// Test lxw_utf8_strlen().
CTEST(utility, lxw_utf8_strlen) {

    ASSERT_EQUAL(0,  lxw_utf8_strlen(""));
    ASSERT_EQUAL(3,  lxw_utf8_strlen("Foo"));
    ASSERT_EQUAL(4,  lxw_utf8_strlen("café"));
    ASSERT_EQUAL(4,  lxw_utf8_strlen("cake"));
    ASSERT_EQUAL(21, lxw_utf8_strlen("Это фраза на русском!"));

}

