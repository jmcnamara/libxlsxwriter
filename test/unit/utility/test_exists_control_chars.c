/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2020, Jiexin Wang, viest@php.net
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/utility.h"

// Test lxw_exists_control_chars().
CTEST(utility, lxw_exists_control_chars) {
    unsigned char result;

    // ASCII + Non ASCII
    result = lxw_exists_control_chars("éABCDEéFGHIJKLMNOPQRSTUVWXYZ");
    ASSERT_FALSE(result);
    result = lxw_exists_control_chars("中ABCDEFGHI中JKLMNOPQRSTUVWXYZ");
    ASSERT_FALSE(result);

    // ASCII
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    ASSERT_FALSE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x09RSTUVWXYZ");
    ASSERT_FALSE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x0ARSTUVWXYZ");
    ASSERT_FALSE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x20RSTUVWXYZ");
    ASSERT_FALSE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ!RSTUVWXYZ");
    ASSERT_FALSE(result);

    // Control Chars
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x01RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x02RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x03RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x04RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x05RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x06RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x07RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x08RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x0BRSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x0CRSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x0DRSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x0ERSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x0FRSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x10RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x11RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x12RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x13RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x14RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x15RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x16RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x17RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x18RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x19RSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x1ARSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x1BRSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x1CRSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x1DRSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x1ERSTUVWXYZ");
    ASSERT_TRUE(result);
    result = lxw_exists_control_chars("ABCDEFGHIJKLMNOPQ\x1FRSTUVWXYZ");
    ASSERT_TRUE(result);
}
