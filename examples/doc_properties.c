/*
 * Example of setting document properties such as Author, Title, etc., for an
 * Excel spreadsheet using libxlsxwriter.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook       *workbook   = workbook_new("doc_properties.xlsx");
    lxw_worksheet      *worksheet  = workbook_add_worksheet(workbook, NULL);

    /* Create a properties structure and set some of the fields. */
    lxw_doc_properties properties = {
        .title    = "This is an example spreadsheet",
        .subject  = "With document properties",
        .author   = "John McNamara",
        .manager  = "Dr. Heinz Doofenshmirtz",
        .company  = "of Wolves",
        .category = "Example spreadsheets",
        .keywords = "Sample, Example, Properties",
        .comments = "Created with libxlsxwriter",
        .status   = "Quo",
    };

    /* Set the properties in the workbook. */
    workbook_set_properties(workbook, &properties);

    /* Add some text to the file. */
    worksheet_set_column(worksheet, 0, 0, 70, NULL);
    worksheet_write_string(worksheet, 0, 0,
        "Select 'Workbook Properties' to see properties." , NULL);

    workbook_close(workbook);

    return 0;
}
