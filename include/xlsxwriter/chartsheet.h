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
 * The Chartsheet object represents an Excel chartsheet. It handles
 * operations such as writing data to cells or formatting chartsheet
 * layout.
 *
 * See @ref chartsheet.h for full details of the functionality.
 *
 * @file chartsheet.h
 *
 * @brief Functions related to adding data and formatting to a chartsheet.
 *
 * The Chartsheet object represents an Excel chartsheet. It handles
 * operations such as writing data to cells or formatting chartsheet
 * layout.
 *
 * A Chartsheet object isn't created directly. Instead a chartsheet is
 * created by calling the workbook_add_chartsheet() function from a
 * Workbook object:
 *
 * @code
 *     #include "xlsxwriter.h"
 *
 *     int main() {
 *
 *         lxw_workbook  *workbook  = workbook_new("filename.xlsx");
 *         lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
 *
 *         worksheet_write_string(worksheet, 0, 0, "Hello Excel", NULL);
 *
 *         return workbook_close(workbook);
 *     }
 * @endcode
 *
 */

#ifndef __LXW_CHARTSHEET_H__
#define __LXW_CHARTSHEET_H__

#include <stdint.h>

#include "common.h"
#include "worksheet.h"
#include "drawing.h"

/*
 * Struct to represent a chartsheet object.
 */
typedef struct lxw_chartsheet {

    FILE *file;
    lxw_worksheet *worksheet;
    lxw_drawing *drawing;

    uint16_t rel_count;

} lxw_chartsheet;


/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

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
