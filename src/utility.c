/*****************************************************************************
 * utility - Utility functions for libxlsxwriter.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "xlsxwriter/utility.h"

/*
 * Convert Excel A-XFD style column name to zero based number.
 */
void
lxw_col_to_name(char col_name[], int col_num, uint8_t absolute)
{
    uint8_t pos = 0;
    uint8_t len;
    uint8_t i;

    /* Change from 0 index to 1 index. */
    col_num++;

    /* Convert the column number to a string in reverse order. */
    while (col_num) {

        /* Get the remainder in base 26. */
        int remainder = col_num % 26;

        if (remainder == 0)
            remainder = 26;

        /* Convert the remainder value to a character. */
        col_name[pos++] = 'A' + remainder - 1;
        col_name[pos] = '\0';

        /* Get the next order of magnitude. */
        col_num = (col_num - 1) / 26;
    }

    if (absolute) {
        col_name[pos] = '$';
        col_name[pos + 1] = '\0';
    }

    /* Reverse the column name string. */
    len = strlen(col_name);
    for (i = 0; i < (len / 2); i++) {
        char tmp = col_name[i];
        col_name[i] = col_name[len - i - 1];
        col_name[len - i - 1] = tmp;
    }
}

/*
 * Convert zero indexed row and column to an Excel style A1 cell reference.
 */
void
lxw_rowcol_to_cell(char *cell_name, int row, int col)
{
    uint8_t pos;

    /* Add the column to the cell. */
    lxw_col_to_name(cell_name, col, 0);

    /* Get the end of the cell. */
    pos = strlen(cell_name);

    /* Add the row to the cell. */
    sprintf(&cell_name[pos], "%d", ++row);
}

/*
 * Convert zero indexed row and column to an Excel style $A$1 cell with
 * an absolute reference.
 */
void
lxw_rowcol_to_cell_abs(char *cell_name,
                       int row, int col, uint8_t abs_row, uint8_t abs_col)
{
    uint8_t pos;

    /* Add the column to the cell. */
    lxw_col_to_name(cell_name, col, abs_col);

    /* Get the end of the cell. */
    pos = strlen(cell_name);

    if (abs_row)
        cell_name[pos++] = '$';

    /* Add the row to the cell. */
    sprintf(&cell_name[pos], "%d", ++row);
}

/*
 * Convert zero indexed row and column pair to an Excel style A1:C5
 * range reference.
 */
void
lxw_range(char *range,
          int first_row, int first_col, int last_row, int last_col)
{
    uint8_t pos;

    /* Add the first cell to the range. */
    lxw_rowcol_to_cell(range, first_row, first_col);

    /* If the start and end cells are the same just return a single cell. */
    if (first_row == last_row && first_col == last_col)
        return;

    /* Get the end of the cell. */
    pos = strlen(range);

    /* Add the range separator. */
    range[pos++] = ':';

    /* Add the first cell to the range. */
    lxw_rowcol_to_cell(&range[pos], last_row, last_col);
}

/*
 * Convert zero indexed row and column pair to an Excel style $A$1:$C$5
 * range reference with absolute values.
 */
void
lxw_range_abs(char *range,
              int first_row, int first_col, int last_row, int last_col)
{
    uint8_t pos;

    /* Add the first cell to the range. */
    lxw_rowcol_to_cell_abs(range, first_row, first_col, 1, 1);

    /* If the start and end cells are the same just return a single cell. */
    if (first_row == last_row && first_col == last_col)
        return;

    /* Get the end of the cell. */
    pos = strlen(range);

    /* Add the range separator. */
    range[pos++] = ':';

    /* Add the first cell to the range. */
    lxw_rowcol_to_cell_abs(&range[pos], last_row, last_col, 1, 1);
}

/*
 * Convert an Excel style A1 cell reference to a zero indexed row number.
 */
uint32_t
lxw_get_row(const char *row_str)
{
    int row_num = 0;
    const char *p = row_str;

    /* Skip the column letters of the A1 cell. */
    while (p && isalpha((unsigned char) *p))
        p++;

    /* Convert the row part of the A1 cell to a number. */
    if (p)
        row_num = atoi(p);

    return row_num - 1;
}

/*
 * Convert an Excel style A1 cell reference to a zero indexed column number.
 */
uint16_t
lxw_get_col(const char *col_str)
{
    int col_num = 0;
    const char *p = col_str;

    /* Convert the leading column letters of the A1 cell. */
    while (p && isupper((unsigned char) *p)) {
        col_num = (col_num * 26) + (*p - 'A' + 1);
        p++;
    }

    return col_num - 1;
}

/*
 * Convert the second row of an Excel range ref to a zero indexed number.
 */
uint32_t
lxw_get_row_2(const char *row_str)
{
    const char *p = row_str;

    /* Find the : separator in the range. */
    while (p && *p != ':')
        p++;

    if (p)
        return lxw_get_row(++p);
    else
        return -1;
}

/*
 * Convert the second column of an Excel range ref to a zero indexed number.
 */
uint16_t
lxw_get_col_2(const char *col_str)
{
    const char *p = col_str;

    /* Find the : separator in the range. */
    while (p && *p != ':')
        p++;

    if (p)
        return lxw_get_col(++p);
    else
        return -1;
}

/*
 * Convert a lxw_datetime struct to an Excel serial date.
 */
double
lxw_datetime_to_excel_date(lxw_datetime *datetime, uint8_t date_1904)
{
    int year = datetime->year;
    int month = datetime->month;
    int day = datetime->day;
    int hour = datetime->hour;
    int min = datetime->min;
    double sec = datetime->sec;
    double seconds;
    int epoch = date_1904 ? 1904 : 1900;
    int offset = date_1904 ? 4 : 0;
    int norm = 300;
    int range;
    /* Set month days and check for leap year. */
    int mdays[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int leap = 0;
    int days = 0;
    int i;

    /* For times without dates set the default date for the epoch. */
    if (!year) {
        if (!date_1904) {
            year = 1899;
            month = 12;
            day = 31;
        }
        else {
            year = 1904;
            month = 1;
            day = 1;
        }
    }

    /* Convert the Excel seconds to a fraction of the seconds in 24 hours. */
    seconds = (hour * 60 * 60 + min * 60 + sec) / (24 * 60 * 60.0);

    /* Special cases for Excel dates in the 1900 epoch. */
    if (!date_1904) {
        /* Excel 1900 epoch. */
        if (year == 1899 && month == 12 && day == 31)
            return seconds;

        /* Excel 1900 epoch. */
        if (year == 1900 && month == 1 && day == 0)
            return seconds;

        /* Excel false leapday */
        if (year == 1900 && month == 2 && day == 29)
            return 60 + seconds;
    }

    /* We calculate the date by calculating the number of days since the */
    /* epoch and adjust for the number of leap days. We calculate the */
    /* number of leap days by normalizing the year in relation to the */
    /* epoch. Thus the year 2000 becomes 100 for 4-year and 100-year */
    /* leapdays and 400 for 400-year leapdays. */
    range = year - epoch;

    if (year % 4 == 0 && (year % 100 > 0 || year % 400 == 0)) {
        leap = 1;
        mdays[2] = 29;
    }

    /*
     * Calculate the serial date by accumulating the number of days
     * since the epoch.
     */

    /* Add days for previous months. */
    for (i = 0; i < month; i++) {
        days += mdays[i];
    }
    /* Add days for current month. */
    days += day;
    /* Add days for all previous years.  */
    days += range * 365;
    /* Add 4 year leapdays. */
    days += (range) / 4;
    /* Remove 100 year leapdays. */
    days -= (range + offset) / 100;
    /* Add 400 year leapdays. */
    days += (range + offset + norm) / 400;
    /* Remove leap days already counted. */
    days -= leap;

    /* Adjust for Excel erroneously treating 1900 as a leap year. */
    if (!date_1904 && days > 59)
        days++;

    return days + seconds;
}

/* Simple strdup() implementation since it isn't ANSI C. */
char *
lxw_strdup(const char *str)
{
    size_t len;
    char *copy;

    if (!str)
        return NULL;

    len = strlen(str) + 1;
    copy = malloc(len);

    if (copy)
        memcpy(copy, str, len);

    return copy;
}

/* Simple tolower() for strings. */
void
lxw_str_tolower(char *str)
{
    int i;

    for (i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

/* Create a quoted version of the worksheet name, or return an unmodified
 * copy if it doesn't required quoting. */
char *
lxw_quote_sheetname(char *str)
{

    uint8_t needs_quoting = 0;
    size_t number_of_quotes = 2;
    size_t i, j;
    size_t len = strlen(str);

    /* Don't quote the sheetname if it is already quoted. */
    if (str[0] == '\'')
        return lxw_strdup(str);

    /* Check if the sheetname contains any characters that require it
     * to be quoted. Also check for single quotes within the string. */
    for (i = 0; i < len; i++) {
        if (!isalnum(str[i]) && str[i] != '_' && str[i] != '.')
            needs_quoting = 1;

        if (str[i] == '\'') {
            needs_quoting = 1;
            number_of_quotes++;
        }
    }

    if (!needs_quoting) {
        return lxw_strdup(str);
    }
    else {
        /* Add single quotes to the start and end of the string. */
        char *quoted_name = calloc(1, len + number_of_quotes + 1);
        RETURN_ON_MEM_ERROR(quoted_name, NULL);

        quoted_name[0] = '\'';

        for (i = 0, j = 1; i < len; i++, j++) {
            quoted_name[j] = str[i];

            /* Double quote inline single quotes. */
            if (str[i] == '\'') {
                quoted_name[++j] = '\'';
            }
        }
        quoted_name[j++] = '\'';
        quoted_name[j++] = '\0';

        return quoted_name;
    }
}

/*
 * Thin wrapper for tmpfile() so it can be over-ridden with a safer version if
 * required.
 */
FILE *
lxw_tmpfile(void)
{
    return tmpfile();
}
