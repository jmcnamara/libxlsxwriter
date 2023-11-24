# Utilized by OSSFuzz to build the harness(es) for continuous fuzz-testing
# OSSFuzz defines the following environment variables, that this target relies upon:
# CXX, CFLAGS, LIB_FUZZING_ENGINE, OUT

add_definitions(-DNDEBUG)  # Do not want assertions

if (DEFINED ENV{CFLAGS})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} $ENV{CFLAGS}")
endif()
if (DEFINED ENV{CXXFLAGS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} $ENV{CXXFLAGS}")
endif()


add_executable(xlsx_fuzzer xlsx_fuzzer.cpp)
target_link_libraries(xlsx_fuzzer PRIVATE ${PROJECT_NAME} $ENV{LIB_FUZZING_ENGINE})
target_compile_features(xlsx_fuzzer PRIVATE cxx_std_17)

if (DEFINED ENV{OUT})
    install(TARGETS xlsx_fuzzer DESTINATION $ENV{OUT})
else ()
    message(WARNING "Cannot install if $OUT is not defined!")
endif ()