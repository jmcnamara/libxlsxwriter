/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook       *workbook   = workbook_new("test_properties01.xlsx");
    lxw_worksheet      *worksheet  = workbook_add_worksheet(workbook, NULL);
    lxw_doc_properties *properties = calloc(1, sizeof(lxw_doc_properties));

    properties->title    = strdup("This is an example spreadsheet");
    properties->subject  = strdup("With document properties");
    properties->author   = strdup("Someone");
    properties->manager  = strdup("Dr. Heinz Doofenshmirtz");
    properties->company  = strdup("of Wolves");
    properties->category = strdup("Example spreadsheets");
    properties->keywords = strdup("Sample, Example, Properties");
    properties->comments = strdup("Created with Perl and Excel::Writer::XLSX");
    properties->status   = strdup("Quo");

    workbook_set_properties(workbook, properties);

    worksheet_set_column(worksheet, 0, 0, 70, NULL);
    worksheet_write_string(worksheet, CELL("A1"), "Select 'Office Button -> Prepare -> Properties' to see the file properties." , NULL);

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

    return workbook_close(workbook);
}
