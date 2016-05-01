/*
 * Tests for the lib_xlsx_writer library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "xlsxwriter/chart.h"

// Test assembling a complete Chart file.
CTEST(chart, chart01) {

    lxw_chart_series series1 = {};
    lxw_chart_series series2 = {};

    char* got;
    char exp[] =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<c:chartSpace xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">"
          "<c:lang val=\"en-US\"/>"
          "<c:chart>"
            "<c:plotArea>"
              "<c:layout/>"
              "<c:barChart>"
                "<c:barDir val=\"bar\"/>"
                "<c:grouping val=\"clustered\"/>"
                "<c:ser>"
                  "<c:idx val=\"0\"/>"
                  "<c:order val=\"0\"/>"
                  "<c:val>"
                    "<c:numRef>"
                      "<c:f>Sheet1!$A$1:$A$5</c:f>"
                    "</c:numRef>"
                  "</c:val>"
                "</c:ser>"
                "<c:ser>"
                  "<c:idx val=\"1\"/>"
                  "<c:order val=\"1\"/>"
                  "<c:val>"
                    "<c:numRef>"
                      "<c:f>Sheet1!$B$1:$B$5</c:f>"
                    "</c:numRef>"
                  "</c:val>"
                "</c:ser>"
                "<c:axId val=\"53850880\"/>"
                "<c:axId val=\"82642816\"/>"
              "</c:barChart>"
              "<c:catAx>"
                "<c:axId val=\"53850880\"/>"
                "<c:scaling>"
                  "<c:orientation val=\"minMax\"/>"
                "</c:scaling>"
                "<c:axPos val=\"l\"/>"
                "<c:tickLblPos val=\"nextTo\"/>"
                "<c:crossAx val=\"82642816\"/>"
                "<c:crosses val=\"autoZero\"/>"
                "<c:auto val=\"1\"/>"
                "<c:lblAlgn val=\"ctr\"/>"
                "<c:lblOffset val=\"100\"/>"
              "</c:catAx>"
              "<c:valAx>"
                "<c:axId val=\"82642816\"/>"
                "<c:scaling>"
                  "<c:orientation val=\"minMax\"/>"
                "</c:scaling>"
                "<c:axPos val=\"b\"/>"
                "<c:majorGridlines/>"
                "<c:numFmt formatCode=\"General\" sourceLinked=\"1\"/>"
                "<c:tickLblPos val=\"nextTo\"/>"
                "<c:crossAx val=\"53850880\"/>"
                "<c:crosses val=\"autoZero\"/>"
                "<c:crossBetween val=\"between\"/>"
              "</c:valAx>"
            "</c:plotArea>"
            "<c:legend>"
              "<c:legendPos val=\"r\"/>"
              "<c:layout/>"
            "</c:legend>"
            "<c:plotVisOnly val=\"1\"/>"
          "</c:chart>"
          "<c:printSettings>"
            "<c:headerFooter/>"
            "<c:pageMargins b=\"0.75\" l=\"0.7\" r=\"0.7\" t=\"0.75\" header=\"0.3\" footer=\"0.3\"/>"
            "<c:pageSetup/>"
          "</c:printSettings>"
        "</c:chartSpace>";

    FILE* testfile = tmpfile();

    lxw_chart *chart = lxw_chart_new(NULL);
    chart->file = testfile;

    /* Set the chart axis ids for testing. */
    chart->axis_id_1 = 53850880;
    chart->axis_id_2 = 82642816;


    series1.values.range = strdup("Sheet1!$A$1:$A$5");
    series2.values.range = strdup("Sheet1!$B$1:$B$5");

    series1.values.sheetname = NULL;
    series2.values.sheetname = NULL;

    chart_add_series(chart, &series1);
    chart_add_series(chart, &series2);

    lxw_chart_assemble_xml_file(chart);

    RUN_XLSX_STREQ_SHORT(exp, got);

    lxw_chart_free(chart);
}

