/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014-2019, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/content_types.h"

// Test assembling a complete ContentTypes file.
CTEST(content_types, content_types01) {

    char* got;
    char exp[] =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">"

          "<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>"
          "<Default Extension=\"xml\" ContentType=\"application/xml\"/>"
          "<Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/>"

          "<Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\"/>"
          "<Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\"/>"
          "<Override PartName=\"/xl/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml\"/>"
          "<Override PartName=\"/xl/theme/theme1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/>"
          "<Override PartName=\"/xl/workbook.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml\"/>"
          "<Override PartName=\"/xl/worksheets/sheet1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml\"/>"
          "<Override PartName=\"/xl/sharedStrings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml\"/>"
          "<Override PartName=\"/xl/calcChain.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.calcChain+xml\"/>"
        "</Types>";

    FILE* testfile = lxw_tmpfile(NULL);

    lxw_content_types *content_types = lxw_content_types_new();
    content_types->file = testfile;

    lxw_ct_add_override(content_types, "/xl/workbook.xml",
                        LXW_APP_DOCUMENT "spreadsheetml.sheet.main+xml");
    lxw_ct_add_worksheet_name(content_types, "/xl/worksheets/sheet1.xml");
    lxw_ct_add_default(content_types, "jpeg", "image/jpeg");
    lxw_ct_add_shared_strings(content_types);
    lxw_ct_add_calc_chain(content_types);

    lxw_content_types_assemble_xml_file(content_types);

    RUN_XLSX_STREQ_SHORT(exp, got);

    lxw_content_types_free(content_types);
}
