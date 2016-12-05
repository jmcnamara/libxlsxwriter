add_library(ioapi ioapi.c)
add_library(zip zip.c)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ZLIB_INCLUDE_DIR})

install(TARGETS ioapi zip
    INCLUDES DESTINATION include
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)
