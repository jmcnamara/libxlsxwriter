/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */
#ifdef _WIN32
#define strdup _strdup
#endif

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

    free((void *)properties->title);
    free((void *)properties->subject);
    free((void *)properties->author);
    free((void *)properties->manager);
    free((void *)properties->company);
    free((void *)properties->category);
    free((void *)properties->keywords);
    free((void *)properties->comments);
    free((void *)properties->status);
    free(properties);

    return workbook_close(workbook);
}
