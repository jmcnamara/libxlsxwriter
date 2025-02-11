/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Simple test case to test data writing.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {
    const char *output_buffer;
    size_t output_buffer_size;
    lxw_workbook_options options = {LXW_FALSE,
                                    ".",
                                    LXW_FALSE,
                                    &output_buffer,
                                    &output_buffer_size};

    lxw_workbook  *workbook  = workbook_new_opt(NULL, &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
    worksheet_write_number(worksheet, 1, 0, 123,     NULL);

    int error = workbook_close(workbook);
    if (error)
        return error;

    FILE *file = fopen("test_output_buffer01.xlsx", "wb");
    fwrite(output_buffer, output_buffer_size, 1, file);
    fclose(file);
    free((void *)output_buffer);

    return 0;
}
