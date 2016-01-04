/*
 * libxlsxwriter
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 */

/**
 * @file utility.h
 *
 * @brief Utility functions for libxlsxwriter.
 *
 * <!-- Copyright 2014-2016, John McNamara, jmcnamara@cpan.org -->
 *
 */

#ifndef __LXW_UTILITY_H__
#define __LXW_UTILITY_H__

#include <stdint.h>
#include "common.h"

/* Max col: $XFD\0 */
#define MAX_COL_NAME_LENGTH   5

/* Max cell: $XFWD$1048576\0 */
#define MAX_CELL_NAME_LENGTH  14

/* Max range: $XFWD$1048576:$XFWD$1048576\0 */
#define MAX_CELL_RANGE_LENGTH (MAX_CELL_NAME_LENGTH * 2)

#define EPOCH_1900            0
#define EPOCH_1904            1

/**
 * @brief Convert an Excel `A1` cell string into a `(row, col)` pair.
 *
 * Convert an Excel `A1` cell string into a `(row, col)` pair.
 *
 * This is a little syntactic shortcut to help with worksheet layout:
 *
 * @code
 *      worksheet_write_string(worksheet, CELL("A1"), "Foo", NULL);
 *
 *      //Same as:
 *      worksheet_write_string(worksheet, 0, 0,       "Foo", NULL);
 * @endcode
 *
 * @note
 *
 * This macro shouldn't be used in performance critical situations since it
 * expands to two function calls.
 */
#define CELL(cell) \
    lxw_get_row(cell), lxw_get_col(cell)

/**
 * @brief Convert an Excel `A:B` column range into a `(col1, col2)` pair.
 *
 * Convert an Excel `A:B` column range into a `(col1, col2)` pair.
 *
 * This is a little syntactic shortcut to help with worksheet layout:
 *
 * @code
 *     worksheet_set_column(worksheet, COLS("B:D"), 20, NULL, NULL);
 *
 *     // Same as:
 *     worksheet_set_column(worksheet, 1, 3,        20, NULL, NULL);
 * @endcode
 *
 */
#define COLS(cols) \
    lxw_get_col(cols), lxw_get_col_2(cols)

/**
 * @brief Convert an Excel `A1:B2` range into a `(first_row, first_col,
 *        last_row, last_col)` sequence.
 *
 * Convert an Excel `A1:B2` range into a `(first_row, first_col, last_row,
 * last_col)` sequence.
 *
 * This is a little syntactic shortcut to help with worksheet layout.
 *
 * @code
 *     worksheet_print_area(worksheet, 0, 0, 41, 10); // A1:K42.
 *
 *     // Same as:
 *     worksheet_print_area(worksheet, RANGE("A1:K42"));
 * @endcode
 */
#define RANGE(range) \
    lxw_get_row(range), lxw_get_col(range), lxw_get_row_2(range), lxw_get_col_2(range)

/** @brief Struct to represent a date and time in Excel.
 *
 * Struct to represent a date and time in Excel. See @ref working_with_dates.
 */
typedef struct lxw_datetime {

    /** Year     : 1900 - 9999 */
    int year;
    /** Month    : 1 - 12 */
    int month;
    /** Day      : 1 - 31 */
    int day;
    /** Hour     : 0 - 23 */
    int hour;
    /** Minute   : 0 - 59 */
    int min;
    /** Seconds  : 0 - 59.999 */
    double sec;

} lxw_datetime;

/* Create a quoted version of the worksheet name */
char *lxw_quote_sheetname(char *str);

 /* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

void lxw_col_to_name(char *col_name, int col_num, uint8_t absolute);

void lxw_rowcol_to_cell(char *cell_name, int row, int col);

void lxw_rowcol_to_cell_abs(char *cell_name,
                            int row,
                            int col, uint8_t abs_row, uint8_t abs_col);

void lxw_range(char *range,
               int first_row, int first_col, int last_row, int last_col);

void lxw_range_abs(char *range,
                   int first_row, int first_col, int last_row, int last_col);

uint32_t lxw_get_row(const char *row_str);
uint16_t lxw_get_col(const char *col_str);
uint32_t lxw_get_row_2(const char *row_str);
uint16_t lxw_get_col_2(const char *col_str);

double lxw_datetime_to_excel_date(lxw_datetime *datetime, uint8_t date_1904);

char *lxw_strdup(const char *str);

void lxw_str_tolower(char *str);

FILE *lxw_tmpfile(void);

/* Declarations required for unit testing. */
#ifdef TESTING

#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_UTILITY_H__ */
