/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/relationships.h"

// Test assembling a complete Relationships file.
CTEST(relationships, relationships01) {

    char* got;
    char exp[] =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
          "<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet\" Target=\"worksheets/sheet1.xml\"/>"
          "<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>"
          "<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>"
          "<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings\" Target=\"sharedStrings.xml\"/>"
          "<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/calcChain\" Target=\"calcChain.xml\"/>"
        "</Relationships>";

    FILE* testfile = tmpfile();

    lxw_relationships *rels = _new_relationships();
    rels->file = testfile;

    _add_document_relationship(rels, "/worksheet",     "worksheets/sheet1.xml");
    _add_document_relationship(rels, "/theme",         "theme/theme1.xml");
    _add_document_relationship(rels, "/styles",        "styles.xml");
    _add_document_relationship(rels, "/sharedStrings", "sharedStrings.xml");
    _add_document_relationship(rels, "/calcChain",     "calcChain.xml");


    _relationships_assemble_xml_file(rels);

    RUN_XLSX_STREQ_SHORT(exp, got);

    _free_relationships(rels);
}

// Test assembling a complete Relationships file.
CTEST(relationships, relationships02) {

    char* got;
    char exp[] =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
          "<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"www.foo.com\" TargetMode=\"External\"/>"
          "<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"link00.xlsx\" TargetMode=\"External\"/>"
        "</Relationships>";

    FILE* testfile = tmpfile();

    lxw_relationships *rels = _new_relationships();
    rels->file = testfile;

    _add_worksheet_relationship(rels, "/hyperlink", "www.foo.com", "External");
    _add_worksheet_relationship(rels, "/hyperlink", "link00.xlsx", "External");

    _relationships_assemble_xml_file(rels);

    RUN_XLSX_STREQ_SHORT(exp, got);

    _free_relationships(rels);
}
