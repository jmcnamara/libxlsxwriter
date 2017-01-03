/*
 * libxlsxwriter
 *
 * Copyright 2014-2017, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 * chart - A libxlsxwriter library for creating Excel XLSX chart files.
 *
 */

/**
 * @page chart_page The Chart object
 *
 * The Chart object represents an Excel chart. It provides functions for
 * adding data series to the chart and for configuring the chart.
 *
 * See @ref chart.h for full details of the functionality.
 *
 * @file chart.h
 *
 * @brief Functions related to adding data to and configuring  a chart.
 *
 * The Chart object represents an Excel chart. It provides functions for
 * adding data series to the chart and for configuring the chart.
 *
 * A Chart object isn't created directly. Instead a chart is created by
 * calling the `workbook_add_chart()` function from a Workbook object. For
 * example:
 *
 * @code
 *
 * #include "xlsxwriter.h"
 *
 * int main() {
 *
 *     lxw_workbook  *workbook  = new_workbook("chart.xlsx");
 *     lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
 *
 *     // User function to add data to worksheet, not shown here.
 *     write_worksheet_data(worksheet);
 *
 *     // Create a chart object.
 *     lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_COLUMN);
 *
 *     // In the simplest case we just add some value data series.
 *     // The NULL categories will default to 1 to 5 like in Excel.
 *     chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$5");
 *     chart_add_series(chart, NULL, "=Sheet1!$B$1:$B$5");
 *     chart_add_series(chart, NULL, "=Sheet1!$C$1:$C$5");
 *
 *     // Insert the chart into the worksheet
 *     worksheet_insert_chart(worksheet, CELL("B7"), chart);
 *
 *     return workbook_close(workbook);
 * }
 *
 * @endcode
 *
 * The chart in the worksheet will look like this:
 * @image html chart_simple.png
 *
 * The basic procedure for adding a chart to a worksheet is:
 *
 * 1. Create the chart with `workbook_add_chart()`.
 * 2. Add one or more data series to the chart which refers to data in the
 *    workbook using `chart_add_series()`.
 * 3. Configure the chart with the other available functions shown below.
 * 4. Insert the chart into a worksheet using `worksheet_insert_chart()`.
 *
 */

#ifndef __LXW_CHART_H__
#define __LXW_CHART_H__

#include <stdint.h>
#include <string.h>

#include "common.h"
#include "format.h"

STAILQ_HEAD(lxw_chart_series_list, lxw_chart_series);
STAILQ_HEAD(lxw_series_data_points, lxw_series_data_point);

#define LXW_CHART_NUM_FORMAT_LEN 128

/**
 * @brief Available chart types.
 */
typedef enum lxw_chart_type {

    /** None. */
    LXW_CHART_NONE = 0,

    /** Area chart. */
    LXW_CHART_AREA,

    /** Area chart - stacked. */
    LXW_CHART_AREA_STACKED,

    /** Area chart - percentage stacked. */
    LXW_CHART_AREA_STACKED_PERCENT,

    /** Bar chart. */
    LXW_CHART_BAR,

    /** Bar chart - stacked. */
    LXW_CHART_BAR_STACKED,

    /** Bar chart - percentage stacked. */
    LXW_CHART_BAR_STACKED_PERCENT,

    /** Column chart. */
    LXW_CHART_COLUMN,

    /** Column chart - stacked. */
    LXW_CHART_COLUMN_STACKED,

    /** Column chart - percentage stacked. */
    LXW_CHART_COLUMN_STACKED_PERCENT,

    /** Doughnut chart. */
    LXW_CHART_DOUGHNUT,

    /** Line chart. */
    LXW_CHART_LINE,

    /** Pie chart. */
    LXW_CHART_PIE,

    /** Scatter chart. */
    LXW_CHART_SCATTER,

    /** Scatter chart - straight. */
    LXW_CHART_SCATTER_STRAIGHT,

    /** Scatter chart - straight with markers. */
    LXW_CHART_SCATTER_STRAIGHT_WITH_MARKERS,

    /** Scatter chart - smooth. */
    LXW_CHART_SCATTER_SMOOTH,

    /** Scatter chart - smooth with markers. */
    LXW_CHART_SCATTER_SMOOTH_WITH_MARKERS,

    /** Radar chart. */
    LXW_CHART_RADAR,

    /** Radar chart - with markers. */
    LXW_CHART_RADAR_WITH_MARKERS,

    /** Radar chart - filled. */
    LXW_CHART_RADAR_FILLED
} lxw_chart_type;

/**
 * @brief Chart legend positions.
 */
typedef enum lxw_chart_legend_position {

    /** No chart legend. */
    LXW_CHART_LEGEND_NONE = 0,

    /** Chart legend positioned at right side. */
    LXW_CHART_LEGEND_RIGHT,

    /** Chart legend positioned at left side. */
    LXW_CHART_LEGEND_LEFT,

    /** Chart legend positioned at top. */
    LXW_CHART_LEGEND_TOP,

    /** Chart legend positioned at bottom. */
    LXW_CHART_LEGEND_BOTTOM,

    /** Chart legend overlaid at right side. */
    LXW_CHART_LEGEND_OVERLAY_RIGHT,

    /** Chart legend overlaid at left side. */
    LXW_CHART_LEGEND_OVERLAY_LEFT
} lxw_chart_legend_position;

/**
 * @brief Chart line dash types.
 *
 * The dash types are shown in the order that they appear in the Excel dialog.
 * See @ref chart_lines.
 */
typedef enum lxw_chart_line_dash_type {

    /** Solid. */
    LXW_CHART_LINE_DASH_SOLID = 0,

    /** Round Dot. */
    LXW_CHART_LINE_DASH_ROUND_DOT,

    /** Square Dot. */
    LXW_CHART_LINE_DASH_SQUARE_DOT,

    /** Dash. */
    LXW_CHART_LINE_DASH_DASH,

    /** Dash Dot. */
    LXW_CHART_LINE_DASH_DASH_DOT,

    /** Long Dash. */
    LXW_CHART_LINE_DASH_LONG_DASH,

    /** Long Dash Dot. */
    LXW_CHART_LINE_DASH_LONG_DASH_DOT,

    /** Long Dash Dot Dot. */
    LXW_CHART_LINE_DASH_LONG_DASH_DOT_DOT,

    /* These aren't available in the dialog but are used by Excel. */
    LXW_CHART_LINE_DASH_DOT,
    LXW_CHART_LINE_DASH_SYSTEM_DASH_DOT,
    LXW_CHART_LINE_DASH_SYSTEM_DASH_DOT_DOT
} lxw_chart_line_dash_type;

/**
 * @brief Chart marker types.
 */
typedef enum lxw_chart_marker_type {

    /** Automatic, series default, marker type. */
    LXW_CHART_MARKER_AUTOMATIC,

    /** No marker type. */
    LXW_CHART_MARKER_NONE,

    /** Square marker type. */
    LXW_CHART_MARKER_SQUARE,

    /** Diamond marker type. */
    LXW_CHART_MARKER_DIAMOND,

    /** Triangle marker type. */
    LXW_CHART_MARKER_TRIANGLE,

    /** X shape marker type. */
    LXW_CHART_MARKER_X,

    /** Star marker type. */
    LXW_CHART_MARKER_STAR,

    /** Short dash marker type. */
    LXW_CHART_MARKER_SHORT_DASH,

    /** Long dash marker type. */
    LXW_CHART_MARKER_LONG_DASH,

    /** Circle marker type. */
    LXW_CHART_MARKER_CIRCLE,

    /** Plus (+) marker type. */
    LXW_CHART_MARKER_PLUS
} lxw_chart_marker_type;

/**
 * @brief Chart pattern types.
 */
typedef enum lxw_chart_pattern_type {

    /** None pattern. */
    LXW_CHART_PATTERN_NONE,

    /** 5 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_5,

    /** 10 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_10,

    /** 20 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_20,

    /** 25 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_25,

    /** 30 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_30,

    /** 40 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_40,

    /** 50 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_50,

    /** 60 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_60,

    /** 70 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_70,

    /** 75 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_75,

    /** 80 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_80,

    /** 90 Percent pattern. */
    LXW_CHART_PATTERN_PERCENT_90,

    /** Light downward diagonal pattern. */
    LXW_CHART_PATTERN_LIGHT_DOWNWARD_DIAGONAL,

    /** Light upward diagonal pattern. */
    LXW_CHART_PATTERN_LIGHT_UPWARD_DIAGONAL,

    /** Dark downward diagonal pattern. */
    LXW_CHART_PATTERN_DARK_DOWNWARD_DIAGONAL,

    /** Dark upward diagonal pattern. */
    LXW_CHART_PATTERN_DARK_UPWARD_DIAGONAL,

    /** Wide downward diagonal pattern. */
    LXW_CHART_PATTERN_WIDE_DOWNWARD_DIAGONAL,

    /** Wide upward diagonal pattern. */
    LXW_CHART_PATTERN_WIDE_UPWARD_DIAGONAL,

    /** Light vertical pattern. */
    LXW_CHART_PATTERN_LIGHT_VERTICAL,

    /** Light horizontal pattern. */
    LXW_CHART_PATTERN_LIGHT_HORIZONTAL,

    /** Narrow vertical pattern. */
    LXW_CHART_PATTERN_NARROW_VERTICAL,

    /** Narrow horizontal pattern. */
    LXW_CHART_PATTERN_NARROW_HORIZONTAL,

    /** Dark vertical pattern. */
    LXW_CHART_PATTERN_DARK_VERTICAL,

    /** Dark horizontal pattern. */
    LXW_CHART_PATTERN_DARK_HORIZONTAL,

    /** Dashed downward diagonal pattern. */
    LXW_CHART_PATTERN_DASHED_DOWNWARD_DIAGONAL,

    /** Dashed upward diagonal pattern. */
    LXW_CHART_PATTERN_DASHED_UPWARD_DIAGONAL,

    /** Dashed horizontal pattern. */
    LXW_CHART_PATTERN_DASHED_HORIZONTAL,

    /** Dashed vertical pattern. */
    LXW_CHART_PATTERN_DASHED_VERTICAL,

    /** Small confetti pattern. */
    LXW_CHART_PATTERN_SMALL_CONFETTI,

    /** Large confetti pattern. */
    LXW_CHART_PATTERN_LARGE_CONFETTI,

    /** Zigzag pattern. */
    LXW_CHART_PATTERN_ZIGZAG,

    /** Wave pattern. */
    LXW_CHART_PATTERN_WAVE,

    /** Diagonal brick pattern. */
    LXW_CHART_PATTERN_DIAGONAL_BRICK,

    /** Horizontal brick pattern. */
    LXW_CHART_PATTERN_HORIZONTAL_BRICK,

    /** Weave pattern. */
    LXW_CHART_PATTERN_WEAVE,

    /** Plaid pattern. */
    LXW_CHART_PATTERN_PLAID,

    /** Divot pattern. */
    LXW_CHART_PATTERN_DIVOT,

    /** Dotted grid pattern. */
    LXW_CHART_PATTERN_DOTTED_GRID,

    /** Dotted diamond pattern. */
    LXW_CHART_PATTERN_DOTTED_DIAMOND,

    /** Shingle pattern. */
    LXW_CHART_PATTERN_SHINGLE,

    /** Trellis pattern. */
    LXW_CHART_PATTERN_TRELLIS,

    /** Sphere pattern. */
    LXW_CHART_PATTERN_SPHERE,

    /** Small grid pattern. */
    LXW_CHART_PATTERN_SMALL_GRID,

    /** Large grid pattern. */
    LXW_CHART_PATTERN_LARGE_GRID,

    /** Small check pattern. */
    LXW_CHART_PATTERN_SMALL_CHECK,

    /** Large check pattern. */
    LXW_CHART_PATTERN_LARGE_CHECK,

    /** Outlined diamond pattern. */
    LXW_CHART_PATTERN_OUTLINED_DIAMOND,

    /** Solid diamond pattern. */
    LXW_CHART_PATTERN_SOLID_DIAMOND
} lxw_chart_pattern_type;

enum lxw_chart_subtype {

    LXW_CHART_SUBTYPE_NONE = 0,
    LXW_CHART_SUBTYPE_STACKED,
    LXW_CHART_SUBTYPE_STACKED_PERCENT
};

enum lxw_chart_grouping {
    LXW_GROUPING_CLUSTERED,
    LXW_GROUPING_STANDARD,
    LXW_GROUPING_PERCENTSTACKED,
    LXW_GROUPING_STACKED
};

enum lxw_chart_axis_tick_position {
    LXW_CHART_AXIS_POSITION_BETWEEN,
    LXW_CHART_AXIS_POSITION_ON_TICK
};

enum lxw_chart_position {
    LXW_CHART_AXIS_RIGHT,
    LXW_CHART_AXIS_LEFT,
    LXW_CHART_AXIS_TOP,
    LXW_CHART_AXIS_BOTTOM
};

typedef struct lxw_series_range {
    char *formula;
    char *sheetname;
    lxw_row_t first_row;
    lxw_row_t last_row;
    lxw_col_t first_col;
    lxw_col_t last_col;
    uint8_t ignore_cache;

    uint8_t has_string_cache;
    uint16_t num_data_points;
    struct lxw_series_data_points *data_cache;

} lxw_series_range;

typedef struct lxw_series_data_point {
    uint8_t is_string;
    double number;
    char *string;
    uint8_t no_data;

    STAILQ_ENTRY (lxw_series_data_point) list_pointers;

} lxw_series_data_point;

/**
 * @brief Struct to represent a chart line.
 *
 * See @ref chart_lines.
 */
typedef struct lxw_chart_line {

    /** The chart font color. See @ref working_with_colors. */
    lxw_color_t color;

    /** Turn off/hide line. Set to 0 or 1.*/
    uint8_t none;

    /** Width of the line in increments of 0.25. Default is 2.25. */
    float width;

    /** The line dash type. See #lxw_chart_line_dash_type. */
    uint8_t dash_type;

    /* Transparency for lines isn't generally useful. Undocumented for now. */
    uint8_t transparency;

    /* Members for internal use only. */
    uint8_t has_color;

} lxw_chart_line;

/**
 * @brief Struct to represent a chart fill.
 *
 * See @ref chart_fills.
 */
typedef struct lxw_chart_fill {

    /** The chart font color. See @ref working_with_colors. */
    lxw_color_t color;

    /** Turn off/hide line. Set to 0 or 1.*/
    uint8_t none;

    /** Set the transparency of the fill. 0 - 100. Default 0. */
    uint8_t transparency;

    /* Members for internal use only. */
    uint8_t has_color;

} lxw_chart_fill;

/**
 * @brief Struct to represent a chart pattern.
 *
 * See @ref chart_patterns.
 */
typedef struct lxw_chart_pattern {

    /** The pattern foreground color. See @ref working_with_colors. */
    lxw_color_t fg_color;

    /** The pattern background color. See @ref working_with_colors. */
    lxw_color_t bg_color;

    /** The pattern type. See #lxw_chart_pattern_type. */
    uint8_t type;

    /* Members for internal use only. */
    uint8_t has_fg_color;
    uint8_t has_bg_color;

} lxw_chart_pattern;

/**
 * @brief Struct to represent a chart font.
 *
 * See @ref chart_fonts.
 */
typedef struct lxw_chart_font {

    /** The chart font name, such as "Arial" or "Calibri". */
    char *name;

    /** The chart font size. The default is 11. */
    uint16_t size;

    /** The chart font bold property. Set to 0 or 1. */
    uint8_t bold;

    /** The chart font italic property. Set to 0 or 1. */
    uint8_t italic;

    /** The chart font underline property. Set to 0 or 1. */
    uint8_t underline;

    /** The chart font rotation property. Range: -90 to 90. */
    int32_t rotation;

    /** The chart font color. See @ref working_with_colors. */
    lxw_color_t color;

    /* Members for internal use only. */
    uint8_t pitch_family;
    uint8_t charset;
    int8_t baseline;
    uint8_t has_color;

} lxw_chart_font;

typedef struct lxw_chart_marker {

    uint8_t type;
    uint8_t size;
    lxw_chart_line *line;
    lxw_chart_fill *fill;
    lxw_chart_pattern *pattern;

} lxw_chart_marker;

typedef struct lxw_chart_legend {

    lxw_chart_font *font;
    uint8_t position;

} lxw_chart_legend;

typedef struct lxw_chart_title {

    char *name;
    lxw_row_t row;
    lxw_col_t col;
    lxw_chart_font *font;
    uint8_t off;
    uint8_t is_horizontal;
    uint8_t ignore_cache;

    /* We use a range to hold the title formula properties even though it
     * will only have 1 point in order to re-use similar functions.*/
    lxw_series_range *range;

    struct lxw_series_data_point data_point;

} lxw_chart_title;

/**
 * @brief Struct to represent an Excel chart data series.
 *
 * The lxw_chart_series is created using the chart_add_series function. It is
 * used in functions that modify a chart series but the members of the struct
 * aren't modified directly.
 */
typedef struct lxw_chart_series {

    lxw_series_range *categories;
    lxw_series_range *values;
    lxw_chart_title title;
    lxw_chart_line *line;
    lxw_chart_fill *fill;
    lxw_chart_pattern *pattern;
    lxw_chart_marker *marker;

    STAILQ_ENTRY (lxw_chart_series) list_pointers;

} lxw_chart_series;

/**
 * @brief Struct to represent an Excel chart axis.
 *
 * The lxw_chart_axis struct is used in functions that modify a chart axis
 * but the members of the struct aren't modified directly.
 */
typedef struct lxw_chart_axis {

    lxw_chart_title title;

    char num_format[LXW_CHART_NUM_FORMAT_LEN];
    char default_num_format[LXW_CHART_NUM_FORMAT_LEN];

    uint8_t default_major_gridlines;
    uint8_t major_tick_mark;
    uint8_t is_horizontal;

    lxw_chart_font *num_font;
    lxw_chart_line *line;
    lxw_chart_fill *fill;
    lxw_chart_pattern *pattern;

    uint8_t reverse;
    uint8_t has_min;
    uint8_t has_max;
    double min;
    double max;
    uint16_t log_base;

} lxw_chart_axis;

/**
 * @brief Struct to represent an Excel chart.
 *
 * The members of the lxw_chart struct aren't modified directly. Instead
 * the chart properties are set by calling the functions shown in chart.h.
 */
typedef struct lxw_chart {

    FILE *file;

    uint8_t type;
    uint8_t subtype;
    uint16_t series_index;

    void (*write_chart_type) (struct lxw_chart *);
    void (*write_plot_area) (struct lxw_chart *);

    /**
     * A pointer to the chart x_axis object which can be used in functions
     * that configures the X axis.
     */
    lxw_chart_axis *x_axis;

    /**
     * A pointer to the chart y_axis object which can be used in functions
     * that configures the Y axis.
     */
    lxw_chart_axis *y_axis;

    lxw_chart_title title;

    uint32_t id;
    uint32_t axis_id_1;
    uint32_t axis_id_2;
    uint32_t axis_id_3;
    uint32_t axis_id_4;

    uint8_t in_use;
    uint8_t is_scatter_chart;
    uint8_t cat_has_num_fmt;

    uint8_t has_horiz_cat_axis;
    uint8_t has_horiz_val_axis;

    uint8_t style_id;
    uint16_t rotation;
    uint16_t hole_size;

    uint8_t no_title;
    uint8_t has_overlap;
    int series_overlap_1;

    uint8_t grouping;
    uint8_t cross_between;
    uint8_t cat_axis_position;
    uint8_t val_axis_position;

    lxw_chart_legend legend;
    int16_t *delete_series;
    uint16_t delete_series_count;
    lxw_chart_marker *default_marker;

    struct lxw_chart_series_list *series_list;

    STAILQ_ENTRY (lxw_chart) ordered_list_pointers;
    STAILQ_ENTRY (lxw_chart) list_pointers;

} lxw_chart;


/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

lxw_chart *lxw_chart_new(uint8_t type);
void lxw_chart_free(lxw_chart *chart);
void lxw_chart_assemble_xml_file(lxw_chart *chart);

/**
 * @brief Add a data series to a chart.
 *
 * @param chart      Pointer to a lxw_chart instance to be configured.
 * @param categories The range of categories in the data series.
 * @param values     The range of values in the data series.
 *
 * @return A lxw_chart_series object pointer.
 *
 * In Excel a chart **series** is a collection of information that defines
 * which data is plotted such as the categories and values. It is also used to
 * define the formatting for the data.
 *
 * For an libxlsxwriter chart object the `%chart_add_series()` function is
 * used to set the categories and values of the series:
 *
 * @code
 *     chart_add_series(chart, "=Sheet1!$A$2:$A$7", "=Sheet1!$C$2:$C$7");
 * @endcode
 *
 *
 * The series parameters are:
 *
 * - `categories`: This sets the chart category labels. The category is more
 *   or less the same as the X axis. In most Excel chart types the
 *   `categories` property is optional and the chart will just assume a
 *   sequential series from `1..n`:
 *
 * @code
 *     // The NULL category will default to 1 to 5 like in Excel.
 *     chart_add_series(chart, NULL, "Sheet1!$A$1:$A$5");
 * @endcode
 *
 *  - `values`: This is the most important property of a series and is the
 *    only mandatory option for every chart object. This parameter links the
 *    chart with the worksheet data that it displays.
 *
 * The `categories` and `values` should be a string formula like
 * `"=Sheet1!$A$2:$A$7"` in the same way it is represented in Excel. This is
 * convenient when recreating a chart from an example in Excel but it is
 * trickier to generate programmatically. For these cases you can set the
 * `categories` and `values` to `NULL` and use the
 * `chart_series_set_categories()` and `chart_series_set_values()` functions:
 *
 * @code
 *     lxw_chart_series *series = chart_add_series(chart, NULL, NULL);
 *
 *     // Configure the series using a syntax that is easier to define programmatically.
 *     chart_series_set_categories(series, "Sheet1", 1, 0, 6, 0); // "=Sheet1!$A$2:$A$7"
 *     chart_series_set_values(    series, "Sheet1", 1, 2, 6, 2); // "=Sheet1!$C$2:$C$7"
 * @endcode
 *
 * As shown in the previous example the return value from
 * `%chart_add_series()` is a lxw_chart_series pointer. This can be used in
 * other functions that configure a series.
 *
 *
 * More than one series can be added to a chart. The series numbering and
 * order in the Excel chart will be the same as the order in which they are
 * added in libxlsxwriter:
 *
 * @code
 *    chart_add_series(chart, NULL, "Sheet1!$A$1:$A$5");
 *    chart_add_series(chart, NULL, "Sheet1!$B$1:$B$5");
 *    chart_add_series(chart, NULL, "Sheet1!$C$1:$C$5");
 * @endcode
 *
 * It is also possible to specify non-contiguous ranges:
 *
 * @code
 *    chart_add_series(
 *        chart,
 *        "=(Sheet1!$A$1:$A$9,Sheet1!$A$14:$A$25)",
 *        "=(Sheet1!$B$1:$B$9,Sheet1!$B$14:$B$25)"
 *    );
 * @endcode
 *
 */
lxw_chart_series *chart_add_series(lxw_chart *chart,
                                   const char *categories,
                                   const char *values);

/**
 * @brief Set a series "categories" range using row and column values.
 *
 * @param series    A series object created via `chart_add_series()`.
 * @param sheetname The name of the worksheet that contains the data range.
 * @param first_row The first row of the range. (All zero indexed.)
 * @param first_col The first column of the range.
 * @param last_row  The last row of the range.
 * @param last_col  The last col of the range.
 *
 * The `categories` and `values` of a chart data series are generally set
 * using the `chart_add_series()` function and Excel range formulas like
 * `"=Sheet1!$A$2:$A$7"`.
 *
 * The `%chart_series_set_categories()` function is an alternative method that
 * is easier to generate programmatically. It requires that you set the
 * `categories` and `values` parameters in `chart_add_series()`to `NULL` and
 * then set them using row and column values in
 * `chart_series_set_categories()` and `chart_series_set_values()`:
 *
 * @code
 *     lxw_chart_series *series = chart_add_series(chart, NULL, NULL);
 *
 *     // Configure the series ranges programmatically.
 *     chart_series_set_categories(series, "Sheet1", 1, 0, 6, 0); // "=Sheet1!$A$2:$A$7"
 *     chart_series_set_values(    series, "Sheet1", 1, 2, 6, 2); // "=Sheet1!$C$2:$C$7"
 * @endcode
 *
 */
void chart_series_set_categories(lxw_chart_series *series,
                                 const char *sheetname, lxw_row_t first_row,
                                 lxw_col_t first_col, lxw_row_t last_row,
                                 lxw_col_t last_col);

/**
 * @brief Set a series "values" range using row and column values.
 *
 * @param series    A series object created via `chart_add_series()`.
 * @param sheetname The name of the worksheet that contains the data range.
 * @param first_row The first row of the range. (All zero indexed.)
 * @param first_col The first column of the range.
 * @param last_row  The last row of the range.
 * @param last_col  The last col of the range.
 *
 * The `categories` and `values` of a chart data series are generally set
 * using the `chart_add_series()` function and Excel range formulas like
 * `"=Sheet1!$A$2:$A$7"`.
 *
 * The `%chart_series_set_values()` function is an alternative method that is
 * easier to generate programmatically. See the documentation for
 * `chart_series_set_categories()` above.
 */
void chart_series_set_values(lxw_chart_series *series, const char *sheetname,
                             lxw_row_t first_row, lxw_col_t first_col,
                             lxw_row_t last_row, lxw_col_t last_col);

/**
 * @brief Set the name of a chart series range.
 *
 * @param series A series object created via `chart_add_series()`.
 * @param name   The series name.
 *
 * The `%chart_series_set_name` function is used to set the name for a chart
 * data series. The series name in Excel is displayed in the chart legend and
 * in the formula bar. The name property is optional and if it isn't supplied
 * it will default to `Series 1..n`.
 *
 * The function applies to a #lxw_chart_series object created using
 * `chart_add_series()`:
 *
 * @code
 *     lxw_chart_series *series = chart_add_series(chart, NULL, "=Sheet1!$B$2:$B$7");
 *
 *     chart_series_set_name(series, "Quarterly budget data");
 * @endcode
 *
 * The name parameter can also be a formula such as `=Sheet1!$A$1` to point to
 * a cell in the workbook that contains the name:
 *
 * @code
 *     lxw_chart_series *series = chart_add_series(chart, NULL, "=Sheet1!$B$2:$B$7");
 *
 *     chart_series_set_name(series, "=Sheet1!$B1$1");
 * @endcode
 *
 * See also the `chart_series_set_name_range()` function to see how to set the
 * name formula programmatically.
 */
void chart_series_set_name(lxw_chart_series *series, const char *name);

/**
 * @brief Set a series name formula using row and column values.
 *
 * @param series    A series object created via `chart_add_series()`.
 * @param sheetname The name of the worksheet that contains the cell range.
 * @param row       The zero indexed row number of the range.
 * @param col       The zero indexed column number of the range.
 *
 * The `%chart_series_set_name_range()` function can be used to set a series
 * name range and is an alternative to using `chart_series_set_name()` and a
 * string formula:
 *
 * @code
 *     lxw_chart_series *series = chart_add_series(chart, NULL, "=Sheet1!$B$2:$B$7");
 *
 *     chart_series_set_name_range(series, "Sheet1", 0, 2); // "=Sheet1!$C$1"
 * @endcode
 */
void chart_series_set_name_range(lxw_chart_series *series,
                                 const char *sheetname, lxw_row_t row,
                                 lxw_col_t col);
/**
 * @brief Set the line properties for a chart series.
 *
 * @param series A series object created via `chart_add_series()`.
 * @param line   A #lxw_chart_line struct.
 *
 * Set the line/border properties of a chart series:
 *
 * @code
 *     lxw_chart_line line = {.color = LXW_COLOR_RED};
 *
 *     chart_series_set_line(series1, &line);
 *     chart_series_set_line(series2, &line);
 *     chart_series_set_line(series3, &line);
 * @endcode
 *
 * @image html chart_series_set_line.png
 *
 * For more information see @ref chart_lines.
 */
void chart_series_set_line(lxw_chart_series *series, lxw_chart_line *line);

/**
 * @brief Set the fill properties for a chart series.
 *
 * @param series A series object created via `chart_add_series()`.
 * @param fill   A #lxw_chart_fill struct.
 *
 * Set the fill properties of a chart series:
 *
 * @code
 *     lxw_chart_fill fill1 = {.color = LXW_COLOR_RED};
 *     lxw_chart_fill fill2 = {.color = LXW_COLOR_YELLOW};
 *     lxw_chart_fill fill3 = {.color = LXW_COLOR_GREEN};
 *
 *     chart_series_set_fill(series1, &fill1);
 *     chart_series_set_fill(series2, &fill2);
 *     chart_series_set_fill(series3, &fill3);
 * @endcode
 *
 * @image html chart_series_set_fill.png
 *
 * For more information see @ref chart_fills.
 */
void chart_series_set_fill(lxw_chart_series *series, lxw_chart_fill *fill);

/**
 * @brief Set the pattern properties for a chart series.
 *
 * @param series  A series object created via `chart_add_series()`.
 * @param pattern A #lxw_chart_pattern struct.
 *
 * Set the pattern properties of a chart series:
 *
 * @code
 *     lxw_chart_pattern pattern1 = {.type = LXW_CHART_PATTERN_SHINGLE,
 *                                   .fg_color = 0x804000,
 *                                   .bg_color = 0XC68C53};
 *
 *     lxw_chart_pattern pattern2 = {.type = LXW_CHART_PATTERN_HORIZONTAL_BRICK,
 *                                   .fg_color = 0XB30000,
 *                                   .bg_color = 0XFF6666};
 *
 *     chart_series_set_pattern(series1, &pattern1);
 *     chart_series_set_pattern(series2, &pattern2);
 *
 * @endcode
 *
 * @image html chart_pattern.png
 *
 * For more information see #lxw_chart_pattern_type and @ref chart_patterns.
 */
void chart_series_set_pattern(lxw_chart_series *series,
                              lxw_chart_pattern *pattern);

/**
 * @brief Set the data marker type for a series.
 *
 * @param series A series object created via `chart_add_series()`.
 * @param type   The marker type, see #lxw_chart_marker_type.
 *
 * In Excel a chart marker is used to distinguish data points in a plotted
 * series. In general only Line and Scatter and Radar chart types use
 * markers. The libxlsxwriter chart types that can have markers are:
 *
 * - #LXW_CHART_LINE
 * - #LXW_CHART_SCATTER
 * - #LXW_CHART_SCATTER_STRAIGHT
 * - #LXW_CHART_SCATTER_STRAIGHT_WITH_MARKERS
 * - #LXW_CHART_SCATTER_SMOOTH
 * - #LXW_CHART_SCATTER_SMOOTH_WITH_MARKERS
 * - #LXW_CHART_RADAR
 * - #LXW_CHART_RADAR_WITH_MARKERS
 *
 * The chart types with `MARKERS` in the name have markers with default colors
 * and shapes turned on by default but it is possible using the various
 * `chart_series_set_marker_xxx()` functions below to change these defaults. It
 * is also possible to turn on an off markers.
 *
 * The `%chart_series_set_marker_type()` function is used to specify the
 * type of the series marker:
 *
 * @code
 *     chart_series_set_marker_type(series, LXW_CHART_MARKER_DIAMOND);
 * @endcode
 *
 * @image html chart_marker1.png
 *
 * The available marker types defined by #lxw_chart_marker_type are:
 *
 * - #LXW_CHART_MARKER_AUTOMATIC
 * - #LXW_CHART_MARKER_NONE
 * - #LXW_CHART_MARKER_SQUARE
 * - #LXW_CHART_MARKER_DIAMOND
 * - #LXW_CHART_MARKER_TRIANGLE
 * - #LXW_CHART_MARKER_X
 * - #LXW_CHART_MARKER_STAR
 * - #LXW_CHART_MARKER_SHORT_DASH
 * - #LXW_CHART_MARKER_LONG_DASH
 * - #LXW_CHART_MARKER_CIRCLE
 * - #LXW_CHART_MARKER_PLUS
 *
 * The `#LXW_CHART_MARKER_NONE` type can be used to turn off default markers:
 *
 * @code
 *     chart_series_set_marker_type(series, LXW_CHART_MARKER_NONE);
 * @endcode
 *
 * @image html chart_series_set_marker_none.png
 *
 * The `#LXW_CHART_MARKER_AUTOMATIC` type is a special case which turns on a
 * marker using the default marker style for the particular series. If
 * automatic is on then other marker properties such as size, line or fill
 * cannot be set.
 */
void chart_series_set_marker_type(lxw_chart_series *series, uint8_t type);

/**
 * @brief Set the size of a data marker for a series.
 *
 * @param series A series object created via `chart_add_series()`.
 * @param size   The size of the marker.
 *
 * The `%chart_series_set_marker_size()` function is used to specify the
 * size of the series marker:
 *
 * @code
 *     chart_series_set_marker_type(series, LXW_CHART_MARKER_CIRCLE);
 *     chart_series_set_marker_size(series, 10);
 * @endcode
 *
 * @image html chart_series_set_marker_size.png
 *
 */
void chart_series_set_marker_size(lxw_chart_series *series, uint8_t size);

/**
 * @brief Set the line properties for a chart series marker.
 *
 * @param series A series object created via `chart_add_series()`.
 * @param line   A #lxw_chart_line struct.
 *
 * Set the line/border properties of a chart marker:
 *
 * @code
 *     lxw_chart_line line = {.color = LXW_COLOR_BLACK};
 *     lxw_chart_fill fill = {.color = LXW_COLOR_RED};
 *
 *     chart_series_set_marker_type(series, LXW_CHART_MARKER_SQUARE);
 *     chart_series_set_marker_size(series, 8);
 *
 *     chart_series_set_marker_line(series, &line);
 *     chart_series_set_marker_fill(series, &fill);
 * @endcode
 *
 * @image html chart_marker2.png
 *
 * For more information see @ref chart_lines.
 */
void chart_series_set_marker_line(lxw_chart_series *series,
                                  lxw_chart_line *line);

/**
 * @brief Set the fill properties for a chart series marker.
 *
 * @param series A series object created via `chart_add_series()`.
 * @param fill   A #lxw_chart_fill struct.
 *
 * Set the fill properties of a chart marker:
 *
 * @code
 *     chart_series_set_marker_fill(series, &fill);
 * @endcode
 *
 * See the example and image above and also see @ref chart_fills.
 */
void chart_series_set_marker_fill(lxw_chart_series *series,
                                  lxw_chart_fill *fill);

/**
 * @brief Set the pattern properties for a chart series marker.
 *
 * @param series  A series object created via `chart_add_series()`.
 * @param pattern A #lxw_chart_pattern struct.
 *
 * Set the pattern properties of a chart marker:
 *
 * @code
 *     chart_series_set_marker_pattern(series, &pattern);
 * @endcode
 *
 * For more information see #lxw_chart_pattern_type and @ref chart_patterns.
 */
void chart_series_set_marker_pattern(lxw_chart_series *series,
                                     lxw_chart_pattern *pattern);

/**
 * @brief Set the name caption of the an axis.
 *
 * @param axis A pointer to a chart #lxw_chart_axis object.
 * @param name The name caption of the axis.
 *
 * The `%chart_axis_set_name()` function sets the name (also known as title or
 * caption) for an axis. It can be used for the X or Y axes. The name is
 * displayed below an X axis and to the side of a Y axis.
 *
 * @code
 *     chart_axis_set_name(chart->x_axis, "Earnings per Quarter");
 *     chart_axis_set_name(chart->y_axis, "US Dollars (Millions)");
 * @endcode
 *
 * @image html chart_axis_set_name.png
 *
 * The name parameter can also be a formula such as `=Sheet1!$A$1` to point to
 * a cell in the workbook that contains the name:
 *
 * @code
 *     chart_axis_set_name(chart->x_axis, "=Sheet1!$B1$1");
 * @endcode
 *
 * See also the `chart_axis_set_name_range()` function to see how to set the
 * name formula programmatically.
 *
 * This function is applicable to category, date and value axes.
 */
void chart_axis_set_name(lxw_chart_axis *axis, const char *name);

/**
 * @brief Set a chart axis name formula using row and column values.
 *
 * @param axis      A pointer to a chart #lxw_chart_axis object.
 * @param sheetname The name of the worksheet that contains the cell range.
 * @param row       The zero indexed row number of the range.
 * @param col       The zero indexed column number of the range.
 *
 * The `%chart_axis_set_name_range()` function can be used to set an axis name
 * range and is an alternative to using `chart_axis_set_name()` and a string
 * formula:
 *
 * @code
 *     chart_axis_set_name_range(chart->x_axis, "Sheet1", 1, 0);
 *     chart_axis_set_name_range(chart->y_axis, "Sheet1", 2, 0);
 * @endcode
 */
void chart_axis_set_name_range(lxw_chart_axis *axis, const char *sheetname,
                               lxw_row_t row, lxw_col_t col);

/**
 * @brief Set the font properties for a chart axis name.
 *
 * @param axis A pointer to a chart #lxw_chart_axis object.
 * @param font A pointer to a chart #lxw_chart_font font struct.
 *
 * The `%chart_axis_set_name_font()` function is used to set the font of an
 * axis name:
 *
 * @code
 *     lxw_chart_font font = {.bold = LXW_TRUE, .color = LXW_COLOR_BLUE};
 *
 *     chart_axis_set_name(chart->x_axis, "Yearly data");
 *     chart_axis_set_name_font(chart->x_axis, &font);
 * @endcode
 *
 * @image html chart_axis_set_name_font.png
 *
 * For more information see @ref chart_fonts.
 */
void chart_axis_set_name_font(lxw_chart_axis *axis, lxw_chart_font *font);

/**
 * @brief Set the font properties for the numbers of a chart axis.
 *
 * @param axis A pointer to a chart #lxw_chart_axis object.
 * @param font A pointer to a chart #lxw_chart_font font struct.
 *
 * The `%chart_axis_set_num_font()` function is used to set the font of the
 * numbers on an axis:
 *
 * @code
 *     lxw_chart_font font = {.bold = LXW_TRUE, .color = LXW_COLOR_BLUE};
 *
 *     chart_axis_set_num_font(chart->x_axis, &font1);
 * @endcode
 *
 * @image html chart_axis_set_num_font.png
 *
 * For more information see @ref chart_fonts.
 */
void chart_axis_set_num_font(lxw_chart_axis *axis, lxw_chart_font *font);

/**
 * @brief Set the line properties for a chart axis.
 *
 * @param axis A pointer to a chart #lxw_chart_axis object.
 * @param line A #lxw_chart_line struct.
 *
 * Set the line properties of a chart axis:
 *
 * @code
 *     // Hide the Y axis.
 *     lxw_chart_line line = {.none = LXW_TRUE};
 *
 *     chart_axis_set_line(chart->y_axis, &line);
 * @endcode
 *
 * @image html chart_axis_set_line.png
 *
 * For more information see @ref chart_lines.
 */
void chart_axis_set_line(lxw_chart_axis *axis, lxw_chart_line *line);

/**
 * @brief Set the fill properties for a chart axis.
 *
 * @param axis A pointer to a chart #lxw_chart_axis object.
 * @param fill A #lxw_chart_fill struct.
 *
 * Set the fill properties of a chart axis:
 *
 * @code
 *     lxw_chart_fill fill = {.color = LXW_COLOR_YELLOW};
 *
 *     chart_axis_set_fill(chart->y_axis, &fill);
 * @endcode
 *
 * @image html chart_axis_set_fill.png
 *
 * For more information see @ref chart_fills.
 */
void chart_axis_set_fill(lxw_chart_axis *axis, lxw_chart_fill *fill);

/**
 * @brief Set the pattern properties for a chart axis.
 *
 * @param axis    A pointer to a chart #lxw_chart_axis object.
 * @param pattern A #lxw_chart_pattern struct.
 *
 * Set the pattern properties of a chart axis:
 *
 * @code
 *     chart_axis_set_pattern(chart->y_axis, &pattern);
 * @endcode
 *
 * For more information see #lxw_chart_pattern_type and @ref chart_patterns.
 */
void chart_axis_set_pattern(lxw_chart_axis *axis, lxw_chart_pattern *pattern);

/**
 * @brief Reverse the order of the axis categories or values.
 *
 * @param axis A pointer to a chart #lxw_chart_axis object.
 *
 * Reverse the order of the axis categories or values:
 *
 * @code
 *     chart_axis_set_reverse(chart->x_axis);
 * @endcode
 *
 * @image html chart_reverse.png
 *
 * Applicable to category, date and value axes.
 */
void chart_axis_set_reverse(lxw_chart_axis *axis);

/**
 * @brief Set the minimum value for a chart axis.
 *
 * @param axis A pointer to a chart #lxw_chart_axis object.
 * @param min  Minimum value for chart axis. Value axes only.
 *
 * Set the minimum value for the axis range.
 *
 * @code
 *     chart_axis_set_min(chart->y_axis, -4);
 *     chart_axis_set_max(chart->y_axis, 21);
 * @endcode
 *
 * @image html chart_max_min.png
 *
 * @note This function is applicable to value and date axes only.
 *       It isn't applicable to the horizontal category axis in the above
 *       example. For more information see @ref ww_charts_axes.
 *
 */
void chart_axis_set_min(lxw_chart_axis *axis, double min);

/**
 * @brief Set the maximum value for a chart axis.
 *
 * @param axis A pointer to a chart #lxw_chart_axis object.
 * @param max  Maximum value for chart axis. Value axes only.
 *
 * Set the maximum value for the axis range.
 *
 * @code
 *     chart_axis_set_min(chart->y_axis, -4);
 *     chart_axis_set_max(chart->y_axis, 21);
 * @endcode
 *
 * See the above image.
 *
 * @note This function is applicable to value and date axes only.
 *       It isn't applicable to the horizontal category axis in the above
 *       example. For more information see @ref ww_charts_axes.
 */
void chart_axis_set_max(lxw_chart_axis *axis, double max);

/**
 * @brief Set the log base of the axis range.
 *
 * @param axis     A pointer to a chart #lxw_chart_axis object.
 * @param log_base The log base for value axis. Value axes only.
 *
 * Set the log base for the axis:
 *
 * @code
 *     chart_axis_set_log_base(chart->y_axis, 10);
 * @endcode
 *
 * @image html chart_log_base.png
 *
 * The allowable range of values for the log base in Excel is between 2 and
 * 1000.
 *
 * @note This function is applicable to value and date axes only.
 *       It isn't applicable to the horizontal category axis in the above
 *       example. For more information see @ref ww_charts_axes.
 */
void chart_axis_set_log_base(lxw_chart_axis *axis, uint16_t log_base);

/**
 * @brief Set the title of the chart.
 *
 * @param chart Pointer to a lxw_chart instance to be configured.
 * @param name  The chart title name.
 *
 * The `%chart_title_set_name()` function sets the name (title) for the
 * chart. The name is displayed above the chart.
 *
 * @code
 *     chart_title_set_name(chart, "Year End Results");
 * @endcode
 *
 * @image html chart_title_set_name.png
 *
 * The name parameter can also be a formula such as `=Sheet1!$A$1` to point to
 * a cell in the workbook that contains the name:
 *
 * @code
 *     chart_title_set_name(chart, "=Sheet1!$B1$1");
 * @endcode
 *
 * See also the `chart_title_set_name_range()` function to see how to set the
 * name formula programmatically.
 *
 * The Excel default is to have no chart title.
 */
void chart_title_set_name(lxw_chart *chart, const char *name);

/**
 * @brief Set a chart title formula using row and column values.
 *
 * @param chart     Pointer to a lxw_chart instance to be configured.
 * @param sheetname The name of the worksheet that contains the cell range.
 * @param row       The zero indexed row number of the range.
 * @param col       The zero indexed column number of the range.
 *
 * The `%chart_title_set_name_range()` function can be used to set a chart
 * title range and is an alternative to using `chart_title_set_name()` and a
 * string formula:
 *
 * @code
 *     chart_title_set_name_range(chart, "Sheet1", 1, 0);
 * @endcode
 */
void chart_title_set_name_range(lxw_chart *chart, const char *sheetname,
                                lxw_row_t row, lxw_col_t col);

/**
 * @brief  Set the font properties for a chart title.
 *
 * @param chart Pointer to a lxw_chart instance to be configured.
 * @param font  A pointer to a chart #lxw_chart_font font struct.
 *
 * The `%chart_title_set_name_font()` function is used to set the font of a
 * chart title:
 *
 * @code
 *     lxw_chart_font font = {.bold = LXW_TRUE, .color = LXW_COLOR_BLUE};
 *
 *     chart_title_set_name(chart, "Year End Results");
 *     chart_title_set_name_font(chart, &font);
 * @endcode
 *
 * @image html chart_title_set_name_font.png
 *
 * For more information see @ref chart_fonts.
 */
void chart_title_set_name_font(lxw_chart *chart, lxw_chart_font *font);

/**
 * @brief Turn off an automatic chart title.
 *
 * @param chart  Pointer to a lxw_chart instance to be configured.
 *
 * In general in Excel a chart title isn't displayed unless the user
 * explicitly adds one. However, Excel adds an automatic chart title to charts
 * with a single series and a user defined series name. The
 * `chart_title_off()` function allows you to turn off this automatic chart
 * title:
 *
 * @code
 *     chart_title_off(chart);
 * @endcode
 */
void chart_title_off(lxw_chart *chart);

/**
 * @brief Set the position of the chart legend.
 *
 * @param chart    Pointer to a lxw_chart instance to be configured.
 * @param position The #lxw_chart_legend_position value for the legend.
 *
 * The `%chart_legend_set_position()` function is used to set the chart
 * legend to one of the #lxw_chart_legend_position values:
 *
 *     LXW_CHART_LEGEND_NONE
 *     LXW_CHART_LEGEND_RIGHT
 *     LXW_CHART_LEGEND_LEFT
 *     LXW_CHART_LEGEND_TOP
 *     LXW_CHART_LEGEND_BOTTOM
 *     LXW_CHART_LEGEND_OVERLAY_RIGHT
 *     LXW_CHART_LEGEND_OVERLAY_LEFT
 *
 * For example:
 *
 * @code
 *     chart_legend_set_position(chart, LXW_CHART_LEGEND_BOTTOM);
 * @endcode
 *
 * @image html chart_legend_bottom.png
 *
 * This function can also be used to turn off a chart legend:
 *
 * @code
 *     chart_legend_set_position(chart, LXW_CHART_LEGEND_NONE);
 * @endcode
 *
 * @image html chart_legend_none.png
 *
 */
void chart_legend_set_position(lxw_chart *chart, uint8_t position);

/**
 * @brief Set the font properties for a chart legend.
 *
 * @param chart Pointer to a lxw_chart instance to be configured.
 * @param font  A pointer to a chart #lxw_chart_font font struct.
 *
 * The `%chart_legend_set_font()` function is used to set the font of a
 * chart legend:
 *
 * @code
 *     lxw_chart_font font = {.bold = LXW_TRUE, .color = LXW_COLOR_BLUE};
 *
 *     chart_legend_set_font(chart, &font);
 * @endcode
 *
 * @image html chart_legend_set_font.png
 *
 * For more information see @ref chart_fonts.
 */
void chart_legend_set_font(lxw_chart *chart, lxw_chart_font *font);

/**
 * @brief Remove one or more series from the the legend.
 *
 * @param chart         Pointer to a lxw_chart instance to be configured.
 * @param delete_series An array of zero-indexed values to delete from series.
 *
 * @return A #lxw_error.
 *
 * The `%chart_legend_delete_series()` function allows you to remove/hide one
 * or more series in a chart legend (the series will still display on the chart).
 *
 * This function takes an array of one or more zero indexed series
 * numbers. The array should be terminated with -1.
 *
 * For example to remove the first and third zero-indexed series from the
 * legend of a chart with 3 series:
 *
 * @code
 *     int16_t series[] = {0, 2, -1};
 *
 *     chart_legend_delete_series(chart, series);
 * @endcode
 *
 * @image html chart_legend_delete.png
 */
lxw_error chart_legend_delete_series(lxw_chart *chart,
                                     int16_t delete_series[]);

/**
 * @brief Set the chart style type.
 *
 * @param chart    Pointer to a lxw_chart instance to be configured.
 * @param style_id An index representing the chart style, 1 - 48.
 *
 * The `%chart_set_style()` function is used to set the style of the chart to
 * one of the 48 built-in styles available on the "Design" tab in Excel 2007:
 *
 * @code
 *     chart_set_style(chart, 37)
 * @endcode
 *
 * @image html chart_style.png
 *
 * The style index number is counted from 1 on the top left in the Excel
 * dialog. The default style is 2.
 *
 * **Note:**
 *
 * In Excel 2013 the Styles section of the "Design" tab in Excel shows what
 * were referred to as "Layouts" in previous versions of Excel. These layouts
 * are not defined in the file format. They are a collection of modifications
 * to the base chart type. They can not be defined by the `chart_set_style()``
 * function.
 *
 *
 */
void chart_set_style(lxw_chart *chart, uint8_t style_id);

void chart_set_rotation(lxw_chart *chart, uint16_t rotation);
void chart_set_hole_size(lxw_chart *chart, uint8_t size);

int lxw_chart_add_data_cache(lxw_series_range *range, uint8_t *data,
                             uint16_t rows, uint8_t cols, uint8_t col);

/* Declarations required for unit testing. */
#ifdef TESTING

STATIC void _chart_xml_declaration(lxw_chart *chart);
STATIC void _chart_write_legend(lxw_chart *chart);

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_CHART_H__ */
