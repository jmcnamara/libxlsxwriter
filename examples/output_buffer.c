/*
 * Example of using libxlsxwriter to write a workbook file to a memory buffer.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include <stdio.h>

#include "xlsxwriter.h"

int main() {
    const char *output_buffer;
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

    /* Do something with the XLSX data in the output buffer. */
    FILE *file = fopen("output_buffer.xlsx", "wb");
    fwrite(output_buffer, output_buffer_size, 1, file);
    fclose(file);
    free((void *)output_buffer);

    return ferror(stdout);
}
