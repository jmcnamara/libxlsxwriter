/*
 * An example of writing cell comments to a worksheet using libxlsxwriter.
 *
 * Each of the worksheets demonstrates different features of cell comments.
 *
 * Copyright 2014-2020, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook   = workbook_new("comments2.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet4 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet5 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet6 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet7 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet8 = workbook_add_worksheet(workbook, NULL);

    lxw_format *text_wrap = workbook_add_format(workbook);
    format_set_text_wrap(text_wrap);
    format_set_align(text_wrap, LXW_ALIGN_VERTICAL_TOP);


    /*
     * Example 1. Demonstrates a simple cell comments without formatting.
     */

    /* Set up some worksheet formatting. */
    worksheet_set_column(worksheet1, 2, 2, 25, NULL);
    worksheet_set_row(worksheet1, 2, 50, NULL);


    worksheet_write_string(worksheet1, CELL("C3"),
                           "Hold the mouse over this cell to see the comment.",
                           text_wrap);

    worksheet_write_comment(worksheet1, CELL("C3"), "This is a comment.");


    /*
     * Example 2. Demonstrates visible and hidden comments.
     */

    /* Set up some worksheet formatting. */
    worksheet_set_column(worksheet2, 2, 2, 25, NULL);
    worksheet_set_row(worksheet2, 2, 50, NULL);
    worksheet_set_row(worksheet2, 2, 50, NULL);


    worksheet_write_string(worksheet2, CELL("C3"),
                           "This cell comment is visible.",
                           text_wrap);

    /* Use an option to make the comment visible. */
    lxw_comment_options options2 = {.visible = LXW_COMMENT_DISPLAY_VISIBLE};
    worksheet_write_comment_opt(worksheet2, CELL("C3"), "Hello.", &options2);


    worksheet_write_string(worksheet2, CELL("C6"),
                           "This cell comment isn't visible until you pass "
                           "the mouse over it (the default).",
                           text_wrap);

    worksheet_write_comment(worksheet2, CELL("C6"), "Hello.");


    /*
     * Example 3. Demonstrates visible and hidden comments, set at the
     * worksheet level.
     */
    worksheet_set_column(worksheet3, 2, 2, 25, NULL);
    worksheet_set_row(worksheet3, 2, 50, NULL);
    worksheet_set_row(worksheet3, 5, 50, NULL);
    worksheet_set_row(worksheet3, 8, 50, NULL);

    /* Make all comments on the worksheet visible. */
    worksheet_show_comments(worksheet3);


    worksheet_write_string(worksheet3, CELL("C3"),
                           "This cell comment is visible, explicitly.",
                           text_wrap);

    lxw_comment_options options3a = {.visible = LXW_COMMENT_DISPLAY_VISIBLE};
    worksheet_write_comment_opt(worksheet3, 2, 2, "Hello", &options3a);


    worksheet_write_string(worksheet3, CELL("C6"),
                           "This cell comment is also visible because "
                           "we used worksheet_show_comments().",
                           text_wrap);

    worksheet_write_comment(worksheet3, CELL("C6"), "Hello");


    worksheet_write_string(worksheet3, CELL("C9"),
                           "However, we can still override it locally.",
                           text_wrap);

    lxw_comment_options options3b = {.visible = LXW_COMMENT_DISPLAY_HIDDEN};
    worksheet_write_comment_opt(worksheet3, CELL("C9"), "Hello", &options3b);


    /*
     * Example 4. Demonstrates changes to the comment box dimensions.
     */
    worksheet_set_column(worksheet4, 2, 2, 25, NULL);
    worksheet_set_row(worksheet4, 2, 50, NULL);
    worksheet_set_row(worksheet4, 5, 50, NULL);
    worksheet_set_row(worksheet4, 8, 50, NULL);
    worksheet_set_row(worksheet4, 15, 50, NULL);
    worksheet_set_row(worksheet4, 18, 50, NULL);

    worksheet_show_comments(worksheet4);


    worksheet_write_string(worksheet4, CELL("C3"),
                           "This cell comment is default size.",
                           text_wrap);

    worksheet_write_comment_opt(worksheet4, 2, 2, "Hello", NULL);


    worksheet_write_string(worksheet4, CELL("C6"),
                           "This cell comment is twice as wide.",
                           text_wrap);

    lxw_comment_options options4a = {.x_scale = 2.0};
    worksheet_write_comment_opt(worksheet4, CELL("C6"), "Hello", &options4a);


    worksheet_write_string(worksheet4, CELL("C9"),
                           "This cell comment is twice as high.",
                           text_wrap);

    lxw_comment_options options4b = {.y_scale = 2.0};
    worksheet_write_comment_opt(worksheet4, CELL("C9"), "Hello", &options4b);


    worksheet_write_string(worksheet4, CELL("C16"),
                           "This cell comment is scaled in both directions.",
                           text_wrap);

    lxw_comment_options options4c = {.x_scale = 1.2, .y_scale = 0.5};
    worksheet_write_comment_opt(worksheet4, CELL("C16"), "Hello", &options4c);


    worksheet_write_string(worksheet4, CELL("C19"),
                           "This cell comment has width and height specified in pixels.",
                           text_wrap);

    lxw_comment_options options4d = {.width = 200, .height = 50};
    worksheet_write_comment_opt(worksheet4, CELL("C19"), "Hello", &options4d);


    /*
     * Example 5. Demonstrates changes to the cell comment position.
     */
    worksheet_set_column(worksheet5, 2, 2, 25, NULL);
    worksheet_set_row(worksheet5, 2, 50, NULL);
    worksheet_set_row(worksheet5, 5, 50, NULL);
    worksheet_set_row(worksheet5, 8, 50, NULL);

    worksheet_show_comments(worksheet5);

    worksheet_write_string(worksheet5, CELL("C3"),
                           "This cell comment is in the default position.",
                           text_wrap);

    worksheet_write_comment(worksheet5, 2, 2, "Hello");


    worksheet_write_string(worksheet5, CELL("C6"),
                           "This cell comment has been moved to another cell.",
                           text_wrap);

    lxw_comment_options options5a = {.start_row = 3, .start_col = 4};
    worksheet_write_comment_opt(worksheet5, CELL("C6"), "Hello", &options5a);


    worksheet_write_string(worksheet5, CELL("C9"),
                           "This cell comment has been shifted within its default cell.",
                           text_wrap);

    lxw_comment_options options5b = {.x_offset = 30, .y_offset = 12};
    worksheet_write_comment_opt(worksheet5, CELL("C9"), "Hello", &options5b);


    /*
     * Example 6. Demonstrates changes to the comment background color.
     */
    worksheet_set_column(worksheet6, 2, 2, 25, NULL);
    worksheet_set_row(worksheet6, 2, 50, NULL);
    worksheet_set_row(worksheet6, 5, 50, NULL);
    worksheet_set_row(worksheet6, 8, 50, NULL);

    worksheet_show_comments(worksheet6);

    worksheet_write_string(worksheet6, CELL("C3"),
                           "This cell comment has a different color.",
                           text_wrap);

    lxw_comment_options options6a = {.color = LXW_COLOR_GREEN};
    worksheet_write_comment_opt(worksheet6, 2, 2, "Hello", &options6a);


    worksheet_write_string(worksheet6, CELL("C6"),
                           "This cell comment has the default color.",
                           text_wrap);

    worksheet_write_comment(worksheet6, CELL("C6"), "Hello");


    worksheet_write_string(worksheet6, CELL("C9"),
                           "This cell comment has a different color.",
                           text_wrap);

    lxw_comment_options options6b = {.color = 0xFF6600};
    worksheet_write_comment_opt(worksheet6, CELL("C9"), "Hello", &options6b);


    /*
     * Example 7. Demonstrates how to set the cell comment author.
     */
    worksheet_set_column(worksheet7, 2, 2, 30, NULL);
    worksheet_set_row(worksheet7, 2, 50, NULL);
    worksheet_set_row(worksheet7, 5, 60, NULL);

    worksheet_write_string(worksheet7, CELL("C3"),
                           "Move the mouse over this cell and you will see 'Cell C3 "
                           "commented by' (blank) in the status bar at the bottom.",
                           text_wrap);

    worksheet_write_comment(worksheet7, CELL("C3"), "Hello");


    worksheet_write_string(worksheet7, CELL("C6"),
                           "Move the mouse over this cell and you will see 'Cell C6 "
                           "commented by libxlsxwriter' in the status bar at the bottom.",
                           text_wrap);

    lxw_comment_options options7a = {.author = "libxlsxwriter"};
    worksheet_write_comment_opt(worksheet7, CELL("C6"), "Hello", &options7a);


    /*
     * Example 8. Demonstrates the need to explicitly set the row height.
     */
    worksheet_set_column(worksheet8, 2, 2, 25, NULL);
    worksheet_set_row(worksheet8, 2, 80, NULL);

    worksheet_show_comments(worksheet8);

    worksheet_write_string(worksheet8, CELL("C3"),
                           "The height of this row has been adjusted explicitly using "
                           "worksheet_set_row(). The size of the comment box is "
                           "adjusted accordingly by libxlsxwriter",
                           text_wrap);

    worksheet_write_comment(worksheet8, CELL("C3"), "Hello");


    worksheet_write_string(worksheet8, CELL("C6"),
                           "The height of this row has been adjusted by Excel when the "
                           "file is opened due to the text wrap property being set. "
                           "Unfortunately this means that the height of the row is "
                           "unknown to libxlsxwriter at run time and thus the comment "
                           "box is stretched as well.\n\n"
                           "Use worksheet_set_row() to specify the row height explicitly "
                           "to avoid this problem.",
                           text_wrap);

    worksheet_write_comment(worksheet8, CELL("C6"), "Hello");

    return workbook_close(workbook);
}
