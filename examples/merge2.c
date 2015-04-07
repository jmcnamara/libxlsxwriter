/*
 * An example of merging cells using libxlsxwriter.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook     = new_workbook("merge2.xlsx");
    lxw_worksheet *worksheet    = workbook_add_worksheet(workbook, NULL);
    lxw_format    *merge_format = workbook_add_format(workbook);

    /* Configure a format for the merged range. */
    format_set_align(merge_format, LXW_ALIGN_CENTER);
    format_set_align(merge_format, LXW_ALIGN_VERTICAL_CENTER);
    format_set_bold(merge_format);
    format_set_bg_color(merge_format, 0xD7E4BC);
    format_set_border(merge_format, LXW_BORDER_DOUBLE);

    /* Increase the cell size of the merged cells to highlight the formatting. */
    worksheet_set_column(worksheet, 1, 3, 12, NULL, NULL);
    worksheet_set_row(worksheet, 2, 30, NULL, NULL);
    worksheet_set_row(worksheet, 3, 30, NULL, NULL);

    /* Merge 3 cells over two rows. */
    worksheet_merge_range(worksheet, 2, 1, 3, 3, "Merged Range", merge_format);

    workbook_close(workbook);

    return 0;
}
