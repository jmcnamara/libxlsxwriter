cmake_minimum_required(VERSION 3.0.0)

project(libxlsxwriter)

# force out of source build so we don't trash the existing Makefiles
if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    message(FATAL_ERROR " This project requires an out of source build."
        " Create an empty directory and configure and build from there.")
endif()

# if we forgot to set the build type default to release
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release"
      CACHE STRING "One of: Debug Release RelWithDebInfo MinSizeRel"
      FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
        STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
message(STATUS "Configuring a ${CMAKE_BUILD_TYPE} build")

# locate dependencies
find_package(ZLIB REQUIRED)

set(USE_STANDARD_TMPFILE OFF CACHE BOOL
    "Enable/Disable the tmpfileplus third party library")

# for configuring shared libraries
set(CMAKE_MACOSX_RPATH 1)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")

# set up build dirs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set default compiler flags
if (NOT MSVC)
    set(tmp "-fPIC -Wall -Wextra")
    if (NOT ((APPLE) AND ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
        AND ("${CMAKE_BUILD_TYPE}" MATCHES "Release"))
        set(tmp "${tmp} -march=native -mtune=native")
    endif()
    set(CMAKE_C_FLAGS "${tmp}"
        CACHE STRING "libxlsxwriter compile flags"
        FORCE)
endif()

# for building tests and examples
function(add_simple_executable ex_name)
    string(CONCAT ex_source ${ex_name} ".c")
    add_executable(${ex_name} ${ex_source})
    set(ex_libs xlsxwriter zip ioapi ${ZLIB_LIBRARIES})
    if (NOT USE_STANDARD_TMP_FILE)
        list(APPEND ex_libs tmpfileplus)
    endif()
    target_link_libraries(${ex_name} ${ex_libs})
endfunction()

# process library components
add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(third_party)
add_subdirectory(examples)

# enable regression tests
set(BUILD_TESTING ON CACHE BOOL "Enable regresion tests")
if (BUILD_TESTING)
    include(CTest)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake"
        @ONLY)
    add_subdirectory(test)
endif()
