/*
 * Example of using libxlsxwriter for writing a workbook file to a buffer.
 *
 * Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
 *
 */

#include <stdio.h>

#include "xlsxwriter.h"

int main() {
    char *output_buffer;
    size_t output_buffer_size;

    /* Set the worksheet options. */
    lxw_workbook_options options = {.output_buffer = &output_buffer,
                                    .output_buffer_size = &output_buffer_size,
                                    .constant_memory = LXW_FALSE,
                                    .tmpdir = NULL,
                                    .use_zip64 = LXW_FALSE};

    /* Create a new workbook with options. */
    lxw_workbook  *workbook  = workbook_new_opt(NULL, &options);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
    worksheet_write_number(worksheet, 1, 0, 123,     NULL);

    lxw_error error = workbook_close(workbook);

    if (error)
        return error;

    /* Write the XLSX file to standard output. */
    fwrite(output_buffer, output_buffer_size, 1, stdout);

    return ferror(stdout);
}
