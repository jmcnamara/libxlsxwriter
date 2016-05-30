/*
 * Tests for the libxlsxwriter library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/utility.h"

// Test lxw_strerror() to ensure the error_string array doesn't go out of sync.
CTEST(utility, lxw_strerror) {

    ASSERT_STR("No error.",
               lxw_strerror(LXW_NO_ERROR));

    ASSERT_STR("Error encountered when creating a tmpfile during file assembly.",
               lxw_strerror(LXW_ERROR_CREATING_TMPFILE));

    ASSERT_STR("Maximum number of worksheet URLs (65530) exceeded.",
               lxw_strerror(LXW_ERROR_WORKSHEET_MAX_NUMBER_URLS_EXCEEDED));

    ASSERT_STR("Unknown error number.",
               lxw_strerror(LXW_MAX_ERRNO));

    ASSERT_STR("Unknown error number.",
               lxw_strerror(LXW_MAX_ERRNO + 1));
}
