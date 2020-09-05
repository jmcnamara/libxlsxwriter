/*
 * An example of how to add conditional formatting to an libxlsxwriter file.
 *
 * Conditional formatting allows you to apply a format to a cell or a
 * range of cells based on certain criteria.
 *
 * Copyright 2014-2020, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

/* Write some data to the worksheet. */
void write_worksheet_data(lxw_worksheet *worksheet) {

    uint8_t data[10][10] = {
        {34, 72,  38, 30, 75, 48, 75, 66, 84, 86},
        {6,  24,  1,  84, 54, 62, 60, 3, 26,  59},
        {28, 79,  97, 13, 85, 93, 93, 22, 5,  14},
        {27, 71,  40, 17, 18, 79, 90, 93, 29, 47},
        {88, 25,  33, 23, 67, 1,  59, 79, 47, 36},
        {24, 100, 20, 88, 29, 33, 38, 54, 54, 88},
        {6,  57,  88, 28, 10, 26, 37, 7,  41, 48},
        {52, 78,  1,  96, 26, 45, 47, 33, 96, 36},
        {60, 54,  81, 66, 81, 90, 80, 93, 12, 55},
        {70, 5,   46, 14, 71, 19, 66, 36, 41, 21},
    };

    int row, col;
    for (row = 0; row < 10; row++)
        for (col = 0; col < 10; col++)
            worksheet_write_number(worksheet, row +2, col +1, data[row][col], NULL);
}

/* Reset the conditional format options back to their initial state. */
void reset_conditional_format(lxw_conditional_format *conditional_format) {
    memset(conditional_format, 0, sizeof(lxw_conditional_format));
}

int main() {

    lxw_workbook  *workbook   = workbook_new("conditional_format.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet4 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet5 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet6 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet7 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet8 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet9 = workbook_add_worksheet(workbook, NULL);

    /* Add a format. Light red fill with dark red text. */
    lxw_format *format1 = workbook_add_format(workbook);
    format_set_bg_color(format1, 0xFFC7CE);
    format_set_font_color(format1, 0x9C0006);

    /* Add a format. Green fill with dark green text. */
    lxw_format *format2 = workbook_add_format(workbook);
    format_set_bg_color(format2, 0xC6EFCE);
    format_set_font_color(format2, 0x006100);

    /* Create a single conditional format object to reuse in the examples. */
    lxw_conditional_format *conditional_format = calloc(1, sizeof(lxw_conditional_format));

    /*
     * Example 1. Conditional formatting based on simple cell based criteria.
     */

    write_worksheet_data(worksheet1);

    worksheet_write_string(worksheet1,
                           CELL("A1"),
                           "Cells with values >= 50 are in light red. "
                           "Values < 50 are in light green.",
                           NULL);

    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_GREATER_THAN_OR_EQUAL_TO;
    conditional_format->value    = 50;
    conditional_format->format   = format1;
    worksheet_conditional_format_range(worksheet1, RANGE("B3:K12"), conditional_format);

    conditional_format->type     = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_LESS_THAN;
    conditional_format->value    = 50;
    conditional_format->format   = format2;
    worksheet_conditional_format_range(worksheet1, RANGE("B3:K12"), conditional_format);

    /*
     * Example 2. Conditional formatting based on max and min values.
     */

    write_worksheet_data(worksheet2);

    worksheet_write_string(worksheet2,
                           CELL("A1"),
                           "Values between 30 and 70 are in light red. "
                           "Values outside that range are in light green.",
                           NULL);

    conditional_format->type      = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria  = LXW_CONDITIONAL_CRITERIA_BETWEEN;
    conditional_format->min_value = 30;
    conditional_format->max_value = 70;
    conditional_format->format    = format1;
    worksheet_conditional_format_range(worksheet2, RANGE("B3:K12"), conditional_format);

    conditional_format->type      = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria  = LXW_CONDITIONAL_CRITERIA_NOT_BETWEEN;
    conditional_format->min_value = 30;
    conditional_format->max_value = 70;
    conditional_format->format    = format2;
    worksheet_conditional_format_range(worksheet2, RANGE("B3:K12"), conditional_format);


    /*
     * Example 3. Conditional formatting with duplicate and unique values.
     */

    write_worksheet_data(worksheet3);

    worksheet_write_string(worksheet3,
                           CELL("A1"),
                           "Duplicate values are in light red. "
                           "Unique values are in light green.",
                           NULL);

    conditional_format->type      = LXW_CONDITIONAL_TYPE_DUPLICATE;
    conditional_format->format    = format1;
    worksheet_conditional_format_range(worksheet3, RANGE("B3:K12"), conditional_format);

    conditional_format->type      = LXW_CONDITIONAL_TYPE_UNIQUE;
    conditional_format->format    = format2;
    worksheet_conditional_format_range(worksheet3, RANGE("B3:K12"), conditional_format);


    /*
     * Example 4. Conditional formatting with above and below average values.
     */

    write_worksheet_data(worksheet4);

    worksheet_write_string(worksheet4,
                           CELL("A1"),
                           "Above average values are in light red. "
                           "Below average values are in light green.",
                           NULL);


    conditional_format->type     = LXW_CONDITIONAL_TYPE_AVERAGE;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_AVERAGE_ABOVE;
    conditional_format->format   = format1;
    worksheet_conditional_format_range(worksheet4, RANGE("B3:K12"), conditional_format);

    conditional_format->type     = LXW_CONDITIONAL_TYPE_AVERAGE;
    conditional_format->criteria = LXW_CONDITIONAL_CRITERIA_AVERAGE_BELOW;
    conditional_format->format   = format2;
    worksheet_conditional_format_range(worksheet4, RANGE("B3:K12"), conditional_format);


    /*
     * Example 5. Conditional formatting with top and bottom values.
     */

    write_worksheet_data(worksheet5);

    worksheet_write_string(worksheet5,
                           CELL("A1"),
                           "Top 10 values are in light red. "
                           "Bottom 10 values are in light green.",
                           NULL);

    conditional_format->type     = LXW_CONDITIONAL_TYPE_TOP;
    conditional_format->value    = 10;
    conditional_format->format   = format1;
    worksheet_conditional_format_range(worksheet5, RANGE("B3:K12"), conditional_format);

    conditional_format->type     = LXW_CONDITIONAL_TYPE_BOTTOM;
    conditional_format->value    = 10;
    conditional_format->format   = format2;
    worksheet_conditional_format_range(worksheet5, RANGE("B3:K12"), conditional_format);


    /*
     * Example 6. Conditional formatting with multiple ranges.
     */

    write_worksheet_data(worksheet6);

    worksheet_write_string(worksheet6,
                           CELL("A1"),
                           "Cells with values >= 50 are in light red."
                           "Values < 50 are in light green. Non-contiguous ranges.",
                           NULL);

    conditional_format->type        = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria    = LXW_CONDITIONAL_CRITERIA_GREATER_THAN_OR_EQUAL_TO;
    conditional_format->value       = 50;
    conditional_format->format      = format1;
    conditional_format->multi_range = "B3:K6 B9:K12";
    worksheet_conditional_format_range(worksheet6, RANGE("B3:K12"), conditional_format);

    conditional_format->type        = LXW_CONDITIONAL_TYPE_CELL;
    conditional_format->criteria    = LXW_CONDITIONAL_CRITERIA_LESS_THAN;
    conditional_format->value       = 50;
    conditional_format->format      = format2;
    conditional_format->multi_range = "B3:K6 B9:K12";
    worksheet_conditional_format_range(worksheet6, RANGE("B3:K12"), conditional_format);

    /* Reset the options before the next example. */
    reset_conditional_format(conditional_format);


    /*
     * Example 7. Conditional formatting with 2 color scales.
     */

    /* Write the worksheet data. */
    for (int i = 1; i <= 12; i++) {
        worksheet_write_number(worksheet7, i + 1, 1, i, NULL);
        worksheet_write_number(worksheet7, i + 1, 3, i, NULL);
        worksheet_write_number(worksheet7, i + 1, 6, i, NULL);
        worksheet_write_number(worksheet7, i + 1, 8, i, NULL);
    }


    worksheet_write_string(worksheet7,
                           CELL("A1"),
                           "Examples of color scales with default and user colors.",
                           NULL);

    worksheet_write_string(worksheet7, CELL("B2"), "2 Color Scale",               NULL);
    worksheet_write_string(worksheet7, CELL("D2"), "2 Color Scale + user colors", NULL);
    worksheet_write_string(worksheet7, CELL("G2"), "3 Color Scale",               NULL);
    worksheet_write_string(worksheet7, CELL("I2"), "3 Color Scale + user colors", NULL);

    /* 2 color scale with standard colors. */
    conditional_format->type      = LXW_CONDITIONAL_2_COLOR_SCALE;
    worksheet_conditional_format_range(worksheet7, RANGE("B3:B14"), conditional_format);

    /* 2 color scale with user defined colors. */
    conditional_format->type      = LXW_CONDITIONAL_2_COLOR_SCALE;
    conditional_format->min_color = 0xFF0000;
    conditional_format->max_color = 0x00FF00;
    worksheet_conditional_format_range(worksheet7, RANGE("D3:D14"), conditional_format);

    /* Reset the colors before the next example. */
    reset_conditional_format(conditional_format);

    /* 3 color scale with standard colors. */
    conditional_format->type     = LXW_CONDITIONAL_3_COLOR_SCALE;
    worksheet_conditional_format_range(worksheet7, RANGE("G3:G14"), conditional_format);

    /* 3 color scale with user defined colors. */
    conditional_format->type      = LXW_CONDITIONAL_3_COLOR_SCALE;
    conditional_format->min_color = 0xC5D9F1;
    conditional_format->mid_color = 0x8DB4E3;
    conditional_format->max_color = 0x538ED5;
    worksheet_conditional_format_range(worksheet7, RANGE("I3:I14"), conditional_format);
    reset_conditional_format(conditional_format);

    /*
     * Example 8. Conditional formatting with data bars.
     */

    /* Write the worksheet data. */
    for (int i = 1; i <= 12; i++) {
        worksheet_write_number(worksheet8, i + 1, 1, i, NULL);
        worksheet_write_number(worksheet8, i + 1, 3, i, NULL);
        worksheet_write_number(worksheet8, i + 1, 5, i, NULL);
        worksheet_write_number(worksheet8, i + 1, 7, i, NULL);
        worksheet_write_number(worksheet8, i + 1, 9, i, NULL);
    }

    int data[] = {-1, -2, -3, -2, -1, 0, 1, 2, 3, 2, 1, 0};
    for (int i = 1; i <= 12; i++) {
        worksheet_write_number(worksheet8, i + 1, 11, data[i -1], NULL);
        worksheet_write_number(worksheet8, i + 1, 13, data[i -1], NULL);
    }

    worksheet_write_string(worksheet8,
                           CELL("A1"),
                           "Examples of data bars.",
                           NULL);

    worksheet_write_string(worksheet8, CELL("B2"), "Default data bars",         NULL);
    worksheet_write_string(worksheet8, CELL("D2"), "Bars only",                 NULL);
    worksheet_write_string(worksheet8, CELL("F2"), "With user color",           NULL);
    worksheet_write_string(worksheet8, CELL("H2"), "Solid bars",                NULL);
    worksheet_write_string(worksheet8, CELL("J2"), "Right to left",             NULL);
    worksheet_write_string(worksheet8, CELL("L2"), "Excel 2010 style",          NULL);
    worksheet_write_string(worksheet8, CELL("N2"), "Negative same as positive", NULL);


    conditional_format->type          = LXW_CONDITIONAL_DATA_BAR;
    worksheet_conditional_format_range(worksheet8, RANGE("B3:B14"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type          = LXW_CONDITIONAL_DATA_BAR;
    conditional_format->bar_only      = LXW_TRUE;
    worksheet_conditional_format_range(worksheet8, RANGE("D3:D14"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type          = LXW_CONDITIONAL_DATA_BAR;
    conditional_format->bar_color     = 0x63C384;
    worksheet_conditional_format_range(worksheet8, RANGE("F3:F14"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type          = LXW_CONDITIONAL_DATA_BAR;
    conditional_format->bar_solid     = LXW_TRUE;
    worksheet_conditional_format_range(worksheet8, RANGE("H3:H14"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type          = LXW_CONDITIONAL_DATA_BAR;
    conditional_format->bar_direction = LXW_CONDITIONAL_BAR_DIRECTION_RIGHT_TO_LEFT;
    worksheet_conditional_format_range(worksheet8, RANGE("J3:J14"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type          = LXW_CONDITIONAL_DATA_BAR;
    conditional_format->data_bar_2010 = LXW_TRUE;
    worksheet_conditional_format_range(worksheet8, RANGE("L3:L14"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type                           = LXW_CONDITIONAL_DATA_BAR;
    conditional_format->bar_negative_color_same        = LXW_TRUE;
    conditional_format->bar_negative_border_color_same = LXW_TRUE;
    worksheet_conditional_format_range(worksheet8, RANGE("N3:N14"), conditional_format);
    reset_conditional_format(conditional_format);


    /*
     * Example 9. Conditional formatting with icon sets.
     */

    /* Write the worksheet data. */
    for (int i = 1; i <= 3; i++) {
        worksheet_write_number(worksheet9, 2, i, i, NULL);
        worksheet_write_number(worksheet9, 3, i, i, NULL);
        worksheet_write_number(worksheet9, 4, i, i, NULL);
        worksheet_write_number(worksheet9, 5, i, i, NULL);
    }

    for (int i = 1; i <= 4; i++) {
        worksheet_write_number(worksheet9, 6, i, i, NULL);
    }

    for (int i = 1; i <= 5; i++) {
        worksheet_write_number(worksheet9, 7, i, i, NULL);
        worksheet_write_number(worksheet9, 8, i, i, NULL);
    }


    worksheet_write_string(worksheet9,
                           CELL("A1"),
                           "Examples of conditional formats with icon sets.",
                           NULL);


    conditional_format->type          = LXW_CONDITIONAL_TYPE_ICON_SETS;
    conditional_format->icon_style    = LXW_CONDITIONAL_ICONS_3_TRAFFIC_LIGHTS_UNRIMMED;
    worksheet_conditional_format_range(worksheet9, RANGE("B3:D3"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type          = LXW_CONDITIONAL_TYPE_ICON_SETS;
    conditional_format->icon_style    = LXW_CONDITIONAL_ICONS_3_TRAFFIC_LIGHTS_UNRIMMED;
    conditional_format->reverse_icons = LXW_TRUE;
    worksheet_conditional_format_range(worksheet9, RANGE("B4:D4"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type          = LXW_CONDITIONAL_TYPE_ICON_SETS;
    conditional_format->icon_style    = LXW_CONDITIONAL_ICONS_3_TRAFFIC_LIGHTS_UNRIMMED;
    conditional_format->icons_only    = LXW_TRUE;
    worksheet_conditional_format_range(worksheet9, RANGE("B5:D5"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type          = LXW_CONDITIONAL_TYPE_ICON_SETS;
    conditional_format->icon_style    = LXW_CONDITIONAL_ICONS_3_ARROWS_COLORED;
    worksheet_conditional_format_range(worksheet9, RANGE("B6:D6"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type          = LXW_CONDITIONAL_TYPE_ICON_SETS;
    conditional_format->icon_style    = LXW_CONDITIONAL_ICONS_4_ARROWS_COLORED;
    worksheet_conditional_format_range(worksheet9, RANGE("B7:E7"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type          = LXW_CONDITIONAL_TYPE_ICON_SETS;
    conditional_format->icon_style    = LXW_CONDITIONAL_ICONS_5_ARROWS_COLORED;
    worksheet_conditional_format_range(worksheet9, RANGE("B8:F8"), conditional_format);
    reset_conditional_format(conditional_format);

    conditional_format->type          = LXW_CONDITIONAL_TYPE_ICON_SETS;
    conditional_format->icon_style    = LXW_CONDITIONAL_ICONS_5_RATINGS;
    worksheet_conditional_format_range(worksheet9, RANGE("B9:F9"), conditional_format);
    reset_conditional_format(conditional_format);


    free(conditional_format);
    return workbook_close(workbook);
}
