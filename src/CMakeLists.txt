set(xlsxwriter_srcs
    app.c
    chart.c
    content_types.c
    core.c
    custom.c
    drawing.c
    format.c
    hash_table.c
    packager.c
    relationships.c
    shared_strings.c
    styles.c
    theme.c
    utility.c
    workbook.c
    worksheet.c
    xmlwriter.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(xlsxwriter ${xlsxwriter_srcs})
add_library(xlsxwriter_test STATIC ${xlsxwriter_srcs})

target_compile_definitions(xlsxwriter_test
    PRIVATE -DTESTING)

install(TARGETS xlsxwriter
    EXPORT libxlsxwriter
    INCLUDES DESTINATION include
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)
