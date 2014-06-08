/*
 * libxlsxwriter
 * 
 * Copyright 2014, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 */

/**
 * @file utility.h
 *
 * @brief Utility functions for libxlsxwriter.
 *
 * <!-- Copyright 2014, John McNamara, jmcnamara@cpan.org -->
 *
 */

#ifndef __LXW_UTILITY_H__
#define __LXW_UTILITY_H__

#include <stdint.h>
#include "common.h"

#define MAX_COL_NAME_LENGTH   5
#define MAX_CELL_NAME_LENGTH  13
#define MAX_CELL_RANGE_LENGTH (MAX_CELL_NAME_LENGTH * 2 + 1)
#define EPOCH_1900            0
#define EPOCH_1904            1

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



 /* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

void _xl_col_to_name(char *col_name, int col_num, uint8_t absolute);

void xl_rowcol_to_cell(char *cell_name, int row, int col);

void xl_rowcol_to_cell_abs(char *cell_name,
                           int row,
                           int col, uint8_t abs_row, uint8_t abs_col);

void xl_range(char *range,
              int first_row, int first_col, int last_row, int last_col);

void xl_range_abs(char *range,
                  int first_row, int first_col, int last_row, int last_col);

double _datetime_to_excel_date(lxw_datetime *datetime, uint8_t date_1904);

/* Declarations required for unit testing. */
#ifdef TESTING

#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_UTILITY_H__ */
