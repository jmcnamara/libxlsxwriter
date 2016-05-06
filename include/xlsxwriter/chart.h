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

/** Available chart types . */
enum lxw_chart_types {

    /** None. */
    LXW_CHART_NONE = 0,

    /** Bar chart. */
    LXW_CHART_BAR
};

typedef struct lxw_series_range {
    char *formula;
    char *sheetname;
    uint32_t first_row;
    uint32_t last_row;
    uint16_t first_col;
    uint16_t last_col;

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

/*
 * Struct to represent a chart object.
 */
typedef struct lxw_chart {

    FILE *file;

    uint8_t type;
    uint16_t series_index;

    uint32_t id;
    uint32_t axis_id_1;
    uint32_t axis_id_2;
    uint32_t axis_id_3;
    uint32_t axis_id_4;

    uint8_t in_use;
    uint8_t ignore_cache;
    uint8_t cat_has_num_fmt;

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
