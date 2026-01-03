/*
 * libxlsxwriter
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 */

/**
 * @file sparkline.h
 *
 * @brief Sparkline options and types for libxlsxwriter.
 *
 * Sparklines are small charts that fit in a single cell and allow you to
 * show trends in data at a glance.
 *
 */

#ifndef __LXW_SPARKLINE_H__
#define __LXW_SPARKLINE_H__

#include "common.h"

/**
 * @brief Sparkline types.
 */
typedef enum lxw_sparkline_type {
    /** Line sparkline (default). */
    LXW_SPARKLINE_LINE = 0,

    /** Column sparkline. */
    LXW_SPARKLINE_COLUMN,

    /** Win/Loss sparkline. */
    LXW_SPARKLINE_WIN_LOSS
} lxw_sparkline_type;

/**
 * @brief Sparkline empty cell handling.
 */
typedef enum lxw_sparkline_empty_cells {
    /** Show empty cells as gaps (default). */
    LXW_SPARKLINE_EMPTY_CELLS_GAP = 0,

    /** Show empty cells as zeros. */
    LXW_SPARKLINE_EMPTY_CELLS_ZERO,

    /** Connect points across empty cells. */
    LXW_SPARKLINE_EMPTY_CELLS_SPAN
} lxw_sparkline_empty_cells;

/**
 * @brief Sparkline axis type for min/max.
 */
typedef enum lxw_sparkline_axis_type {
    /** Axis value is automatic (default). */
    LXW_SPARKLINE_AXIS_AUTOMATIC = 0,

    /** Axis value is same for all sparklines in group. */
    LXW_SPARKLINE_AXIS_GROUP,

    /** Axis value is set to a custom value. */
    LXW_SPARKLINE_AXIS_CUSTOM
} lxw_sparkline_axis_type;

/**
 * @brief Sparkline options.
 *
 * The members of this struct are used to define a sparkline that is inserted
 * into a worksheet via worksheet_add_sparkline().
 */
typedef struct lxw_sparkline_options {

    /** A string containing the range of data for the sparkline in a format
     *  like "Sheet1!$A$1:$E$1". Required. */
    const char *range;

    /** A string containing the cell location of the sparkline. If this is
     *  NULL, the location is specified by row/col parameters. */
    const char *location;

    /** The type of sparkline: line, column, or win/loss.
     *  See #lxw_sparkline_type. Default is LXW_SPARKLINE_LINE. */
    uint8_t type;

    /** The style index (0-35) from the predefined Excel sparkline styles.
     *  Default is 0. */
    uint8_t style;

    /** Show high point marker. */
    uint8_t high_point;

    /** Show low point marker. */
    uint8_t low_point;

    /** Show negative point markers. */
    uint8_t negative_points;

    /** Show first point marker. */
    uint8_t first_point;

    /** Show last point marker. */
    uint8_t last_point;

    /** Show markers on all points (line sparkline only). */
    uint8_t markers;

    /** Custom color for sparkline series (RGB). Use 0 for style default. */
    lxw_color_t series_color;

    /** Custom color for negative values (RGB). Use 0 for style default. */
    lxw_color_t negative_color;

    /** Custom color for markers (RGB). Use 0 for style default. */
    lxw_color_t markers_color;

    /** Custom color for first point (RGB). Use 0 for style default. */
    lxw_color_t first_color;

    /** Custom color for last point (RGB). Use 0 for style default. */
    lxw_color_t last_color;

    /** Custom color for high point (RGB). Use 0 for style default. */
    lxw_color_t high_color;

    /** Custom color for low point (RGB). Use 0 for style default. */
    lxw_color_t low_color;

    /** Minimum axis value for sparkline. */
    double min;

    /** Maximum axis value for sparkline. */
    double max;

    /** How min axis is calculated. See #lxw_sparkline_axis_type. */
    uint8_t min_axis_type;

    /** How max axis is calculated. See #lxw_sparkline_axis_type. */
    uint8_t max_axis_type;

    /** Show the horizontal axis. */
    uint8_t show_axis;

    /** Reverse the order of the data (right to left). */
    uint8_t reverse;

    /** Show data in hidden rows. */
    uint8_t show_hidden;

    /** How empty cells are handled. See #lxw_sparkline_empty_cells. */
    uint8_t empty_cells;

    /** Line weight in points (e.g., 0.75, 1.0, 2.25). Default is 0.75. */
    double weight;

    /** Range containing date axis data. */
    const char *date_axis;

} lxw_sparkline_options;

/* Internal sparkline structure with resolved values */
typedef struct lxw_sparkline {

    STAILQ_ENTRY (lxw_sparkline) list_pointers;

    char *range;
    lxw_row_t row;
    lxw_col_t col;
    char *location;

    uint8_t type;
    uint8_t style;

    uint8_t high_point;
    uint8_t low_point;
    uint8_t negative_points;
    uint8_t first_point;
    uint8_t last_point;
    uint8_t markers;

    lxw_color_t series_color;
    lxw_color_t negative_color;
    lxw_color_t markers_color;
    lxw_color_t first_color;
    lxw_color_t last_color;
    lxw_color_t high_color;
    lxw_color_t low_color;

    double min;
    double max;
    uint8_t min_axis_type;
    uint8_t max_axis_type;

    uint8_t show_axis;
    uint8_t reverse;
    uint8_t show_hidden;
    uint8_t empty_cells;

    double weight;
    char *date_axis;

} lxw_sparkline;

STAILQ_HEAD(lxw_sparklines, lxw_sparkline);

#endif /* __LXW_SPARKLINE_H__ */
