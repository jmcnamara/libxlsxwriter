/*****************************************************************************
 *
 * An example of adding macros to a libxlsxwriter file using a VBA project
 * file extracted from an existing Excel .xlsm file.
 *
 * The vba_extract.py utility from the libxlsxwriter examples directory can be
 * used to extract the vbaProject.bin file.
 *
 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("macro.xlsm");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Add a macro that will execute when the file is opened. */
    workbook_add_vba_project(workbook, "vbaProject.bin");

    worksheet_write_string(worksheet, 0, 0, "Overwrite this", NULL);

    return workbook_close(workbook);
}
