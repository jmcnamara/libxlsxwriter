/*
 * Tests for the libxlsxwriter library.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "../../../include/xlsxwriter/workbook.h"

/* Create a string of emojis, which are 4 bytes in UTF-8 and 2 units in
 * UTF-16, with an optional trailing ASCII character. */
static char *
_emoji_string(size_t count, uint8_t add_ascii)
{
    size_t i;
    char *string = calloc(1, count * 4 + 2);

    for (i = 0; i < count; i++)
        memcpy(string + i * 4, "\xF0\x9F\x98\x80", 4);

    if (add_ascii)
        string[count * 4] = 'a';

    return string;
}

/* Test the worksheet string limit, which Excel measures in UTF-16 units. */
CTEST(worksheet, string_limits01) {

    lxw_error err;
    char *string;
    lxw_workbook *workbook = workbook_new(NULL);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* 16,384 emojis = 32,768 UTF-16 units. */
    string = _emoji_string(16384, LXW_FALSE);
    err = worksheet_write_string(worksheet, 0, 0, string, NULL);
    ASSERT_EQUAL(LXW_ERROR_MAX_STRING_LENGTH_EXCEEDED, err);
    free(string);

    /* 16,383 emojis + 1 ASCII = 32,767 UTF-16 units. */
    string = _emoji_string(16383, LXW_TRUE);
    err = worksheet_write_string(worksheet, 0, 0, string, NULL);
    ASSERT_EQUAL(LXW_NO_ERROR, err);
    free(string);

    lxw_workbook_free(workbook);
}
