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

    lxw_workbook       *workbook   = workbook_new("test_properties02.xlsx");
    lxw_worksheet      *worksheet  = workbook_add_worksheet(workbook, NULL);
    lxw_doc_properties *properties = calloc(1, sizeof(lxw_doc_properties));

    properties->hyperlink_base = strdup("C:\\");

    workbook_set_properties(workbook, properties);

    (void)worksheet;

    free(properties->hyperlink_base);
    free(properties);

    return workbook_close(workbook);
}
