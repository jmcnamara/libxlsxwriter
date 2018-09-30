/*
 * An example of merging cells containing a rich string using libxlsxwriter.
 *
 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("merge_rich_string.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Configure a format for the merged range. */
    lxw_format *merge_format = workbook_add_format(workbook);
    format_set_align(merge_format, LXW_ALIGN_CENTER);
    format_set_align(merge_format, LXW_ALIGN_VERTICAL_CENTER);
    format_set_border(merge_format, LXW_BORDER_THIN);

    /* Configure formats for the rich string. */
    lxw_format *red = workbook_add_format(workbook);
    format_set_font_color(red, LXW_COLOR_RED);

    lxw_format *blue = workbook_add_format(workbook);
    format_set_font_color(blue, LXW_COLOR_BLUE);

    /* Create the fragments for the rich string. */
    lxw_rich_string_tuple fragment1 = {.format = NULL, .string = "This is "     };
    lxw_rich_string_tuple fragment2 = {.format = red,  .string = "red"          };
    lxw_rich_string_tuple fragment3 = {.format = NULL, .string = " and this is "};
    lxw_rich_string_tuple fragment4 = {.format = blue, .string = "blue"         };

    lxw_rich_string_tuple *rich_string[] = {&fragment1, &fragment2,
                                            &fragment3, &fragment4, NULL};


    /* Write an empty string to the merged range. */
    worksheet_merge_range(worksheet, 1, 1, 4, 3, "", merge_format);

    /* We then overwrite the first merged cell with a rich string. Note that
     * we must also pass the cell format used in the merged cells format at
     * the end. */
    worksheet_write_rich_string(worksheet, 1, 1, rich_string, merge_format);

    workbook_close(workbook);

    return 0;
}
