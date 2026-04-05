/*
 * Tests for the lib_xlsx_writer library.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "../../../include/xlsxwriter/worksheet.h"

/*
 * Test that a validation list with 256 single-char items is rejected.
 *
 * The combined CSV string (items + commas) is 511 characters, which exceeds
 * Excel's limit of 255. Before the fix, _validation_list_length used uint8_t
 * for its loop counter and exited early once the running total reached 255,
 * returning 255 for lists well beyond that limit. The caller accepted 255 as
 * within bounds and called _validation_list_to_csv, which also used uint8_t.
 * After processing 255 items the counter wrapped to 0, making the loop
 * infinite and overflowing the 1023-byte heap buffer.
 */
CTEST(worksheet, validation_list_256_items_rejected) {
    const char *list[257];
    char items[256][2];
    int i;

    for (i = 0; i < 256; i++) {
        items[i][0] = 'a' + (i % 26);
        items[i][1] = '\0';
        list[i] = items[i];
    }
    list[256] = NULL;

    lxw_data_validation *data_validation = calloc(1, sizeof(lxw_data_validation));
    data_validation->validate = LXW_VALIDATION_TYPE_LIST;
    data_validation->value_list = (const char **) list;

    lxw_worksheet *worksheet = lxw_worksheet_new(NULL);
    FILE *testfile = lxw_tmpfile(NULL);
    worksheet->file = testfile;

    int err = worksheet_data_validation_cell(worksheet, 0, 0, data_validation);
    ASSERT_EQUAL(LXW_ERROR_255_STRING_LENGTH_EXCEEDED, err);

    free(data_validation);
    lxw_worksheet_free(worksheet);
    fclose(testfile);
}
