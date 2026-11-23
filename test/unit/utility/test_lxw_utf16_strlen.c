/*
 * Tests for the libxlsxwriter library.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "../../../include/xlsxwriter/utility.h"

// Test lxw_utf16_strlen(). Characters above U+FFFF count as 2 UTF-16 units.
CTEST(utility, lxw_utf16_strlen) {

    ASSERT_EQUAL(0,  (long)lxw_utf16_strlen(""));
    ASSERT_EQUAL(3,  (long)lxw_utf16_strlen("Foo"));
    ASSERT_EQUAL(4,  (long)lxw_utf16_strlen("café"));
    ASSERT_EQUAL(4,  (long)lxw_utf16_strlen("cake"));
    ASSERT_EQUAL(21, (long)lxw_utf16_strlen("Это фраза на русском!"));
    ASSERT_EQUAL(3,  (long)lxw_utf16_strlen("€€€"));
    ASSERT_EQUAL(2,  (long)lxw_utf16_strlen("\xF0\x9F\x98\x80"));
    ASSERT_EQUAL(6,  (long)lxw_utf16_strlen("a\xF0\x9F\x98\x80" "b\xF0\x9F\x98\x80"));
    ASSERT_EQUAL(2,  (long)lxw_utf16_strlen("\xF4\x8F\xBF\xBF"));
}
