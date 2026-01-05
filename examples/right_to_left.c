/*
 * Example of how to use libxlsxwriter to change the default worksheet and
 * cell text direction from left-to-right to right-to-left as required by
 * some middle eastern versions of Excel.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook   = workbook_new("right_to_left.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);

    /* Create formats for text direction. */
    lxw_format *format_left_to_right = workbook_add_format(workbook);
    lxw_format *format_right_to_left = workbook_add_format(workbook);

    /* Set reading order: 1 = left-to-right, 2 = right-to-left. */
    format_set_reading_order(format_left_to_right, 1);
    format_set_reading_order(format_right_to_left, 2);

    /* Make the columns wider for clarity. */
    worksheet_set_column(worksheet1, 0, 0, 25, NULL);
    worksheet_set_column(worksheet2, 0, 0, 25, NULL);

    /* Change the direction for worksheet2. */
    worksheet_right_to_left(worksheet2);

    /* Write some data to show the difference. */
    /* Standard direction:         | A1 | B1 | C1 | ... */
    worksheet_write_string(worksheet1, 0, 0,
        "\xd9\x86\xd8\xb5 \xd8\xb9\xd8\xb1\xd8\xa8\xd9\x8a / English text",
        NULL);  /* Arabic text / English text - Default direction */
    worksheet_write_string(worksheet1, 1, 0,
        "\xd9\x86\xd8\xb5 \xd8\xb9\xd8\xb1\xd8\xa8\xd9\x8a / English text",
        format_left_to_right);
    worksheet_write_string(worksheet1, 2, 0,
        "\xd9\x86\xd8\xb5 \xd8\xb9\xd8\xb1\xd8\xa8\xd9\x8a / English text",
        format_right_to_left);

    /* Right to left direction:    ... | C1 | B1 | A1 | */
    worksheet_write_string(worksheet2, 0, 0,
        "\xd9\x86\xd8\xb5 \xd8\xb9\xd8\xb1\xd8\xa8\xd9\x8a / English text",
        NULL);  /* Default direction */
    worksheet_write_string(worksheet2, 1, 0,
        "\xd9\x86\xd8\xb5 \xd8\xb9\xd8\xb1\xd8\xa8\xd9\x8a / English text",
        format_left_to_right);
    worksheet_write_string(worksheet2, 2, 0,
        "\xd9\x86\xd8\xb5 \xd8\xb9\xd8\xb1\xd8\xa8\xd9\x8a / English text",
        format_right_to_left);

    return workbook_close(workbook);
}
