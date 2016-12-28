set(test_sources
    app/test_app.c
    app/test_app_xml_declaration.c
    chart/test_chart.c
    chart/test_chart_xml_declaration.c
    content_types/test_content_types.c
    content_types/test_content_types_write_default.c
    content_types/test_content_types_write_override.c
    content_types/test_content_types_xml_declaration.c
    core/test_core.c
    core/test_core_xml_declaration.c
    custom/test_custom_xml_declaration.c
    drawing/test_drawing_image.c
    drawing/test_drawing_xml_declaration.c
    relationships/test_relationships.c
    relationships/test_relationships_xml_declaration.c
    sst/test_shared_strings.c
    sst/test_sst_xml_declaration.c
    styles/test_styles.c
    styles/test_styles_write_border.c
    styles/test_styles_write_borders.c
    styles/test_styles_write_cell_style.c
    styles/test_styles_write_cell_styles.c
    styles/test_styles_write_cell_style_xfs.c
    styles/test_styles_write_cell_xfs.c
    styles/test_styles_write_default_fill.c
    styles/test_styles_write_dxfs.c
    styles/test_styles_write_fills.c
    styles/test_styles_write_font.c
    styles/test_styles_write_font_color.c
    styles/test_styles_write_font_family.c
    styles/test_styles_write_font_name.c
    styles/test_styles_write_fonts.c
    styles/test_styles_write_font_scheme.c
    styles/test_styles_write_font_sz.c
    styles/test_styles_write_style_sheet.c
    styles/test_styles_write_style_xf.c
    styles/test_styles_write_table_styles.c
    styles/test_styles_write_xf.c
    styles/test_styles_xml_declaration.c
    utility/test_col_to_name.c
    utility/test_datetime_to_excel.c
    utility/test_name_to_col.c
    utility/test_quote_sheetname.c
    utility/test_row_colto_cell.c
    utility/test_rowcol_to_formula.c
    utility/test_rowcol_to_range.c
    utility/test_strerror.c
    utility/test_xl_name_to_row.c
    workbook/test_workbook.c
    workbook/test_workbook_get_worksheet_by_name.c
    workbook/test_workbook_write_book_views.c
    workbook/test_workbook_write_calc_pr.c
    workbook/test_workbook_write_defined_name.c
    workbook/test_workbook_write_defined_names.c
    workbook/test_workbook_write_file_version.c
    workbook/test_workbook_write_sheet.c
    workbook/test_workbook_write_sheets.c
    workbook/test_workbook_write_workbook.c
    workbook/test_workbook_write_workbook_pr.c
    workbook/test_workbook_write_workbook_view.c
    workbook/test_workbook_xml_declaration.c
    worksheet/test_worksheet_array_formula.c
    worksheet/test_worksheet.c
    worksheet/test_worksheet_merged_range.c
    worksheet/test_worksheet_spans.c
    worksheet/test_worksheet_write_col_info.c
    worksheet/test_worksheet_write_dimension.c
    worksheet/test_worksheet_write_header_footer.c
    worksheet/test_worksheet_write_page_margin.c
    worksheet/test_worksheet_write_page_margins.c
    worksheet/test_worksheet_write_page_setup.c
    worksheet/test_worksheet_write_print_options.c
    worksheet/test_worksheet_write_row.c
    worksheet/test_worksheet_write_sheet_data.c
    worksheet/test_worksheet_write_sheet_format_pr.c
    worksheet/test_worksheet_write_sheet_pr.c
    worksheet/test_worksheet_write_sheet_protection.c
    worksheet/test_worksheet_write_sheet_view.c
    worksheet/test_worksheet_write_sheet_views1.c
    worksheet/test_worksheet_write_sheet_views2.c
    worksheet/test_worksheet_write_sheet_views3.c
    worksheet/test_worksheet_write_sheet_views4.c
    worksheet/test_worksheet_write_sheet_views5.c
    worksheet/test_worksheet_write_sheet_views6.c
    worksheet/test_worksheet_write_sheet_views7.c
    worksheet/test_worksheet_write_sheet_views8.c
    worksheet/test_worksheet_write_sheet_views9.c
    worksheet/test_worksheet_xml_declaration.c
    xmlwriter/test_xmlwriter.c)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${ZLIB_INCLUDE_DIR})

add_definitions(-DTESTING -DCOLOR_OK)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin/test_unit)

add_executable(test_all test_all.c ${test_sources})

set(test_libs xlsxwriter_test zip ioapi ${ZLIB_LIBRARIES})
if (NOT USE_STANDARD_TMP_FILE)
    list(APPEND test_libs tmpfileplus)
endif()
target_link_libraries(test_all ${test_libs})

add_test(NAME unit_test_all COMMAND test_all)
