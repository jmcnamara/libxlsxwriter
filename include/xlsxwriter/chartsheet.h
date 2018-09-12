/*
 * libxlsxwriter
 *
 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 * chartsheet - A libxlsxwriter library for creating Excel XLSX chartsheet files.
 *
 */

/**
 * @page chartsheet_page The Chartsheet object
 *
 * The Chartsheet object represents an Excel chartsheet, which is a type of
 * worksheet that only contains a chart. The Chartsheet object handles
 * operations such as adding a chart and setting the page layout.
 *
 * See @ref chartsheet.h for full details of the functionality.
 *
 * @file chartsheet.h
 *
 * @brief Functions related to adding data and formatting to a chartsheet.
 *
 * The Chartsheet object represents an Excel chartsheet. It handles operations
 * such as adding a chart and setting the page layout.
 *
 * A Chartsheet object isn't created directly. Instead a chartsheet is created
 * by calling the workbook_add_chartsheet() function from a Workbook object. A
 * chartsheet object functions as a worksheet and not as a chart. In order to
 * have it display data a #lxw_chart object must be created and added to the
 * chartsheet:
 *
 * @code
 *     #include "xlsxwriter.h"
 *
 *     int main() {
 *
 *         lxw_workbook   *workbook   = new_workbook("chartsheet.xlsx");
 *         lxw_worksheet  *worksheet  = workbook_add_worksheet(workbook, NULL);
 *         lxw_chartsheet *chartsheet = workbook_add_chartsheet(workbook, NULL);
 *         lxw_chart      *chart      = workbook_add_chart(workbook, LXW_CHART_BAR);
 *
 *         //... Set up the chart.
 *
 *         // Add the chart to the chartsheet.
 *         return workbook_close(workbook);
 *
 *     }
 * @endcode
 *
 * @image html chartsheet.png
 *
 * The data for the chartsheet chart must be contained on a separate
 * worksheet. That is why it is always created in conjunction with at least
 * one data worksheet, as shown above.
 */

#ifndef __LXW_CHARTSHEET_H__
#define __LXW_CHARTSHEET_H__

#include <stdint.h>

#include "common.h"
#include "worksheet.h"
#include "drawing.h"

/**
 * @brief Struct to represent an Excel chartsheet.
 *
 * The members of the lxw_chartsheet struct aren't modified directly. Instead
 * the chartsheet properties are set by calling the functions shown in
 * chartsheet.h.
 */
typedef struct lxw_chartsheet {

    FILE *file;
    lxw_worksheet *worksheet;

    char *name;
    char *quoted_name;
    char *tmpdir;
    uint32_t index;
    uint8_t active;
    uint8_t selected;
    uint8_t hidden;
    uint16_t *active_sheet;
    uint16_t *first_sheet;
    uint16_t rel_count;

    STAILQ_ENTRY (lxw_chartsheet) list_pointers;

} lxw_chartsheet;


/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

/**
 * @brief Insert a chart object into a chartsheet.
 *
 * @param chartsheet   Pointer to a lxw_chartsheet instance to be updated.
 * @param chart        A #lxw_chart object created via workbook_add_chart().
 *
 * @return A #lxw_error code.
 *
 * The `%chartsheet_set_chart()` function can be used to insert a chart into a
 * chartsheet. The chart object must be created first using the
 * `workbook_add_chart()` function and configured using the @ref chart.h
 * functions.
 *
 * @code
 *     // Create the chartsheet.
 *     lxw_chartsheet *chartsheet = workbook_add_chartsheet(workbook, NULL);
 *
 *     // Create a chart object.
 *     lxw_chart *chart = workbook_add_chart(workbook, LXW_CHART_LINE);
 *
 *     // Add a data series to the chart.
 *     chart_add_series(chart, NULL, "=Sheet1!$A$1:$A$6");
 *
 *     // Insert the chart into the chartsheet.
 *     chartsheet_set_chart(chartsheet, chart);
 * @endcode
 *
 * @image html chartsheet2.png
 *
 * **Note:**
 *
 * A chart may only be inserted once into a chartsheet or a worksheet. If
 * several similar charts are required then each one must be created
 * separately.
 *
 */
lxw_error chartsheet_set_chart(lxw_chartsheet *chartsheet, lxw_chart *chart);

/* Not currently required since scale options aren't useful in a chartsheet. */
lxw_error chartsheet_set_chart_opt(lxw_chartsheet *chartsheet,
                                   lxw_chart *chart,
                                   lxw_image_options *user_options);

/**
 * @brief Make a chartsheet the active, i.e., visible chartsheet.
 *
 * @param chartsheet Pointer to a lxw_chartsheet instance to be updated.
 *
 * The `%chartsheet_activate()` function is used to specify which chartsheet
 * is initially visible in a multi-sheet workbook:
 *
 * @code
 *     lxw_worksheet  *worksheet1  = workbook_add_worksheet(workbook, NULL);
 *     lxw_chartsheet *chartsheet1 = workbook_add_chartsheet(workbook, NULL);
 *     lxw_chartsheet *chartsheet2 = workbook_add_chartsheet(workbook, NULL);
 *     lxw_chartsheet *chartsheet3 = workbook_add_chartsheet(workbook, NULL);
 *
 *     chartsheet_activate(chartsheet3);
 * @endcode
 *
 * @image html chartsheet_activate.png
 *
 * More than one chartsheet can be selected via the `chartsheet_select()`
 * function, see below, however only one chartsheet can be active.
 *
 * The default active chartsheet is the first chartsheet.
 *
 * See also `worksheet_activate()`.
 *
 */
void chartsheet_activate(lxw_chartsheet *chartsheet);

/**
 * @brief Set a chartsheet tab as selected.
 *
 * @param chartsheet Pointer to a lxw_chartsheet instance to be updated.
 *
 * The `%chartsheet_select()` function is used to indicate that a chartsheet
 * is selected in a multi-sheet workbook:
 *
 * @code
 *     chartsheet_activate(chartsheet1);
 *     chartsheet_select(chartsheet2);
 *     chartsheet_select(chartsheet3);
 *
 * @endcode
 *
 * A selected chartsheet has its tab highlighted. Selecting chartsheets is a
 * way of grouping them together so that, for example, several chartsheets
 * could be printed in one go. A chartsheet that has been activated via the
 * `chartsheet_activate()` function will also appear as selected.
 *
 * See also `worksheet_select()`.
 *
 */
void chartsheet_select(lxw_chartsheet *chartsheet);

/**
 * @brief Hide the current chartsheet.
 *
 * @param chartsheet Pointer to a lxw_chartsheet instance to be updated.
 *
 * The `%chartsheet_hide()` function is used to hide a chartsheet:
 *
 * @code
 *     chartsheet_hide(chartsheet2);
 * @endcode
 *
 * You may wish to hide a chartsheet in order to avoid confusing a user with
 * intermediate data or calculations.
 *
 * @image html hide_sheet.png
 *
 * A hidden chartsheet can not be activated or selected so this function is
 * mutually exclusive with the `chartsheet_activate()` and
 * `chartsheet_select()` functions. In addition, since the first chartsheet
 * will default to being the active chartsheet, you cannot hide the first
 * chartsheet without activating another sheet:
 *
 * @code
 *     chartsheet_activate(chartsheet2);
 *     chartsheet_hide(chartsheet1);
 * @endcode
 *
 * See also `worksheet_hide()`.
 *
 */
void chartsheet_hide(lxw_chartsheet *chartsheet);

/**
 * @brief Set current chartsheet as the first visible sheet tab.
 *
 * @param chartsheet Pointer to a lxw_chartsheet instance to be updated.
 *
 * The `chartsheet_activate()` function determines which chartsheet is
 * initially selected.  However, if there are a large number of chartsheets the
 * selected chartsheet may not appear on the screen. To avoid this you can
 * select the leftmost visible chartsheet tab using
 * `%chartsheet_set_first_sheet()`:
 *
 * @code
 *     chartsheet_set_first_sheet(chartsheet19); // First visible chartsheet tab.
 *     chartsheet_activate(chartsheet20);        // First visible chartsheet.
 * @endcode
 *
 * This function is not required very often. The default value is the first
 * chartsheet.
 *
 * See also `worksheet_set_first_sheet()`.
 *
 */
void chartsheet_set_first_sheet(lxw_chartsheet *chartsheet);

lxw_chartsheet *lxw_chartsheet_new();
void lxw_chartsheet_free(lxw_chartsheet *chartsheet);
void lxw_chartsheet_assemble_xml_file(lxw_chartsheet *self);

/* Declarations required for unit testing. */
#ifdef TESTING

STATIC void _chartsheet_xml_declaration(lxw_chartsheet *self);

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_CHARTSHEET_H__ */
