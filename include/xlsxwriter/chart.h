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

    LWX_CHART_END_REMOVEP_LATER
};

enum lxw_chart_subtypes {

    LXW_CHART_SUBTYPE_NONE = 0,
    LXW_CHART_SUBTYPE_STACKED,
    LXW_CHART_SUBTYPE_STACKED_PERCENT
};

typedef struct lxw_series_range {
    char *formula;
    char *sheetname;
    lxw_row_t first_row;
    lxw_row_t last_row;
    lxw_col_t first_col;
    lxw_col_t last_col;
    uint8_t ignore_cache;

    uint16_t num_data_points;
    struct lxw_series_data_points *data_cache;

} lxw_series_range;

typedef struct lxw_series_data_point {
    double number;

    STAILQ_ENTRY (lxw_series_data_point) list_pointers;

} lxw_series_data_point;

typedef struct lxw_chart_series {
    lxw_series_range *categories;
    lxw_series_range *values;

    STAILQ_ENTRY (lxw_chart_series) list_pointers;

} lxw_chart_series;

typedef struct lxw_axis {

    char num_format[LXW_CHART_NUM_FORMAT_LEN];
    char default_num_format[LXW_CHART_NUM_FORMAT_LEN];

} lxw_axis;

/*
 * Struct to represent a chart object.
 */
typedef struct lxw_chart {

    FILE *file;

    uint8_t type;
    uint8_t subtype;
    uint16_t series_index;

    lxw_axis x_axis;
    lxw_axis y_axis;

    uint32_t id;
    uint32_t axis_id_1;
    uint32_t axis_id_2;
    uint32_t axis_id_3;
    uint32_t axis_id_4;

    uint8_t in_use;
    uint8_t cat_has_num_fmt;

    uint8_t has_overlap;
    int series_overlap_1;

    char grouping[32];
    char cat_axis_position[2];
    char val_axis_position[2];

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

int lxw_chart_add_data_cache(lxw_series_range *range, uint8_t *data,
                             uint16_t rows, uint8_t cols, uint8_t col);

/* Declarations required for unit testing. */
#ifdef TESTING

STATIC void _chart_xml_declaration(lxw_chart *self);

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_CHART_H__ */
