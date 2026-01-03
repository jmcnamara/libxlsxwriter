/*
 * A demonstration of the available colors and how to use them in
 * libxlsxwriter.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("colors.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Set the column width for clarity. */
    worksheet_set_column(worksheet, 0, 0, 16, NULL);
    worksheet_set_column(worksheet, 1, 1, 10, NULL);

    /* Define some named colors with their RGB values. */
    struct {
        const char *name;
        lxw_color_t color;
    } named_colors[] = {
        {"Black",   LXW_COLOR_BLACK},
        {"Blue",    LXW_COLOR_BLUE},
        {"Brown",   LXW_COLOR_BROWN},
        {"Cyan",    LXW_COLOR_CYAN},
        {"Gray",    LXW_COLOR_GRAY},
        {"Green",   LXW_COLOR_GREEN},
        {"Lime",    LXW_COLOR_LIME},
        {"Magenta", LXW_COLOR_MAGENTA},
        {"Navy",    LXW_COLOR_NAVY},
        {"Orange",  LXW_COLOR_ORANGE},
        {"Pink",    LXW_COLOR_PINK},
        {"Purple",  LXW_COLOR_PURPLE},
        {"Red",     LXW_COLOR_RED},
        {"Silver",  LXW_COLOR_SILVER},
        {"White",   LXW_COLOR_WHITE},
        {"Yellow",  LXW_COLOR_YELLOW},
    };

    int num_colors = sizeof(named_colors) / sizeof(named_colors[0]);

    /* Write the named colors. */
    for (int row = 0; row < num_colors; row++) {
        lxw_format *color_format = workbook_add_format(workbook);
        format_set_bg_color(color_format, named_colors[row].color);

        worksheet_write_string(worksheet, row, 0, named_colors[row].name, NULL);
        worksheet_write_blank(worksheet, row, 1, color_format);
    }

    /* Write some user-defined RGB colors. */
    struct {
        const char *name;
        lxw_color_t color;
    } user_colors[] = {
        {"#FF7F50", 0xFF7F50},  /* Coral */
        {"#DCDCDC", 0xDCDCDC},  /* Gainsboro */
        {"#6495ED", 0x6495ED},  /* CornflowerBlue */
        {"#DAA520", 0xDAA520},  /* GoldenRod */
    };

    int num_user_colors = sizeof(user_colors) / sizeof(user_colors[0]);

    for (int i = 0; i < num_user_colors; i++) {
        int row = num_colors + i;
        lxw_format *color_format = workbook_add_format(workbook);
        format_set_bg_color(color_format, user_colors[i].color);

        worksheet_write_string(worksheet, row, 0, user_colors[i].name, NULL);
        worksheet_write_blank(worksheet, row, 1, color_format);
    }

    return workbook_close(workbook);
}
