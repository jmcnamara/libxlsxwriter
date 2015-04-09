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

#define LXW_COL_META_MAX 128
#define LXW_HEADER_FOOTER_MAX 255

/** Default column width in Excel */
#define LXW_DEF_COL_WIDTH 8.43

/** Default row height in Excel */
#define LXW_DEF_ROW_HEIGHT 15

/** Error codes from `worksheet_write*()` functions. */
enum lxw_write_error {
    LXW_WRITE_ERROR_NONE = 0,
    LXW_RANGE_ERROR,
    LXW_STRING_HASH_ERROR,
    LXW_STRING_LENGTH_ERROR,
    LXW_END
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
    BLANK_CELL
};

/* Define the queue.h TAILQ structs for the list head types. */
TAILQ_HEAD(lxw_table_cells, lxw_cell);
TAILQ_HEAD(lxw_table_rows, lxw_row);
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
    struct lxw_cell **array;
    struct lxw_merged_ranges *merged_ranges;

    lxw_row_t dim_rowmin;
    lxw_row_t dim_rowmax;
    lxw_col_t dim_colmin;
    lxw_col_t dim_colmax;

    lxw_sst *sst;
    char *name;

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
    uint16_t page_start;
    uint16_t print_scale;
    uint16_t vertical_dpi;
    uint8_t fit_page;
    uint8_t orientation;
    uint8_t page_order;
    uint8_t page_setup_changed;
    uint8_t page_view;
    uint8_t paper_size;
    double margin_left;
    double margin_right;
    double margin_top;
    double margin_bottom;
    double margin_header;
    double margin_footer;

    uint8_t header_footer_changed;
    char header[LXW_HEADER_FOOTER_MAX];
    char footer[LXW_HEADER_FOOTER_MAX];

    uint16_t merged_range_count;

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

    /* List pointers for queue.h. */
    TAILQ_ENTRY (lxw_row) list_pointers;
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
 * @return A #lxw_write_error code.
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
 * @return A #lxw_write_error code.
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
 * @return A #lxw_write_error code.
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
 * @brief Write a formula to a worksheet cell with a user defined result.
 *
 * @param worksheet Pointer to the lxw_worksheet instance to be updated.
 * @param row       The zero indexed row number.
 * @param col       The zero indexed column number.
 * @param formula   Formula string to write to cell.
 * @param format    A pointer to a Format instance or NULL.
 * @param result    A user defined result for a formula.
 *
 * @return A #lxw_write_error code.
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
 * @brief Write a date or time to a worksheet cell.
 *
 * @param worksheet Pointer to the lxw_worksheet instance to be updated.
 * @param row       The zero indexed row number.
 * @param col       The zero indexed column number.
 * @param datetime  The datetime to write to the cell.
 * @param format    A pointer to a Format instance or NULL.
 *
 * @return A #lxw_write_error code.
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

/**
 * @brief Write a formatted blank worksheet cell.
 *
 * @param worksheet Pointer to the lxw_worksheet instance to be updated.
 * @param row       The zero indexed row number.
 * @param col       The zero indexed column number.
 * @param format    A pointer to a Format instance or NULL.
 *
 * @return A #lxw_write_error code.
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
 * * `hidden`
 * * `level`
 * * `collapsed`
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
 * * `hidden`
 * * `level`
 * * `collapsed`
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
 * The `merge_range()` function allows cells to be merged together so that
 * they act as a single area.
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
 * @image html merge_range.png
 *
 * The `merge_range()` function writes a `char*` string using
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
  * @brief Make a worksheet the active, i.e., visible worksheet.
  *
  * @param worksheet Pointer to a lxw_worksheet instance to be updated.
  *
  * The `activate()` method is used to specify which worksheet is initially
  * visible in a multi-sheet workbook:
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
  * More than one worksheet can be selected via the `select()` method, see
  * below, however only one worksheet can be active.
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
  * The `select()` method is used to indicate that a worksheet is selected in
  * a multi-sheet workbook:
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
  * `activate()` method will also appear as selected.
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
 * The `set_margins()` function is used to set the margins of the worksheet
 * when it is printed. The units are in inches. Specifying `-1` for any
 * parameter will give the default Excel value as shown above.
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
 *    $ xmllint --format `find myfile -name "*.xml" | xargs` \
 *                        | egrep "Header|Footer"
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
                                 lxw_header_footer_options * options);

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
                                 lxw_header_footer_options * options);

/**
 * @brief Set the order in which pages are printed.
 *
 * @param worksheet Pointer to a lxw_worksheet instance to be updated.
 *
 * The `print_across()` function is used to change the default print
 * direction. This is referred to by Excel as the sheet "page order":
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
STATIC void _write_row(lxw_worksheet *worksheet, lxw_row *row, char *spans);
STATIC void _write_col_info(lxw_worksheet *worksheet,
                            lxw_col_options *options);
STATIC lxw_row *_get_row_list(lxw_worksheet *worksheet, lxw_row_t row_num);

STATIC void _write_merge_cell(lxw_worksheet *worksheet,
                              lxw_merged_range * merged_range);
STATIC void _write_merge_cells(lxw_worksheet *worksheet);

STATIC void _worksheet_write_odd_header(lxw_worksheet *worksheet);
STATIC void _worksheet_write_odd_footer(lxw_worksheet *worksheet);
STATIC void _worksheet_write_header_footer(lxw_worksheet *worksheet);

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_WORKSHEET_H__ */
