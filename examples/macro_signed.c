/*****************************************************************************
 *
 * An example of adding signed macros to a libxlsxwriter file using a VBA 
 * project file and a VBA project signature file extracted from an existing 
 * Excel .xlsm file.
 *
 * The vba_extract.py utility from the libxlsxwriter examples directory can be
 * used to extract the vbaProject.bin file and the vbaProjectSignature.bin file
 * (if the macros were signed).
 *
 * This example connects the macro to a button (the only Excel/VBA form object
 * supported by libxlsxwriter) but that isn't a requirement for adding a macro
 * file to the workbook.
 *
 * Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
 */

#include "xlsxwriter.h"

int main() {

    /* Note the xlsm extension of the filename */
    lxw_workbook  *workbook  = workbook_new("macro_signed.xlsm");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_set_column(worksheet, COLS("A:A"), 30, NULL);

    /* Add a macro file extracted from an Excel workbook. */
    workbook_add_vba_project(workbook, "vbaProject.bin");

    /* Add a VBA project signature file extracted from an Excel workbook. */
    workbook_add_vba_project_signature(workbook, "vbaProjectSignature.bin");

    worksheet_write_string(worksheet, 2, 0, "Press the button to say hello.", NULL);

    lxw_button_options options = {.caption = "Press Me", .macro = "say_hello",
                                  .width = 80, .height = 30};

     worksheet_insert_button(worksheet, 2, 1, &options);


    return workbook_close(workbook);
}
