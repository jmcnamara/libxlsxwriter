/*
 * Example of setting document properties such as Author, Title, etc., for an
 * Excel spreadsheet using libxlsxwriter.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook       *workbook   = new_workbook("doc_properties.xlsx");
    lxw_worksheet      *worksheet  = workbook_add_worksheet(workbook, NULL);

    /* Create a properties structure and set some of the fields. */
    lxw_doc_properties *properties = calloc(1, sizeof(lxw_doc_properties));

    properties->title    = strdup("This is an example spreadsheet");
    properties->subject  = strdup("With document properties");
    properties->author   = strdup("John McNamara");
    properties->manager  = strdup("Dr. Heinz Doofenshmirtz");
    properties->company  = strdup("of Wolves");
    properties->category = strdup("Example spreadsheets");
    properties->keywords = strdup("Sample, Example, Properties");
    properties->comments = strdup("Created with libxlsxwriter");
    properties->status   = strdup("Quo");

    /* Set the properties in the workbook. */
    workbook_set_properties(workbook, properties);

    /* Add some text to the file. */
    worksheet_set_column(worksheet, 0, 0, 70, NULL, NULL);
    worksheet_write_string(worksheet, 0, 0,
        "Select 'Workbook Properties' to see properties." , NULL);

    workbook_close(workbook);

    /* Clean up memory used for properties. */
    free(properties->title);
    free(properties->subject);
    free(properties->author);
    free(properties->manager);
    free(properties->company);
    free(properties->category);
    free(properties->keywords);
    free(properties->comments);
    free(properties->status);
    free(properties);


    return 0;
}
