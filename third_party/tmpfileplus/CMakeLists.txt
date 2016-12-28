add_library(tmpfileplus tmpfileplus.c)

install(TARGETS tmpfileplus
    INCLUDES DESTINATION include
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)
