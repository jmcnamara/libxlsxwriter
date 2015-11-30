/*
 * libxlsxwriter
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 */

/**
 * @page worksheet_page The Worksheet object
 *
 * The Worksheet object represents an Excel worksheet. It handles
 * operations such as writing data to cells or formatting worksheet
 * layout.
 *
 * See @ref worksheet.h for full details of the functionality.
 *
 * @file worksheet.h
 *
 * @brief Functions related to adding data and formatting to a worksheet.
 *
 * The Worksheet object represents an Excel worksheet. It handles
 * operations such as writing data to cells or formatting worksheet
 * layout.
 *
 * A Worksheet object isn't created directly. Instead a worksheet is
 * created by calling the workbook_add_worksheet() function from a
 * Workbook object:
 *
 * @code
 *     #include "xlsxwriter.h"
 *
 *     int main() {
 *
 *         lxw_workbook  *workbook  = new_workbook("filename.xlsx");
 *         lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
 *
 *         worksheet_write_string(worksheet, 0, 0, "Hello Excel", NULL);
 *
 *         return workbook_close(workbook);
 *     }
 * @endcode
 *
 */
#ifndef __LXW_WORKSHEET_H__
#define __LXW_WORKSHEET_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "shared_strings.h"
#include "common.h"
#include "format.h"
#include "utility.h"

#define LXW_ROW_MAX 1048576
#define LXW_COL_MAX 16384
#define LXW_COL_META_MAX 128
#define LXW_HEADER_FOOTER_MAX 255
#define LXW_MAX_NUMBER_URLS 65530

/* The Excel 2007 specification says that the maximum number of page
 * breaks is 1026. However, in practice it is actually 1023. */
#define LXW_BREAKS_MAX 1023

/** Default column width in Excel */
#define LXW_DEF_COL_WIDTH 8.43

/** Default row height in Excel */
#define LXW_DEF_ROW_HEIGHT 15

/** Error codes from worksheet functions. */
enum lxw_worksheet_error {
    /** No error. */
    LXW_ERROR_WORKSHEET_NONE = 0,

    /** Row or column index out of range. */
    LXW_ERROR_WORKSHEET_INDEX_OUT_OF_RANGE,

    /** String exceeds Excel's LXW_STR_MAX limit. */
    LXW_ERROR_WORKSHEET_MAX_STRING_LENGTH_EXCEEDED,

    /** NULL String ignored as function parameter. */
    LXW_ERROR_WORKSHEET_NULL_STRING_IGNORED,

    /** Maximum number of worksheet URLs (65530) exceeded. */
    LXW_ERROR_WORKSHEET_MAX_NUMBER_URLS_EXCEEDED,

    /** Error finding string index. */
    LXW_ERROR_WORKSHEET_STRING_HASH_NOT_FOUND,

    /** Memory error. */
    LXW_ERROR_WORKSHEET_MEMORY_ERROR
};

/** Gridline options using in `worksheet_gridlines()`. */
enum lxw_gridlines {
    /** Hide screen and print gridlines. */
    LXW_HIDE_ALL_GRIDLINES = 0,
    /** Show screen gridlines. */
    LXW_SHOW_SCREEN_GRIDLINES,
    /** Show print gridlines. */
    LXW_SHOW_PRINT_GRIDLINES,
    /** Show screen and print gridlines. */
    LXW_SHOW_ALL_GRIDLINES
};

/** Data type to represent a row value.
 *
 * The maximum row in Excel is 1,048,576.
 */
typedef uint32_t lxw_row_t;

/** Data type to represent a column value.
 *
 * The maximum column in Excel is 16,384.
 */
typedef uint16_t lxw_col_t;

enum cell_types {
    NUMBER_CELL = 1,
    STRING_CELL,
    INLINE_STRING_CELL,
    FORMULA_CELL,
    ARRAY_FORMULA_CELL,
    BLANK_CELL,
    HYPERLINK_URL,
    HYPERLINK_INTERNAL,
    HYPERLINK_EXTERNAL
};

/* Define the queue.h TAILQ structs for the list head types. */
TAILQ_HEAD(lxw_table_cells, lxw_cell);
RB_HEAD(lxw_table_rows, lxw_row);
STAILQ_HEAD(lxw_merged_ranges, lxw_merged_range);

/**
 * @brief Options for rows and columns.
 *
 * Options struct for the worksheet_set_column() and worksheet_set_row()
 * functions.
 *
 * It has the following members but currently only the `hidden` property is
 * supported:
 *
 * * `hidden`
 * * `level`
 * * `collapsed`
 */
typedef struct lxw_row_col_options {
    /** Hide the row/column */
    uint8_t hidden;
    uint8_t level;
    uint8_t collapsed;
} lxw_row_col_options;

typedef struct lxw_col_options {
    lxw_col_t firstcol;
    lxw_col_t lastcol;
    double width;
    lxw_format *format;
    uint8_t hidden;
    uint8_t level;
    uint8_t collapsed;
} lxw_col_options;

typedef struct lxw_merged_range {
    lxw_row_t first_row;
    lxw_row_t last_row;
    lxw_col_t first_col;
    lxw_col_t last_col;

    STAILQ_ENTRY (lxw_merged_range) list_pointers;
} lxw_merged_range;

typedef struct lxw_repeat_rows {
    uint8_t in_use;
    lxw_row_t first_row;
    lxw_row_t last_row;
} lxw_repeat_rows;

typedef struct lxw_repeat_cols {
    uint8_t in_use;
    lxw_col_t first_col;
    lxw_col_t last_col;
} lxw_repeat_cols;

typedef struct lxw_print_area {
    uint8_t in_use;
    lxw_row_t first_row;
    lxw_row_t last_row;
    lxw_col_t first_col;
    lxw_col_t last_col;
} lxw_print_area;

typedef struct lxw_autofilter {
    uint8_t in_use;
    lxw_row_t first_row;
    lxw_row_t last_row;
    lxw_col_t first_col;
    lxw_col_t last_col;
} lxw_autofilter;

/**
 * @brief Header and footer options.
 *
 * Optional parameters used in the worksheet_set_header_opt() and
 * worksheet_set_footer_opt() functions.
 *
 */
typedef struct lxw_header_footer_options {
    /** Header or footer margin in inches. Excel default is 0.3. */
    double margin;
} lxw_header_footer_options;

/**
 * @brief Struct to represent an Excel worksheet.
 *
 * The members of the lxw_worksheet struct aren't modified directly. Instead
 * the worksheet properties are set by calling the functions shown in
 * worksheet.h.
 */
typedef struct lxw_worksheet {

    FILE *file;
    FILE *optimize_tmpfile;
    struct lxw_table_rows *table;
    struct lxw_table_rows *hyperlinks;
    struct lxw_cell **array;
    struct lxw_merged_ranges *merged_ranges;

    lxw_row_t dim_rowmin;
    lxw_row_t dim_rowmax;
    lxw_col_t dim_colmin;
    lxw_col_t dim_colmax;

    lxw_sst *sst;
    char *name;
    char *quoted_name;

    uint32_t index;
    uint8_t active;
    uint8_t selected;
    uint8_t hidden;
    uint32_t *active_sheet;

    lxw_col_options **col_options;
    uint16_t col_options_max;

    double *col_sizes;
    uint16_t col_sizes_max;

    lxw_format **col_formats;
    uint16_t col_formats_max;

    uint8_t col_size_changed;
    uint8_t optimize;
    struct lxw_row *optimize_row;

    uint16_t fit_height;
    uint16_t fit_width;
    uint16_t horizontal_dpi;
    uint16_t hlink_count;
    uint16_t page_start;
    uint16_t print_scale;
    uint16_t rel_count;
    uint16_t vertical_dpi;
    uint8_t filter_on;
    uint8_t fit_page;
    uint8_t hcenter;
    uint8_t orientation;
    uint8_t outline_changed;
    uint8_t page_order;
    uint8_t page_setup_changed;
    uint8_t page_view;
    uint8_t paper_size;
    uint8_t print_gridlines;
    uint8_t print_headers;
    uint8_t print_options_changed;
    uint8_t screen_gridlines;
    uint8_t tab_color;
    uint8_t vba_codename;
    uint8_t vcenter;

    double margin_left;
    double margin_right;
    double margin_top;
    double margin_bottom;
    double margin_header;
    double margin_footer;

    uint8_t header_footer_changed;
    char header[LXW_HEADER_FOOTER_MAX];
    char footer[LXW_HEADER_FOOTER_MAX];

    struct lxw_repeat_rows repeat_rows;
    struct lxw_repeat_cols repeat_cols;
    struct lxw_print_area print_area;
    struct lxw_autofilter autofilter;

    uint16_t merged_range_count;

    lxw_row_t *hbreaks;
    lxw_col_t *vbreaks;

    struct lxw_rel_tuples *external_hyperlinks;

    STAILQ_ENTRY (lxw_worksheet) list_pointers;

} lxw_worksheet;

/*
 * Worksheet initialisation data.
 */
typedef struct lxw_worksheet_init_data {
    uint32_t index;
    uint8_t hidden;
    uint8_t optimize;
    uint32_t *active_sheet;
    lxw_sst *sst;
    char *name;
    char *quoted_name;

} lxw_worksheet_init_data;

/* Struct to represent a worksheet row. */
typedef struct lxw_row {
    lxw_row_t row_num;
    double height;
    lxw_format *format;
    uint8_t hidden;
    uint8_t level;
    uint8_t collapsed;
    uint8_t row_changed;
    uint8_t data_changed;
    struct lxw_table_cells *cells;

    /* tree management pointers for tree.h. */
    RB_ENTRY (lxw_row) tree_pointers;
} lxw_row;

/* Struct to represent a worksheet cell. */
typedef struct lxw_cell {
    lxw_row_t row_num;
    lxw_col_t col_num;
    enum cell_types type;
    lxw_format *format;

    union {
        double number;
        int32_t string_id;
        char *string;
    } u;

    double formula_result;
    char *user_data1;
    char *user_data2;

    /* List pointers for queue.h. */
    TAILQ_ENTRY (lxw_cell) list_pointers;
} lxw_cell;

/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

/**
 * @brief Write a number to a worksheet cell.
 *
 * @param worksheet Pointer to the lxw_worksheet instance to be updated.
 * @param row       The zero indexed row number.
 * @param col       The zero indexed column number.
 * @param number    The number to write to the cell.
 * @param format    A pointer to a Format instance or NULL.
 *
 * @return A #lxw_worksheet_error code.
 *
 * The `worksheet_write_number()` function writes numeric types to the cell
 * specified by `row` and `column`:
 *
 * @code
 *     worksheet_write_number(worksheet, 0, 0, 123456, NULL);
 *     worksheet_write_number(worksheet, 1, 0, 2.3451, NULL);
 * @endcode
 *
 * @image html write_number01.png
 *
 * The native data type for all numbers in Excel is a IEEE-754 64-bit
 * double-precision floating point, which is also the default type used by
 * `%worksheet_write_number`.
 *
 * The `format` parameter is used to apply formatting to the cell. This
 * parameter can be `NULL` to indicate no formatting or it can be a
 * @ref format.h "Format" object.
 *
 * @code
 *     lxw_format *format = workbook_add_format(workbook);
 *     format_set_num_format(format, "$#,##0.00");
 *
 *     worksheet_write_number(worksheet, 0, 0, 1234.567, format);
 * @endcode
 *
 * @image html write_number02.png
 *
 */
int8_t worksheet_write_number(lxw_worksheet *worksheet,
                              lxw_row_t row,
                              lxw_col_t col, double number,
                              lxw_format *format);
/**
 * @brief Write a string to a worksheet cell.
 *
 * @param worksheet Pointer to the lxw_worksheet instance to be updated.
 * @param row       The zero indexed row number.
 * @param col       The zero indexed column number.
 * @param string    String to write to cell.
 * @param format    A pointer to a Format instance or NULL.
 *
 * @return A #lxw_worksheet_error code.
 *
 * The `%worksheet_write_string()` function writes a string to the cell
 * specified by `row` and `column`:
 *
 * @code
 *     worksheet_write_string(worksheet, 0, 0, "This phrase is English!", NULL);
 * @endcode
 *
 * @image html write_string01.png
 *
 * The `format` parameter is used to apply formatting to the cell. This
 * parameter can be `NULL` to indicate no formatting or it can be a
 * @ref format.h "Format" object:
 *
 * @code
 *     lxw_format *format = workbook_add_format(workbook);
 *     format_set_bold(format);
 *
 *     worksheet_write_string(worksheet, 0, 0, "This phrase is Bold!", format);
 * @endcode
 *
 * @image html write_string02.png
 *
 * Unicode strings are supported in UTF-8 encoding. This generally requires
 * that your source file is UTF-8 encoded or that the data has been read from
 * a UTF-8 source:
 *
 * @code
 *    worksheet_write_string(worksheet, 0, 0, "Это фраза на русском!", NULL);
 * @endcode
 *
 * @image html write_string03.png
 *
 */
int8_t worksheet_write_string(lxw_worksheet *worksheet,
                              lxw_row_t row,
                              lxw_col_t col, const char *string,
                              lxw_format *format);
/**
 * @brief Write a formula to a worksheet cell.
 *
 * @param worksheet Pointer to the lxw_worksheet instance to be updated.
 * @param row       The zero indexed row number.
 * @param col       The zero indexed column number.
 * @param formula   Formula string to write to cell.
 * @param format    A pointer to a Format instance or NULL.
 *
 * @return A #lxw_worksheet_error code.
 *
 * The `%worksheet_write_formula()` function writes a formula or function to
 * the cell specified by `row` and `column`:
 *
 * @code
 *  worksheet_write_formula(worksheet, 0, 0, "=B3 + 6",                    NULL);
 *  worksheet_write_formula(worksheet, 1, 0, "=SIN(PI()/4)",               NULL);
 *  worksheet_write_formula(worksheet, 2, 0, "=SUM(A1:A2)",                NULL);
 *  worksheet_write_formula(worksheet, 3, 0, "=IF(A3>1,\"Yes\", \"No\")",  NULL);
 *  worksheet_write_formula(worksheet, 4, 0, "=AVERAGE(1, 2, 3, 4)",       NULL);
 *  worksheet_write_formula(worksheet, 5, 0, "=DATEVALUE(\"1-Jan-2013\")", NULL);
 * @endcode
 *
 * @image html write_formula01.png
 *
 * The `format` parameter is used to apply formatting to the cell. This
 * parameter can be `NULL` to indicate no formatting or it can be a
 * @ref format.h "Format" object.
 *
 * Libxlsxwriter doesn't calculate the value of a formula and instead stores a
 * default value of `0`. The correct formula result is displayed in Excel, as
 * shown in the example above, since it recalculates the formulas when it loads
 * the file. For cases where this is an issue see the
 * `worksheet_write_formula_num()` function and the discussion in that section.
 *
 * Formulas must be written with the US style separator/range operator which
 * is a comma (not semi-colon). Therefore a formula with multiple values
 * should be written as follows:
 *
 * @code
 *     // OK.
 *     worksheet_write_formula(worksheet, 0, 0, "=SUM(1, 2, 3)", NULL);
 *
 *     // NO. Error on load.
 *     worksheet_write_formula(worksheet, 1, 0, "=SUM(1; 2; 3)", NULL);
 * @endcode
 *
 */
int8_t worksheet_write_formula(lxw_worksheet *worksheet,
                               lxw_row_t row,
                               lxw_col_t col, const char *formula,
                               lxw_format *format);
/**
 * @brief Write an array formula to a worksheet cell.
 *
 * @param worksheet
 * @param first_row   The first row of the range. (All zero indexed.)
 * @param first_col   The first column of the range.
 * @param last_row    The last row of the range.
 * @param last_col    The last col of the range.
 * @param formula     Array formula to write to cell.
 * @param format      A pointer to a Format instance or NULL.
 *
 * @return A #lxw_worksheet_error code.
 *
  * The `%worksheet_write_array_formula()` function writes an array formula to
 * a cell range. In Excel an array formula is a formula that performs a
 * calculation on a set of values.
 *
 * In Excel an array formula is indicated by a pair of braces around the
 * formula: `{=SUM(A1:B1*A2:B2)}`.
 *
 * Array formulas can return a single value or a range or values. For array
 * formulas that return a range of values you must specify the range that the
 * return values will be written to. This is why this function has `first_`
 * and `last_` row/column parameters. The RANGE() macro can also be used to
 * specify the range:
 *
 * @code
 *     worksheet_write_array_formula(worksheet, 4, 0, 6, 0,     "{=TREND(C5:C7,B5:B7)}", NULL);
 *
 *     // Same as above using the RANGE() macro.
 *     worksheet_write_array_formula(worksheet, RANGE("A5:A7"), "{=TREND(C5:C7,B5:B7)}", NULL);
 * @endcode
 *
 * If the array formula returns a single value then the `first_` and `last_`
 * parameters should be the same:
 *
 * @code
 *     worksheet_write_array_formula(worksheet, 1, 0, 1, 0,     "{=SUM(B1:C1*B2:C2)}", NULL);
 *     worksheet_write_array_formula(worksheet, RANGE("A2:A2"), "{=SUM(B1:C1*B2:C2)}", NULL);
 * @endcode
 *
 */
int8_t worksheet_write_array_formula(lxw_worksheet *worksheet,
                                     lxw_row_t first_row,
                                     lxw_col_t first_col,
                                     lxw_row_t last_row,
                                     lxw_col_t last_col,
                                     const char *formula, lxw_format *format);

int8_t worksheet_write_array_formula_num(lxw_worksheet *worksheet,
                                         lxw_row_t first_row,
                                         lxw_col_t first_col,
                                         lxw_row_t last_row,
                                         lxw_col_t last_col,
                                         const char *formula,
                                         lxw_format *format, double result);

/**
 * @brief Write a date or time to a worksheet cell.
 *
 * @param worksheet Pointer to the lxw_worksheet instance to be updated.
 * @param row       The zero indexed row number.
 * @param col       The zero indexed column number.
 * @param datetime  The datetime to write to the cell.
 * @param format    A pointer to a Format instance or NULL.
 *
 * @return A #lxw_worksheet_error code.
 *
 * The `worksheet_write_datetime()` function can be used to write a date or
 * time to the cell specified by `row` and `column`:
 *
 * @dontinclude dates_and_times02.c
 * @skip include
 * @until num_format
 * @skip Feb
 * @until }
 *
 * The `format` parameter should be used to apply formatting to the cell using
 * a @ref format.h "Format" object as shown above. Without a date format the
 * datetime will appear as a number only.
 *
 * See @ref working_with_dates for more information about handling dates and
 * times in libxlsxwriter.
 */
int8_t worksheet_write_datetime(lxw_worksheet *worksheet,
                                lxw_row_t row,
                                lxw_col_t col, lxw_datetime *datetime,
                                lxw_format *format);

int8_t worksheet_write_url_opt(lxw_worksheet *worksheet,
                               lxw_row_t row_num,
                               lxw_col_t col_num, const char *url,
                               lxw_format *format, const char *string,
                               const char *tooltip);
/**
 *
 * @param worksheet Pointer to the lxw_worksheet instance to be updated.
 * @param row       The zero indexed row number.
 * @param col       The zero indexed column number.
 * @param url       The url to write to the cell.
 * @param format    A pointer to a Format instance or NULL.
 *
 * @return A #lxw_worksheet_error code.
 *
 *
 * The `%worksheet_write_url()` function is used to write a URL/hyperlink to a
 * worksheet cell specified by `row` and `column`.
 *
 * @code
 *     worksheet_write_url(worksheet, 0, 0, "http://libxlsxwriter.github.io", url_format);
 * @endcode
 *
 * @image html hyperlinks_short.png
 *
 * The `format` parameter is used to apply formatting to the cell. This
 * parameter can be `NULL` to indicate no formatting or it can be a @ref
 * format.h "Format" object. The typical worksheet format for a hyperlink is a
 * blue underline:
 *
 * @code
 *    lxw_format *url_format   = workbook_add_format(workbook);
 *
 *    format_set_underline (url_format, LXW_UNDERLINE_SINGLE);
 *    format_set_font_color(url_format, LXW_COLOR_BLUE);
 *
 * @endcode
 *
 * The usual web style URI's are supported: `%http://`, `%https://`, `%ftp://`
 * and `mailto:` :
 *
 * @code
 *     worksheet_write_url(worksheet, 0, 0, "ftp://www.python.org/",    url_format);
 *     worksheet_write_url(worksheet, 1, 0, "http://www.python.org/",   url_format);
 *     worksheet_write_url(worksheet, 2, 0, "https://www.python.org/",  url_format);
 *     worksheet_write_url(worksheet, 3, 0, "mailto:jmcnamaracpan.org", url_format);
 *
 * @endcode
 *
 * An Excel hyperlink is comprised of two elements: the displayed string and
 * the non-displayed link. By default the displayed string is the same as the
 * link. However, it is possible to overwrite it with any other
 * `libxlsxwriter` type using the appropriate `worksheet_write_*()`
 * function. The most common case is to overwrite the displayed link text with
 * another string:
 *
 * @code
 *  // Write a hyperlink but overwrite the displayed string.
 *  worksheet_write_url   (worksheet, 2, 0, "http://libxlsxwriter.github.io", url_format);
 *  worksheet_write_string(worksheet, 2, 0, "Read the documentation.",        url_format);
 *
 * @endcode
 *
 * @image html hyperlinks_short2.png
 *
 * Two local URIs are supported: `internal:` and `external:`. These are used
 * for hyperlinks to internal worksheet references or external workbook and
 * worksheet references:
 *
 * @code
 *     worksheet_write_url(worksheet, 0, 0, "internal:Sheet2!A1",                url_format);
 *     worksheet_write_url(worksheet, 1, 0, "internal:Sheet2!B2",                url_format);
 *     worksheet_write_url(worksheet, 2, 0, "internal:Sheet2!A1:B2",             url_format);
 *     worksheet_write_url(worksheet, 3, 0, "internal:'Sales Data'!A1",          url_format);
 *     worksheet_write_url(worksheet, 4, 0, "external:c:\\temp\\foo.xlsx",       url_format);
 *     worksheet_write_url(worksheet, 5, 0, "external:c:\\foo.xlsx#Sheet2!A1",   url_format);
 *     worksheet_write_url(worksheet, 6, 0, "external:..\\foo.xlsx",             url_format);
 *     worksheet_write_url(worksheet, 7, 0, "external:..\\foo.xlsx#Sheet2!A1",   url_format);
 *     worksheet_write_url(worksheet, 8, 0, "external:\\\\NET\\share\\foo.xlsx", url_format);
 *
 * @endcode
 *
 * Worksheet references are typically of the form `Sheet1!A1`. You can also
 * link to a worksheet range using the standard Excel notation:
 * `Sheet1!A1:B2`.
 *
 * In external links the workbook and worksheet name must be separated by the
 * `#` character:
 *
 * @code
 *     worksheet_write_url(worksheet, 0, 0, "external:c:\\foo.xlsx#Sheet2!A1",   url_format);
 * @endcode
 *
 * You can also link to a named range in the target worksheet: For example say
 * you have a named range called `my_name` in the workbook `c:\temp\foo.xlsx`
 * you could link to it as follows:
 *
 * @code
 *     worksheet_write_url(worksheet, 0, 0, "external:c:\\temp\\foo.xlsx#my_name", url_format);
 *
 * @endcode
 *
 * Excel requires that worksheet names containing spaces or non alphanumeric
 * characters are single quoted as follows:
 *
 * @code
 *     worksheet_write_url(worksheet, 0, 0, "internal:'Sales Data'!A1", url_format);
 * @endcode
 *
 * Links to network files are also supported. Network files normally begin
 * with two back slashes as follows `\\NETWORK\etc`. In order to represent
 * this in a C string literal the backslashes should be escaped:
 * @code
 *     worksheet_write_url(worksheet, 0, 0, "external:\\\\NET\\share\\foo.xlsx", url_format);
 * @endcode
 *
 *
 * Alternatively, you can use Windows style forward slashes. These are
 * translated internally to backslashes:
 *
 * @code
 *     worksheet_write_url(worksheet, 0, 0, "external:c:/temp/foo.xlsx",     url_format);
 *     worksheet_write_url(worksheet, 1, 0, "external://NET/share/foo.xlsx", url_format);
 *
 * @endcode
 *
 *
 * **Note:**
 *
 *    libxlsxwriter will escape the following characters in URLs as required
 *    by Excel: `\s " < > \ [ ]  ^ { }` unless the URL already contains `%%xx`
 *    style escapes. In which case it is assumed that the URL was escaped
 *    correctly by the user and will by passed directly to Excel.
 *
 */
int8_t worksheet_write_url(lxw_worksheet *worksheet,
                           lxw_row_t row,
                           lxw_col_t col, const char *url,
                           lxw_format *format);

/**
 * @brief Write a formatted blank worksheet cell.
 *
 * @param worksheet Pointer to the lxw_worksheet instance to be updated.
 * @param row       The zero indexed row number.
 * @param col       The zero indexed column number.
 * @param format    A pointer to a Format instance or NULL.
 *
 * @return A #lxw_worksheet_error code.
 *
 * Write a blank cell specified by `row` and `column`:
 *
 * @code
 *     worksheet_write_blank(worksheet, 1, 1, border_format);
 * @endcode
 *
 * This function is used to add formatting to a cell which doesn't contain a
 * string or number value.
 *
 * Excel differentiates between an "Empty" cell and a "Blank" cell. An Empty
 * cell is a cell which doesn't contain data or formatting whilst a Blank cell
 * doesn't contain data but does contain formatting. Excel stores Blank cells
 * but ignores Empty cells.
 *
 * As such, if you write an empty cell without formatting it is ignored.
 *
 */
int8_t worksheet_write_blank(lxw_worksheet *worksheet,
                             lxw_row_t row, lxw_col_t col,
                             lxw_format *format);

/**
 * @brief Write a formula to a worksheet cell with a user defined result.
 *
 * @param worksheet Pointer to the lxw_worksheet instance to be updated.
 * @param row       The zero indexed row number.
 * @param col       The zero indexed column number.
 * @param formula   Formula string to write to cell.
 * @param format    A pointer to a Format instance or NULL.
 * @param result    A user defined result for a formula.
 *
 * @return A #lxw_worksheet_error code.
 *
 * The `%worksheet_write_formula_num()` function writes a formula or Excel
 * function to the cell specified by `row` and `column` with a user defined
 * result:
 *
 * @code
 *     // Required as a workaround only.
 *     worksheet_write_formula_num(worksheet, 0, 0, "=1 + 2", NULL, 3);
 * @endcode
 *
 * Libxlsxwriter doesn't calculate the value of a formula and instead stores
 * the value `0` as the formula result. It then sets a global flag in the XLSX
 * file to say that all formulas and functions should be recalculated when the
 * file is opened.
 *
 * This is the method recommended in the Excel documentation and in general it
 * works fine with spreadsheet applications.
 *
 * However, applications that don't have a facility to calculate formulas,
 * such as Excel Viewer, or some mobile applications will only display the `0`
 * results.
 *
 * If required, the `%worksheet_write_formula_num()` function can be used to
 * specify a formula and its result.
 *
 * This function is rarely required and is only provided for compatibility
 * with some third party applications. For most applications the
 * worksheet_write_formula() function is the recommended way of writing
 * formulas.
 *
 */
int8_t worksheet_write_formula_num(lxw_worksheet *worksheet,
                                   lxw_row_t row,
                                   lxw_col_t col,
                                   const char *formula,
                                   lxw_format *format, double result);

/**
 * @brief Set the properties for a row of cells.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param row       The zero indexed row number.
 * @param height    The row height.
 * @param format    A pointer to a Format instance or NULL.
 * @param options   Optional row parameters: hidden, level, collapsed.
 *
 * The `%worksheet_set_row()` function is used to change the default
 * properties of a row. The most common use for this function is to change the
 * height of a row:
 *
 * @code
 *     // Set the height of Row 1 to 20.
 *     worksheet_set_row(worksheet, 0, 20, NULL, NULL);
 * @endcode
 *
 * The other common use for `%worksheet_set_row()` is to set the a @ref
 * format.h "Format" for all cells in the row:
 *
 * @code
 *     lxw_format *bold = workbook_add_format(workbook);
 *     format_set_bold(bold);
 *
 *     // Set the header row to bold.
 *     worksheet_set_row(worksheet, 0, 15, bold, NULL);
 * @endcode
 *
 * If you wish to set the format of a row without changing the height you can
 * pass the default row height of #LXW_DEF_ROW_HEIGHT = 15:
 *
 * @code
 *     worksheet_set_row(worksheet, 0, LXW_DEF_ROW_HEIGHT, format, NULL);
 *     worksheet_set_row(worksheet, 0, 15, format, NULL); // Same as above.
 * @endcode
 *
 * The `format` parameter will be applied to any cells in the row that don't
 * have a format. As with Excel the row format is overridden by an explicit
 * cell format. For example:
 *
 * @code
 *     // Row 1 has format1.
 *     worksheet_set_row(worksheet, 0, 15, format1, NULL);
 *
 *     // Cell A1 in Row 1 defaults to format1.
 *     worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
 *
 *     // Cell B1 in Row 1 keeps format2.
 *     worksheet_write_string(worksheet, 0, 1, "Hello", format2);
 * @endcode
 *
 * The `options` parameter is a #lxw_row_col_options struct. It has the
 * following members but currently only the `hidden` property is supported:
 *
 * - `hidden`
 * - `level`
 * - `collapsed`
 *
 * The `"hidden"` option is used to hide a row. This can be used, for
 * example, to hide intermediary steps in a complicated calculation:
 *
 * @code
 *     lxw_row_col_options options = {.hidden = 1, .level = 0, .collapsed = 0};
 *
 *     // Hide the fourth row.
 *     worksheet_set_row(worksheet, 3, 20, NULL, &options);
 * @endcode
 *
 */
int8_t worksheet_set_row(lxw_worksheet *worksheet,
                         lxw_row_t row,
                         double height,
                         lxw_format *format, lxw_row_col_options *options);

/**
 * @brief Set the properties for one or more columns of cells.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param first_col The zero indexed first column.
 * @param last_col  The zero indexed last column.
 * @param width     The width of the column(s).
 * @param format    A pointer to a Format instance or NULL.
 * @param options   Optional row parameters: hidden, level, collapsed.
 *
 * The `%worksheet_set_column()` function can be used to change the default
 * properties of a single column or a range of columns:
 *
 * @code
 *     // Width of columns B:D set to 30.
 *     worksheet_set_column(worksheet, 1, 3, 30, NULL, NULL);
 *
 * @endcode
 *
 * If `%worksheet_set_column()` is applied to a single column the value of
 * `first_col` and `last_col` should be the same:
 *
 * @code
 *     // Width of column B set to 30.
 *     worksheet_set_column(worksheet, 1, 1, 30, NULL, NULL);
 *
 * @endcode
 *
 * It is also possible, and generally clearer, to specify a column range using
 * the form of `COLS()` macro:
 *
 * @code
 *     worksheet_set_column(worksheet, 4, 4, 20, NULL, NULL);
 *     worksheet_set_column(worksheet, 5, 8, 30, NULL, NULL);
 *
 *     // Same as the examples above but clearer.
 *     worksheet_set_column(worksheet, COLS("E:E"), 20, NULL, NULL);
 *     worksheet_set_column(worksheet, COLS("F:H"), 30, NULL, NULL);
 *
 * @endcode
 *
 * The width corresponds to the column width value that is specified in
 * Excel. It is approximately equal to the length of a string in the default
 * font of Calibri 11. Unfortunately, there is no way to specify "AutoFit" for
 * a column in the Excel file format. This feature is only available at
 * runtime from within Excel. It is possible to simulate "AutoFit" by tracking
 * the width of the data in the column as your write it.
 *
 * As usual the @ref format.h `format` parameter is optional. If you wish to
 * set the format without changing the width you can pass default col width of
 * #LXW_DEF_COL_WIDTH = 8.43:
 *
 * @code
 *     lxw_format *bold = workbook_add_format(workbook);
 *     format_set_bold(bold);
 *
 *     // Set the first column to bold.
 *     worksheet_set_column(worksheet, 0, 0, LXW_DEF_COL_HEIGHT, bold, NULL);
 * @endcode
 *
 * The `format` parameter will be applied to any cells in the column that
 * don't have a format. For example:
 *
 * @code
 *     // Column 1 has format1.
 *     worksheet_set_column(worksheet, COLS("A:A"), 8.43, format1, NULL);
 *
 *     // Cell A1 in column 1 defaults to format1.
 *     worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
 *
 *     // Cell A2 in column 1 keeps format2.
 *     worksheet_write_string(worksheet, 1, 0, "Hello", format2);
 * @endcode
 *
 * As in Excel a row format takes precedence over a default column format:
 *
 * @code
 *     // Row 1 has format1.
 *     worksheet_set_row(worksheet, 0, 15, format1, NULL);
 *
 *     // Col 1 has format2.
 *     worksheet_set_column(worksheet, COLS("A:A"), 8.43, format2, NULL);
 *
 *     // Cell A1 defaults to format1, the row format.
 *     worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
 *
 *    // Cell A2 keeps format2, the column format.
 *     worksheet_write_string(worksheet, 1, 0, "Hello", NULL);
 * @endcode
 *
 * The `options` parameter is a #lxw_row_col_options struct. It has the
 * following members but currently only the `hidden` property is supported:
 *
 * - `hidden`
 * - `level`
 * - `collapsed`
 *
 * The `"hidden"` option is used to hide a column. This can be used, for
 * example, to hide intermediary steps in a complicated calculation:
 *
 * @code
 *     lxw_row_col_options options = {.hidden = 1, .level = 0, .collapsed = 0};
 *
 *     worksheet_set_column(worksheet, COLS("A:A"), 8.43, NULL, &options);
 * @endcode
 *
 */
int8_t worksheet_set_column(lxw_worksheet *worksheet, lxw_col_t first_col,
                            lxw_col_t last_col, double width,
                            lxw_format *format, lxw_row_col_options *options);

/**
 * @brief Merge a range of cells.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param first_row The first row of the range. (All zero indexed.)
 * @param first_col The first column of the range.
 * @param last_row  The last row of the range.
 * @param last_col  The last col of the range.
 * @param string    String to write to the merged range.
 * @param format    A pointer to a Format instance or NULL.
 *
 * @return 0 for success, non-zero on error.
 *
 * The `%worksheet_merge_range()` function allows cells to be merged together
 * so that they act as a single area.
 *
 * Excel generally merges and centers cells at same time. To get similar
 * behaviour with libxlsxwriter you need to apply a @ref format.h "Format"
 * object with the appropriate alignment:
 *
 * @code
 *     lxw_format *merge_format = workbook_add_format(workbook);
 *     format_set_align(merge_format, LXW_ALIGN_CENTER);
 *
 *     worksheet_merge_range(worksheet, 1, 1, 1, 3, "Merged Range", merge_format);
 *
 * @endcode
 *
 * It is possible to apply other formatting to the merged cells as well:
 *
 * @code
 *    format_set_align   (merge_format, LXW_ALIGN_CENTER);
 *    format_set_align   (merge_format, LXW_ALIGN_VERTICAL_CENTER);
 *    format_set_border  (merge_format, LXW_BORDER_DOUBLE);
 *    format_set_bold    (merge_format);
 *    format_set_bg_color(merge_format, 0xD7E4BC);
 *
 *    worksheet_merge_range(worksheet, 2, 1, 3, 3, "Merged Range", merge_format);
 *
 * @endcode
 *
 * @image html merge.png
 *
 * The `%worksheet_merge_range()` function writes a `char*` string using
 * `worksheet_write_string()`. In order to write other data types, such as a
 * number or a formula, you can overwrite the first cell with a call to one of
 * the other write functions. The same Format should be used as was used in
 * the merged range.
 *
 * @code
 *    // First write a range with a blank string.
 *    worksheet_merge_range (worksheet, 1, 1, 1, 3, "", format);
 *
 *    // Then overwrite the first cell with a number.
 *    worksheet_write_number(worksheet, 1, 1, 123, format);
 * @endcode
 */
uint8_t worksheet_merge_range(lxw_worksheet *worksheet, lxw_row_t first_row,
                              lxw_col_t first_col, lxw_row_t last_row,
                              lxw_col_t last_col, const char *string,
                              lxw_format *format);

/**
 * @brief Set the autofilter area in the worksheet.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param first_row The first row of the range. (All zero indexed.)
 * @param first_col The first column of the range.
 * @param last_row  The last row of the range.
 * @param last_col  The last col of the range.
 *
 * @return 0 for success, non-zero on error.
 *
 * The `%worksheet_autofilter()` method allows an autofilter to be added to a
 * worksheet.
 *
 * An autofilter is a way of adding drop down lists to the headers of a 2D
 * range of worksheet data. This allows users to filter the data based on
 * simple criteria so that some data is shown and some is hidden.
 *
 * @image html autofilter.png
 *
 * To add an autofilter to a worksheet:
 *
 * @code
 *     worksheet_autofilter(worksheet, 0, 0, 50, 3);
 *
 *     // Same as above using the RANGE() macro.
 *     worksheet_autofilter(worksheet, RANGE("A1:D51"));
 * @endcode
 *
 * Note: it isn't currently possible to apply filter conditions to the
 * autofilter.
 */
uint8_t worksheet_autofilter(lxw_worksheet *worksheet, lxw_row_t first_row,
                             lxw_col_t first_col, lxw_row_t last_row,
                             lxw_col_t last_col);

 /**
  * @brief Make a worksheet the active, i.e., visible worksheet.
  *
  * @param worksheet Pointer to a lxw_worksheet instance to be updated.
  *
  * The `%worksheet_activate()` function is used to specify which worksheet is
  * initially visible in a multi-sheet workbook:
  *
  * @code
  *     lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
  *     lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
  *     lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);
  *
  *     worksheet_activate(worksheet3);
  * @endcode
  *
  * @image html worksheet_activate.png
  *
  * More than one worksheet can be selected via the `worksheet_select()`
  * function, see below, however only one worksheet can be active.
  *
  * The default active worksheet is the first worksheet.
  *
  */
void worksheet_activate(lxw_worksheet *worksheet);

 /**
  * @brief Set a worksheet tab as selected.
  *
  * @param worksheet Pointer to a lxw_worksheet instance to be updated.
  *
  * The `%worksheet_select()` function is used to indicate that a worksheet is
  * selected in a multi-sheet workbook:
  *
  * @code
  *     worksheet_activate(worksheet1);
  *     worksheet_select(worksheet2);
  *     worksheet_select(worksheet3);
  *
  * @endcode
  *
  * A selected worksheet has its tab highlighted. Selecting worksheets is a
  * way of grouping them together so that, for example, several worksheets
  * could be printed in one go. A worksheet that has been activated via the
  * `worksheet_activate()` function will also appear as selected.
  *
  */
void worksheet_select(lxw_worksheet *worksheet);

/**
 * @brief Set the page orientation as landscape.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 *
 * This function is used to set the orientation of a worksheet's printed page
 * to landscape:
 *
 * @code
 *     worksheet_set_landscape(worksheet);
 * @endcode
 */
void worksheet_set_landscape(lxw_worksheet *worksheet);

/**
 * @brief Set the page orientation as portrait.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 *
 * This function is used to set the orientation of a worksheet's printed page
 * to portrait. The default worksheet orientation is portrait, so this
 * function isn't generally required:
 *
 * @code
 *     worksheet_set_portrait(worksheet);
 * @endcode
 */
void worksheet_set_portrait(lxw_worksheet *worksheet);

/**
 * @brief Set the page layout to page view mode.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 *
 * This function is used to display the worksheet in "Page View/Layout" mode:
 *
 * @code
 *     worksheet_set_page_view(worksheet);
 * @endcode
 */
void worksheet_set_page_view(lxw_worksheet *worksheet);

/**
 * @brief Set the paper type for printing.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param paper_type The Excel paper format type.
 *
 * This function is used to set the paper format for the printed output of a
 * worksheet. The following paper styles are available:
 *
 *
 *   Index    | Paper format            | Paper size
 *   :------- | :---------------------- | :-------------------
 *   0        | Printer default         | Printer default
 *   1        | Letter                  | 8 1/2 x 11 in
 *   2        | Letter Small            | 8 1/2 x 11 in
 *   3        | Tabloid                 | 11 x 17 in
 *   4        | Ledger                  | 17 x 11 in
 *   5        | Legal                   | 8 1/2 x 14 in
 *   6        | Statement               | 5 1/2 x 8 1/2 in
 *   7        | Executive               | 7 1/4 x 10 1/2 in
 *   8        | A3                      | 297 x 420 mm
 *   9        | A4                      | 210 x 297 mm
 *   10       | A4 Small                | 210 x 297 mm
 *   11       | A5                      | 148 x 210 mm
 *   12       | B4                      | 250 x 354 mm
 *   13       | B5                      | 182 x 257 mm
 *   14       | Folio                   | 8 1/2 x 13 in
 *   15       | Quarto                  | 215 x 275 mm
 *   16       | ---                     | 10x14 in
 *   17       | ---                     | 11x17 in
 *   18       | Note                    | 8 1/2 x 11 in
 *   19       | Envelope 9              | 3 7/8 x 8 7/8
 *   20       | Envelope 10             | 4 1/8 x 9 1/2
 *   21       | Envelope 11             | 4 1/2 x 10 3/8
 *   22       | Envelope 12             | 4 3/4 x 11
 *   23       | Envelope 14             | 5 x 11 1/2
 *   24       | C size sheet            | ---
 *   25       | D size sheet            | ---
 *   26       | E size sheet            | ---
 *   27       | Envelope DL             | 110 x 220 mm
 *   28       | Envelope C3             | 324 x 458 mm
 *   29       | Envelope C4             | 229 x 324 mm
 *   30       | Envelope C5             | 162 x 229 mm
 *   31       | Envelope C6             | 114 x 162 mm
 *   32       | Envelope C65            | 114 x 229 mm
 *   33       | Envelope B4             | 250 x 353 mm
 *   34       | Envelope B5             | 176 x 250 mm
 *   35       | Envelope B6             | 176 x 125 mm
 *   36       | Envelope                | 110 x 230 mm
 *   37       | Monarch                 | 3.875 x 7.5 in
 *   38       | Envelope                | 3 5/8 x 6 1/2 in
 *   39       | Fanfold                 | 14 7/8 x 11 in
 *   40       | German Std Fanfold      | 8 1/2 x 12 in
 *   41       | German Legal Fanfold    | 8 1/2 x 13 in
 *
 * Note, it is likely that not all of these paper types will be available to
 * the end user since it will depend on the paper formats that the user's
 * printer supports. Therefore, it is best to stick to standard paper types:
 *
 * @code
 *     worksheet_set_paper(worksheet1, 1);  // US Letter
 *     worksheet_set_paper(worksheet2, 9);  // A4
 * @endcode
 *
 * If you do not specify a paper type the worksheet will print using the
 * printer's default paper style.
 */
void worksheet_set_paper(lxw_worksheet *worksheet, uint8_t paper_type);

/**
 * @brief Set the worksheet margins for the printed page.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param left    Left margin in inches.   Excel default is 0.7.
 * @param right   Right margin in inches.  Excel default is 0.7.
 * @param top     Top margin in inches.    Excel default is 0.75.
 * @param bottom  Bottom margin in inches. Excel default is 0.75.
 *
 * The `%worksheet_set_margins()` function is used to set the margins of the
 * worksheet when it is printed. The units are in inches. Specifying `-1` for
 * any parameter will give the default Excel value as shown above.
 *
 * @code
 *    worksheet_set_margins(worksheet, 1.3, 1.2, -1, -1);
 * @endcode
 *
 */
void worksheet_set_margins(lxw_worksheet *worksheet, double left,
                           double right, double top, double bottom);

/**
 * @brief Set the printed page header caption.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param string    The header string.
 *
 * @return 0 for success, non-zero on error.
 *
 * Headers and footers are generated using a string which is a combination of
 * plain text and control characters.
 *
 * The available control character are:
 *
 *
 *   | Control         | Category      | Description           |
 *   | --------------- | ------------- | --------------------- |
 *   | `&L`            | Justification | Left                  |
 *   | `&C`            |               | Center                |
 *   | `&R`            |               | Right                 |
 *   | `&P`            | Information   | Page number           |
 *   | `&N`            |               | Total number of pages |
 *   | `&D`            |               | Date                  |
 *   | `&T`            |               | Time                  |
 *   | `&F`            |               | File name             |
 *   | `&A`            |               | Worksheet name        |
 *   | `&Z`            |               | Workbook path         |
 *   | `&fontsize`     | Font          | Font size             |
 *   | `&"font,style"` |               | Font name and style   |
 *   | `&U`            |               | Single underline      |
 *   | `&E`            |               | Double underline      |
 *   | `&S`            |               | Strikethrough         |
 *   | `&X`            |               | Superscript           |
 *   | `&Y`            |               | Subscript             |
 *
 *
 * Text in headers and footers can be justified (aligned) to the left, center
 * and right by prefixing the text with the control characters `&L`, `&C` and
 * `&R`.
 *
 * For example (with ASCII art representation of the results):
 *
 * @code
 *     worksheet_set_header(worksheet, "&LHello");
 *
 *      ---------------------------------------------------------------
 *     |                                                               |
 *     | Hello                                                         |
 *     |                                                               |
 *
 *
 *     worksheet_set_header(worksheet, "&CHello");
 *
 *      ---------------------------------------------------------------
 *     |                                                               |
 *     |                          Hello                                |
 *     |                                                               |
 *
 *
 *     worksheet_set_header(worksheet, "&RHello");
 *
 *      ---------------------------------------------------------------
 *     |                                                               |
 *     |                                                         Hello |
 *     |                                                               |
 *
 *
 * @endcode
 *
 * For simple text, if you do not specify any justification the text will be
 * centred. However, you must prefix the text with `&C` if you specify a font
 * name or any other formatting:
 *
 * @code
 *     worksheet_set_header(worksheet, "Hello");
 *
 *      ---------------------------------------------------------------
 *     |                                                               |
 *     |                          Hello                                |
 *     |                                                               |
 *
 * @endcode
 *
 * You can have text in each of the justification regions:
 *
 * @code
 *     worksheet_set_header(worksheet, "&LCiao&CBello&RCielo");
 *
 *      ---------------------------------------------------------------
 *     |                                                               |
 *     | Ciao                     Bello                          Cielo |
 *     |                                                               |
 *
 * @endcode
 *
 * The information control characters act as variables that Excel will update
 * as the workbook or worksheet changes. Times and dates are in the users
 * default format:
 *
 * @code
 *     worksheet_set_header(worksheet, "&CPage &P of &N");
 *
 *      ---------------------------------------------------------------
 *     |                                                               |
 *     |                        Page 1 of 6                            |
 *     |                                                               |
 *
 *     worksheet_set_header(worksheet, "&CUpdated at &T");
 *
 *      ---------------------------------------------------------------
 *     |                                                               |
 *     |                    Updated at 12:30 PM                        |
 *     |                                                               |
 *
 * @endcode
 *
 * You can specify the font size of a section of the text by prefixing it with
 * the control character `&n` where `n` is the font size:
 *
 * @code
 *     worksheet_set_header(worksheet1, "&C&30Hello Big");
 *     worksheet_set_header(worksheet2, "&C&10Hello Small");
 *
 * @endcode
 *
 * You can specify the font of a section of the text by prefixing it with the
 * control sequence `&"font,style"` where `fontname` is a font name such as
 * Windows font descriptions: "Regular", "Italic", "Bold" or "Bold Italic":
 * "Courier New" or "Times New Roman" and `style` is one of the standard
 *
 * @code
 *     worksheet_set_header(worksheet1, "&C&\"Courier New,Italic\"Hello");
 *     worksheet_set_header(worksheet2, "&C&\"Courier New,Bold Italic\"Hello");
 *     worksheet_set_header(worksheet3, "&C&\"Times New Roman,Regular\"Hello");
 *
 * @endcode
 *
 * It is possible to combine all of these features together to create
 * sophisticated headers and footers. As an aid to setting up complicated
 * headers and footers you can record a page set-up as a macro in Excel and
 * look at the format strings that VBA produces. Remember however that VBA
 * uses two double quotes `""` to indicate a single double quote. For the last
 * example above the equivalent VBA code looks like this:
 *
 * @code
 *     .LeftHeader = ""
 *     .CenterHeader = "&""Times New Roman,Regular""Hello"
 *     .RightHeader = ""
 *
 * @endcode
 *
 * Alternatively you can inspect the header and footer strings in an Excel
 * file by unzipping it and grepping the XML sub-files. The following shows
 * how to do that using libxml's xmllint to format the XML for clarity:
 *
 * @code
 *
 *    $ unzip myfile.xlsm -d myfile
 *    $ xmllint --format `find myfile -name "*.xml" | xargs` | egrep "Header|Footer"
 *
 *      <headerFooter scaleWithDoc="0">
 *        <oddHeader>&amp;L&amp;P</oddHeader>
 *      </headerFooter>
 *
 * @endcode
 *
 * Note that in this case you need to unescape the Html. In the above example
 * the header string would be `&L&P`.
 *
 * To include a single literal ampersand `&` in a header or footer you should
 * use a double ampersand `&&`:
 *
 * @code
 *     worksheet_set_header(worksheet, "&CCuriouser && Curiouser - Attorneys at Law");
 * @endcode
 *
 * Note, the header or footer string must be less than 255 characters. Strings
 * longer than this will not be written.
 *
 */
uint8_t worksheet_set_header(lxw_worksheet *worksheet, char *string);

/**
 * @brief Set the printed page footer caption.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param string    The footer string.
 *
 * @return 0 for success, non-zero on error.
 *
 * The syntax of this function is the same as worksheet_set_header().
 *
 */
uint8_t worksheet_set_footer(lxw_worksheet *worksheet, char *string);

/**
 * @brief Set the printed page header caption with additional options.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param string    The header string.
 * @param options   Header options.
 *
 * @return 0 for success, non-zero on error.
 *
 * The syntax of this function is the same as worksheet_set_header() with an
 * additional parameter to specify options for the header.
 *
 * Currently, the only available option is the header margin:
 *
 * @code
 *
 *    lxw_header_footer_options header_options = { 0.2 };
 *
 *    worksheet_set_header_opt(worksheet, "Some text", &header_options);
 *
 * @endcode
 *
 */
uint8_t worksheet_set_header_opt(lxw_worksheet *worksheet, char *string,
                                 lxw_header_footer_options *options);

/**
 * @brief Set the printed page footer caption with additional options.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param string    The footer string.
 * @param options   Footer options.
 *
 * @return 0 for success, non-zero on error.
 *
 * The syntax of this function is the same as worksheet_set_header_opt().
 *
 */
uint8_t worksheet_set_footer_opt(lxw_worksheet *worksheet, char *string,
                                 lxw_header_footer_options *options);

/**
 * @brief Set the horizontal page breaks on a worksheet.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param breaks    Array of page breaks.
 *
 * The `%worksheet_set_h_pagebreaks()` function adds horizontal page breaks to
 * a worksheet. A page break causes all the data that follows it to be printed
 * on the next page. Horizontal page breaks act between rows.
 *
 * The function takes an array of one or more page breaks. The type of the
 * array data is @ref lxw_row_t and the last element of the array must be 0:
 *
 * @code
 *    lxw_row_t breaks1[] = {20, 0}; // 1 page break. Zero indicates the end.
 *    lxw_row_t breaks2[] = {20, 40, 60, 80, 0};
 *
 *    worksheet_set_h_pagebreaks(worksheet1, breaks1);
 *    worksheet_set_h_pagebreaks(worksheet2, breaks2);
 * @endcode
 *
 * To create a page break between rows 20 and 21 you must specify the break at
 * row 21. However in zero index notation this is actually row 20:
 *
 * @code
 *    // Break between row 20 and 21.
 *    lxw_row_t breaks[] = {20, 0};
 *
 *    worksheet_set_h_pagebreaks(worksheet, breaks);
 * @endcode
 *
 * There is an Excel limitation of 1023 horizontal page breaks per worksheet.
 *
 * Note: If you specify the "fit to page" option via the
 * `worksheet_fit_to_pages()` function it will override all manual page
 * breaks.
 *
 */
void worksheet_set_h_pagebreaks(lxw_worksheet *worksheet, lxw_row_t breaks[]);

/**
 * @brief Set the vertical page breaks on a worksheet.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param breaks    Array of page breaks.
 *
 * The `%worksheet_set_v_pagebreaks()` function adds vertical page breaks to a
 * worksheet. A page break causes all the data that follows it to be printed
 * on the next page. Vertical page breaks act between columns.
 *
 * The function takes an array of one or more page breaks. The type of the
 * array data is @ref lxw_col_t and the last element of the array must be 0:
 *
 * @code
 *    lxw_col_t breaks1[] = {20, 0}; // 1 page break. Zero indicates the end.
 *    lxw_col_t breaks2[] = {20, 40, 60, 80, 0};
 *
 *    worksheet_set_v_pagebreaks(worksheet1, breaks1);
 *    worksheet_set_v_pagebreaks(worksheet2, breaks2);
 * @endcode
 *
 * To create a page break between columns 20 and 21 you must specify the break
 * at column 21. However in zero index notation this is actually column 20:
 *
 * @code
 *    // Break between column 20 and 21.
 *    lxw_col_t breaks[] = {20, 0};
 *
 *    worksheet_set_v_pagebreaks(worksheet, breaks);
 * @endcode
 *
 * There is an Excel limitation of 1023 vertical page breaks per worksheet.
 *
 * Note: If you specify the "fit to page" option via the
 * `worksheet_fit_to_pages()` function it will override all manual page
 * breaks.
 *
 */
void worksheet_set_v_pagebreaks(lxw_worksheet *worksheet, lxw_col_t breaks[]);

/**
 * @brief Set the order in which pages are printed.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 *
 * The `%worksheet_print_across()` function is used to change the default
 * print direction. This is referred to by Excel as the sheet "page order":
 *
 * @code
 *     worksheet_print_across(worksheet);
 * @endcode
 *
 * The default page order is shown below for a worksheet that extends over 4
 * pages. The order is called "down then across":
 *
 *     [1] [3]
 *     [2] [4]
 *
 * However, by using the `print_across` function the print order will be
 * changed to "across then down":
 *
 *     [1] [2]
 *     [3] [4]
 *
 */
void worksheet_print_across(lxw_worksheet *worksheet);

/**
 * @brief Set the option to display or hide gridlines on the screen and
 *        the printed page.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param option    Gridline option.
 *
 * Display or hide screen and print gridlines using one of the values of
 * @ref lxw_gridlines.
 *
 * @code
 *    worksheet_gridlines(worksheet1, LXW_HIDE_ALL_GRIDLINES);
 *
 *    worksheet_gridlines(worksheet2, LXW_SHOW_PRINT_GRIDLINES);
 * @endcode
 *
 * The Excel default is that the screen gridlines are on  and the printed
 * worksheet is off.
 *
 */
void worksheet_gridlines(lxw_worksheet *worksheet, uint8_t option);

/**
 * @brief Center the printed page horizontally.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 *
 * Center the worksheet data horizontally between the margins on the printed
 * page:
 *
 * @code
 *     worksheet_center_horizontally(worksheet);
 * @endcode
 *
 */
void worksheet_center_horizontally(lxw_worksheet *worksheet);

/**
 * @brief Center the printed page vertically.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 *
 * Center the worksheet data vertically between the margins on the printed
 * page:
 *
 * @code
 *     worksheet_center_vertically(worksheet);
 * @endcode
 *
 */
void worksheet_center_vertically(lxw_worksheet *worksheet);

/**
 * @brief Set the option to print the row and column headers on the printed
 *        page.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 *
 * When printing a worksheet from Excel the row and column headers (the row
 * numbers on the left and the column letters at the top) aren't printed by
 * default.
 *
 * This function sets the printer option to print these headers:
 *
 * @code
 *    worksheet_print_row_col_headers(worksheet);
 * @endcode
 *
 */
void worksheet_print_row_col_headers(lxw_worksheet *worksheet);

/**
 * @brief Set the number of rows to repeat at the top of each printed page.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param first_row First row of repeat range.
 * @param last_row  Last row of repeat range.
 *
 * For large Excel documents it is often desirable to have the first row or
 * rows of the worksheet print out at the top of each page.
 *
 * This can be achieved by using this function. The parameters `first_row`
 * and `last_row` are zero based:
 *
 * @code
 *     worksheet_repeat_rows(worksheet, 0, 0); // Repeat the first row.
 *     worksheet_repeat_rows(worksheet, 0, 1); // Repeat the first two rows.
 * @endcode
 *
 * @return 0 for success, non-zero on error.
 */
uint8_t worksheet_repeat_rows(lxw_worksheet *worksheet, lxw_row_t first_row,
                              lxw_row_t last_row);

/**
 * @brief Set the number of columns to repeat at the top of each printed page.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param first_col First column of repeat range.
 * @param last_col  Last column of repeat range.
 *
 * For large Excel documents it is often desirable to have the first column or
 * columns of the worksheet print out at the left of each page.
 *
 * This can be achieved by using this function. The parameters `first_col`
 * and `last_col` are zero based:
 *
 * @code
 *     worksheet_repeat_columns(worksheet, 0, 0); // Repeat the first col.
 *     worksheet_repeat_columns(worksheet, 0, 1); // Repeat the first two cols.
 * @endcode
 *
 * @return 0 for success, non-zero on error.
 */
uint8_t worksheet_repeat_columns(lxw_worksheet *worksheet,
                                 lxw_col_t first_col, lxw_col_t last_col);

/**
 * @brief Set the print area for a worksheet.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param first_row The first row of the range. (All zero indexed.)
 * @param first_col The first column of the range.
 * @param last_row  The last row of the range.
 * @param last_col  The last col of the range.
 *
 * This function is used to specify the area of the worksheet that will be
 * printed. The RANGE() macro is often convenient for this.
 *
 * @code
 *     worksheet_print_area(worksheet, 0, 0, 41, 10); // A1:K42.
 *
 *     // Same as:
 *     worksheet_print_area(worksheet, RANGE("A1:K42"));
 * @endcode
 *
 * In order to set a row or column range you must specify the entire range:
 *
 * @code
 *     worksheet_print_area(worksheet, RANGE("A1:H1048576")); // Same as A:H.
 * @endcode
 *
 * @return 0 for success, non-zero on error.
 */
uint8_t worksheet_print_area(lxw_worksheet *worksheet, lxw_row_t first_row,
                             lxw_col_t first_col, lxw_row_t last_row,
                             lxw_col_t last_col);
/**
 * @brief Fit the printed area to a specific number of pages both vertically
 *        and horizontally.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param width     Number of pages horizontally.
 * @param height    Number of pages vertically.
 *
 * The `%worksheet_fit_to_pages()` function is used to fit the printed area to
 * a specific number of pages both vertically and horizontally. If the printed
 * area exceeds the specified number of pages it will be scaled down to
 * fit. This ensures that the printed area will always appear on the specified
 * number of pages even if the page size or margins change:
 *
 * @code
 *     worksheet_fit_to_pages(worksheet1, 1, 1); // Fit to 1x1 pages.
 *     worksheet_fit_to_pages(worksheet2, 2, 1); // Fit to 2x1 pages.
 *     worksheet_fit_to_pages(worksheet3, 1, 2); // Fit to 1x2 pages.
 * @endcode
 *
 * The print area can be defined using the `worksheet_print_area()` function
 * as described above.
 *
 * A common requirement is to fit the printed output to `n` pages wide but
 * have the height be as long as necessary. To achieve this set the `height`
 * to zero:
 *
 * @code
 *     // 1 page wide and as long as necessary.
 *     worksheet_fit_to_pages(worksheet, 1, 0);
 * @endcode
 *
 * **Note**:
 *
 * - Although it is valid to use both `%worksheet_fit_to_pages()` and
 *   `worksheet_set_print_scale()` on the same worksheet Excel only allows one
 *   of these options to be active at a time. The last function call made will
 *   set the active option.
 *
 * - The `%worksheet_fit_to_pages()` function will override any manual page
 *   breaks that are defined in the worksheet.
 *
 * - When using `%worksheet_fit_to_pages()` it may also be required to set the
 *   printer paper size using `worksheet_set_paper()` or else Excel will
 *   default to "US Letter".
 *
 */
void worksheet_fit_to_pages(lxw_worksheet *worksheet, uint16_t width,
                            uint16_t height);

/**
 * @brief Set the start page number when printing.
 *
 * @param worksheet  Pointer to a lxw_worksheet instance to be updated.
 * @param start_page Starting page number.
 *
 * The `%worksheet_set_start_page()` function is used to set the number of
 * the starting page when the worksheet is printed out:
 *
 * @code
 *     // Start print from page 2.
 *     worksheet_set_start_page(worksheet, 2);
 * @endcode
 */
void worksheet_set_start_page(lxw_worksheet *worksheet, uint16_t start_page);

/**
 * @brief Set the scale factor for the printed page.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 * @param scale     Print scale of worksheet to be printed.
 *
 * This function sets the scale factor of the printed page. The Scale factor
 * must be in the range `10 <= scale <= 400`:
 *
 * @code
 *     worksheet_set_print_scale(worksheet1, 75);
 *     worksheet_set_print_scale(worksheet2, 400);
 * @endcode
 *
 * The default scale factor is 100. Note, `%worksheet_set_print_scale()` does
 * not affect the scale of the visible page in Excel. For that you should use
 * `worksheet_set_zoom()`.
 *
 * Note that although it is valid to use both `worksheet_fit_to_pages()` and
 * `%worksheet_set_print_scale()` on the same worksheet Excel only allows one
 * of these options to be active at a time. The last function call made will
 * set the active option.
 *
 */
void worksheet_set_print_scale(lxw_worksheet *worksheet, uint16_t scale);

lxw_worksheet *_new_worksheet(lxw_worksheet_init_data *init_data);
void _free_worksheet(lxw_worksheet *worksheet);
void _worksheet_assemble_xml_file(lxw_worksheet *worksheet);
void _worksheet_write_single_row(lxw_worksheet *worksheet);

/* Declarations required for unit testing. */
#ifdef TESTING

STATIC void _worksheet_xml_declaration(lxw_worksheet *worksheet);
STATIC void _worksheet_write_worksheet(lxw_worksheet *worksheet);
STATIC void _worksheet_write_dimension(lxw_worksheet *worksheet);
STATIC void _worksheet_write_sheet_view(lxw_worksheet *worksheet);
STATIC void _worksheet_write_sheet_views(lxw_worksheet *worksheet);
STATIC void _worksheet_write_sheet_format_pr(lxw_worksheet *worksheet);
STATIC void _worksheet_write_sheet_data(lxw_worksheet *worksheet);
STATIC void _worksheet_write_page_margins(lxw_worksheet *worksheet);
STATIC void _worksheet_write_page_setup(lxw_worksheet *worksheet);
STATIC void _worksheet_write_col_info(lxw_worksheet *worksheet,
                                      lxw_col_options *options);
STATIC void _write_row(lxw_worksheet *worksheet, lxw_row *row, char *spans);
STATIC lxw_row *_get_row_list(struct lxw_table_rows *table,
                              lxw_row_t row_num);

STATIC void _worksheet_write_merge_cell(lxw_worksheet *worksheet,
                                        lxw_merged_range *merged_range);
STATIC void _worksheet_write_merge_cells(lxw_worksheet *worksheet);

STATIC void _worksheet_write_odd_header(lxw_worksheet *worksheet);
STATIC void _worksheet_write_odd_footer(lxw_worksheet *worksheet);
STATIC void _worksheet_write_header_footer(lxw_worksheet *worksheet);

STATIC void _worksheet_write_print_options(lxw_worksheet *worksheet);
#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_WORKSHEET_H__ */
