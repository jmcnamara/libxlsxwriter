/*
 * libxlsxwriter
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 * chart - A libxlsxwriter library for creating Excel XLSX chart files.
 *
 */
#ifndef __LXW_CHART_H__
#define __LXW_CHART_H__

#include <stdint.h>
#include <string.h>

#include "common.h"

STAILQ_HEAD(lxw_chart_series_list, lxw_chart_series);
STAILQ_HEAD(lxw_series_data_points, lxw_series_data_point);

#define LXW_CHART_NUM_FORMAT_LEN 128

/** Available chart types . */
enum lxw_chart_types {

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
};

enum lxw_chart_subtypes {

    LXW_CHART_SUBTYPE_NONE = 0,
    LXW_CHART_SUBTYPE_STACKED,
    LXW_CHART_SUBTYPE_STACKED_PERCENT
};

enum lxw_chart_groupings {
    LXW_GROUPING_CLUSTERED,
    LXW_GROUPING_STANDARD,
    LXW_GROUPING_PERCENTSTACKED,
    LXW_GROUPING_STACKED
};

enum lxw_chart_axis_positions {
    LXW_CHART_AXIS_POSITION_BETWEEN,
    LXW_CHART_AXIS_POSITION_ON_TICK
};

enum lxw_chart_positions {
    LXW_CHART_RIGHT,
    LXW_CHART_LEFT,
    LXW_CHART_TOP,
    LXW_CHART_BOTTOM
};

typedef struct lxw_series_range {
    char *formula;
    char *sheetname;
    lxw_row_t first_row;
    lxw_row_t last_row;
    lxw_col_t first_col;
    lxw_col_t last_col;
    uint8_t ignore_cache;

    uint8_t has_number_cache;
    uint16_t num_data_points;
    struct lxw_series_data_points *data_cache;

} lxw_series_range;

typedef struct lxw_series_data_point {
    uint8_t is_string;
    double number;
    char *string;

    STAILQ_ENTRY (lxw_series_data_point) list_pointers;

} lxw_series_data_point;

typedef struct lxw_chart_series {

    lxw_series_range *categories;
    lxw_series_range *values;
    char *name;

    STAILQ_ENTRY (lxw_chart_series) list_pointers;

} lxw_chart_series;

typedef struct lxw_chart_font {

    uint8_t bold;

} lxw_chart_font;

typedef struct lxw_chart_title {

    char *name;
    lxw_chart_font font;
    uint8_t has_formula;
    uint8_t none;
    uint8_t is_horizontal;

} lxw_chart_title;

typedef struct lxw_chart_axis {

    lxw_chart_title title;

    char num_format[LXW_CHART_NUM_FORMAT_LEN];
    char default_num_format[LXW_CHART_NUM_FORMAT_LEN];

    uint8_t default_major_gridlines;
    uint8_t major_tick_mark;

} lxw_chart_axis;

/*
 * Struct to represent a chart object.
 */
typedef struct lxw_chart {

    FILE *file;

    uint8_t type;
    uint8_t subtype;
    uint16_t series_index;

    void (*write_chart_type) (struct lxw_chart *);
    void (*write_plot_area) (struct lxw_chart *);

    lxw_chart_axis *x_axis;
    lxw_chart_axis *y_axis;
    lxw_chart_title title;

    uint32_t id;
    uint32_t axis_id_1;
    uint32_t axis_id_2;
    uint32_t axis_id_3;
    uint32_t axis_id_4;

    uint8_t in_use;
    uint8_t is_scatter;
    uint8_t cat_has_num_fmt;

    uint8_t has_horiz_cat_axis;
    uint8_t has_horiz_val_axis;

    uint8_t style_id;
    uint16_t rotation;
    uint16_t hole_size;

    uint8_t no_title;
    uint8_t has_markers;
    uint8_t has_overlap;
    int series_overlap_1;

    uint8_t grouping;
    uint8_t cross_between;
    uint8_t cat_axis_position;
    uint8_t val_axis_position;

    struct lxw_chart_series_list *series_list;

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
int lxw_chart_init_data_cache(lxw_series_range *range);
lxw_chart_series *chart_add_series(lxw_chart *chart,
                                   char *categories, char *values);

void chart_set_series_name(lxw_chart_series *series, char *name);

void chart_set_axis_name(lxw_chart_axis *axis, char *name);

void chart_set_style(lxw_chart *chart, uint8_t style_id);
void chart_set_title(lxw_chart *chart, lxw_chart_title *title);

void chart_set_rotation(lxw_chart *chart, uint16_t rotation);
void chart_set_hole_size(lxw_chart *chart, uint8_t size);

int lxw_chart_add_data_cache(lxw_series_range *range, uint8_t *data,
                             uint16_t rows, uint8_t cols, uint8_t col);

/* Declarations required for unit testing. */
#ifdef TESTING

STATIC void _chart_xml_declaration(lxw_chart *chart);

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_CHART_H__ */
