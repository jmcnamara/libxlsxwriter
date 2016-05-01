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

typedef struct lxw_series_range {
    char *range;
    char *sheetname;
    uint32_t first_row;
    uint32_t last_row;
    uint16_t first_col;
    uint16_t last_col;

} lxw_series_range;

typedef struct lxw_chart_series {
    lxw_series_range values;

    STAILQ_ENTRY (lxw_chart_series) list_pointers;

} lxw_chart_series;

/*
 * Struct to represent a chart object.
 */
typedef struct lxw_chart {

    FILE *file;

    uint16_t series_index;

    uint32_t id;
    uint32_t axis_id_1;
    uint32_t axis_id_2;
    uint32_t axis_id_3;
    uint32_t axis_id_4;

    struct lxw_chart_series_list *series;

} lxw_chart;


/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

lxw_chart *lxw_chart_new();
void lxw_chart_free(lxw_chart *chart);
void lxw_chart_assemble_xml_file(lxw_chart *self);

int chart_add_series(lxw_chart *chart, lxw_chart_series *user_series);

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
