#  :copyright: 2014-2025. John McNamara, Alex Huszagh.
#  :license: SPDX-License-Identifier: BSD-2-Clause

# CMake file for libxlsxwriter
# ============================
#
# To build libxlsxwriter with CMake move to a custom directory, ideally outside
# the root of the source tree, and type `cmake $LXW_SOURCE_LOCATION $FLAGS`,
# where `LXW_SOURCE_LOCATION` is the path to the libxlsxwriter project, and
# `FLAGS` are custom flags to pass to the compiler.
#
# For example, in the project directory, you can build libxlsxwriter as follows:
#
# ```
#     mkdir build
#     cd build
#
#     cmake .. -DCMAKE_BUILD_TYPE=Release
#     cmake --build . --config Release
#
# ```
#
# Or to build and run the tests (this can take several minutes):
#
# ```
#     mkdir build
#     cd build
#
#     cmake .. -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release
#     cmake --build . --config Release
#     ctest -C Release -V
#     cmake --build . --config Release --target install
# ```
#
# If the tests run successfully, you can install the library as follows:
#
# ```
#     cmake --build . --config Release --target install
# ```
#
# The libxlsxwriter CMake options are shown below.

# ---------------------
# Project configuration
# ---------------------
cmake_minimum_required(VERSION 3.16)

set(XLSX_PROJECT_NAME
    "xlsxwriter"
    CACHE STRING
    "Libxlsxwriter is a C library for creating new Excel XLSX files"
)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
project(${XLSX_PROJECT_NAME} C)
enable_testing()

# -------------------
# Compilation options
# -------------------

# `USE_SYSTEM_MINIZIP`
#
# Libxlsxwriter uses the `minizip` component of `zlib`to create the xlsx zip
# file container. The vendored source files for `minizip` are included in the
# src tree of libxlsxwriter and are statically linked by default.
#
# You can use this option if you have the `minizip` library already installed on
# your system and prefer to dynamically link against it.
#
# To enable this option pass `-DUSE_SYSTEM_MINIZIP=ON` during configuration.
option(
    USE_SYSTEM_MINIZIP
    "Use system minizip library instead of the vendored copy"
    OFF
)

# `USE_STANDARD_TMPFILE`
#
# Uses the standard library `tmpfile` function to handle temp files instead of
# the vendored `tmpfileplus` function. Turning this off eliminates a dependency.
# This option is not recommended for Windows as it doesn't support a
# configurable location and the default location may be in a privileged
# directory.
#
# To enable this option pass `-DUSE_STANDARD_TMPFILE=ON` during configuration.
option(
    USE_STANDARD_TMPFILE
    "Use the C standard library tmpfile() instead of tmpfileplus"
    OFF
)

# `USE_OPENSSL_MD5`
#
# Uses OpenSSL to provide a MD5 digest of image files in order to avoid storing
# duplicates instead of the vendored OpenWall MD5 functions. This will link
# against libcrypto for MD5 support rather than using the local MD5 support.
#
# To enable this option pass `-DUSE_OPENSSL_MD5=ON` during configuration.
option(
    USE_OPENSSL_MD5
    "Build libxlsxwriter with the OpenSSL MD5 support instead of built in version"
    OFF
)

# `USE_NO_MD5`
#
# Compile without third party MD5 support. This will turn off the functionality
# of avoiding duplicate image files in the output xlsx file. This can reduce the
# executable size slightly if you aren't using images.
#
# To enable this option pass `-DUSE_NO_MD5=ON` during configuration.
option(
    USE_NO_MD5
    "Build libxlsxwriter without MD5 support for eliminating duplicate images"
    OFF
)

# `USE_DTOA_LIBRARY`
#
# Use the third party Milo Yip DTOA library to handle string formatting of
# doubles. This is used to avoid issues with double formatting in different
# locales and gives better performance with numeric data.
#
# To enable this library, pass `-DUSE_DTOA_LIBRARY=ON` during configuration.
option(
    USE_DTOA_LIBRARY
    "Use the Milo Yip DTOA library to handle string formatting of doubles."
    OFF
)

# `USE_MEM_FILE`
#
# Use in memory files instead of temp files using the
# `fmemopen()`/`open_memstream()` functions. This option isn't available with
# MSVC.
#
# To enable this option pass `-DUSE_MEM_FILE=ON` during configuration.
if(NOT MSVC)
    option(
        USE_MEM_FILE
        "Use fmemopen()/open_memstream() in place of temporary files"
        OFF
    )
endif()

# `BUILD_TESTS`
#
# Compile the unit and function tests for libxlsxwriter. This functional tests
# require the Python pytest library. The tests can be run with `ctest` after
# compilation. The uint tests do not work with MSVC and are turned off for that
# platform. Note, the tests can take a while to compile and run.
#
# To enable this option pass `-DBUILD_TESTS=ON` during configuration.
option(
    BUILD_TESTS
    "Build the libxlsxwriter unit and functional tests (requires pytest)"
    OFF
)

# TODO LXW_TARGET_BIG_ENDIAN

# `BUILD_EXAMPLES`
#
# Compile the libxlsxwriter example programs.
#
# To enable this option pass `-DBUILD_EXAMPLES=ON` during configuration.
option(BUILD_EXAMPLES "Build libxlsxwriter examples" OFF)

# `BUILD_FUZZERS`
#
# Compile the fuzzer harnesses.
#
# To enable this option pass `-DBUILD_FUZZERS=ON` during configuration.
option(BUILD_FUZZERS "Build harness(es) for fuzzing" OFF)

# `IOAPI_NO_64`
#
# Turn off `IOAPI_NO_64` support in minizip ioapi.c.
#
# To enable this option pass `-DIOAPI_NO_64=ON` during configuration.
option(IOAPI_NO_64 "Disable 64-bit filesystem support with minizip" OFF)

# `USE_STATIC_MSVC_RUNTIME`
#
# Compile as a static library with the static MSVC runtime library.
#
# To enable this option pass `-DUSE_STATIC_MSVC_RUNTIME=ON` during configuration.
if(MSVC)
    option(USE_STATIC_MSVC_RUNTIME "Use the static runtime library" OFF)
endif()

# `ZLIB_ROOT`
#
# The `ZLIB `root directory can be specified either through an environment
# variable (`export ZLIB_ROOT=/usr/include`) or using a flag with CMake
# (`-DZLIB_ROOT:STRING=/usr/include`). This sets the preferred search path for
# the ZLIB installation.
set(ZLIB_ROOT "" CACHE STRING "Optional root for the ZLIB installation")
if(DEFINED ENV{ZLIB_ROOT})
    set(ZLIB_ROOT $ENV{ZLIB_ROOT})
endif()

# -------------------------
# Configure the compilation
# -------------------------

if(USE_SYSTEM_MINIZIP)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS USE_SYSTEM_MINIZIP)
endif()

if(USE_STANDARD_TMPFILE)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS USE_STANDARD_TMPFILE)
endif()

if(NOT USE_OPENSSL_MD5 AND USE_NO_MD5)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS USE_NO_MD5)
endif()

if(USE_OPENSSL_MD5)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS USE_OPENSSL_MD5)
    if(NOT MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
    endif()
endif()

if(USE_MEM_FILE OR USE_FMEMOPEN)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS USE_FMEMOPEN)
endif()

if(USE_DTOA_LIBRARY)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS USE_DTOA_LIBRARY)
endif()

if(IOAPI_NO_64)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS IOAPI_NO_64=1)
endif()

if(NOT BUILD_SHARED_LIBS)
    if(UNIX)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    elseif(MINGW OR MSYS)
        set(CMAKE_C_FLAGS
            "${CMAKE_C_FLAGS} -static -static-libgcc -Wno-char-subscripts -Wno-long-long"
        )
        list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS USE_FILE32API)
    elseif(MSVC)
        set(CMAKE_C_FLAGS_DEBUG
            "${CMAKE_C_FLAGS_DEBUG} /Fd\"${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb\""
        )
        set(CMAKE_C_FLAGS_RELEASE
            "${CMAKE_C_FLAGS_RELEASE} /Ox /Zi /Fd\"${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb\""
        )
        set(CMAKE_C_FLAGS_MINSIZEREL
            "${CMAKE_C_FLAGS_MINSIZEREL} /Zi /Fd\"${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb\""
        )
        set(CMAKE_C_FLAGS_RELWITHDEBINFO
            "${CMAKE_C_FLAGS_RELWITHDEBINFO} /Fd\"${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb\""
        )
    endif()
endif()

if(MSVC AND USE_STATIC_MSVC_RUNTIME)
    foreach(
        flag_var
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL
        CMAKE_C_FLAGS_RELWITHDEBINFO
    )
        if(${flag_var} MATCHES "/MD")
            string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
endif()

# -----------------------------
# Create the xlsxwriter.pc file
# -----------------------------
set(PREFIX ${CMAKE_INSTALL_PREFIX})

file(READ "include/xlsxwriter.h" ver)
string(REGEX MATCH "LXW_VERSION \"([^\"]+)\"" _ ${ver})
set(VERSION ${CMAKE_MATCH_1})
string(REGEX MATCH "LXW_SOVERSION \"([^\"]+)\"" _ ${ver})
set(SOVERSION ${CMAKE_MATCH_1})

set(ENABLED_OPTIONS "zlib")
if(USE_SYSTEM_MINIZIP)
    string(APPEND ENABLED_OPTIONS " minizip")
endif()
if(USE_OPENSSL_MD5)
    string(APPEND ENABLED_OPTIONS " libcrypto")
endif()

# Expand out the xlsxwriter.pc file.
configure_file(dev/release/pkg-config.txt xlsxwriter.pc @ONLY)

# ----------------
# Set the includes
# ----------------
enable_language(CXX)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set the zlib includes.
find_package(ZLIB "1.2.8" REQUIRED)
list(APPEND LXW_PRIVATE_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})

# Set the minizip includes.
if(USE_SYSTEM_MINIZIP)
    if(MSVC)
        find_package(MINIZIP NAMES unofficial-minizip REQUIRED)
        set(MINIZIP_LIBRARIES unofficial::minizip::minizip)
    else()
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(MINIZIP minizip)
        list(APPEND LXW_PRIVATE_INCLUDE_DIRS ${MINIZIP_INCLUDE_DIRS}/..)
    endif()
endif()

# Set the openssl includes.
if(USE_OPENSSL_MD5)
    find_package(OpenSSL REQUIRED)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

# ----------------------------
# Set the library dependencies
# ----------------------------
list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS NOCRYPT NOUNCRYPT)

# Ensure CRT Secure warnings are disabled.
if(MSVC)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
endif()

# Ensure "TESTING" macro is defined if building tests.
if(BUILD_TESTS)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS TESTING)
endif()

# Define "LXW_BIG_ENDIAN" macro on big-endian architectures.
include(TestBigEndian)
test_big_endian(LXW_TARGET_BIG_ENDIAN)
if(LXW_TARGET_BIG_ENDIAN)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS LXW_BIG_ENDIAN)
endif()

# Set the source files.
file(GLOB LXW_SOURCES src/*.c)
file(GLOB_RECURSE LXW_HEADERS RELATIVE include *.h)

# If not using the system minizip, add the vendored minizip source files.
if(NOT USE_SYSTEM_MINIZIP)
    list(
        APPEND
        LXW_SOURCES
        third_party/minizip/ioapi.c
        third_party/minizip/zip.c
    )
    if(MSVC)
        list(APPEND LXW_SOURCES third_party/minizip/iowin32.c)
    endif()
endif()

# If not using the system tmpfile, add the vendored tmpfileplus files.
if(NOT USE_STANDARD_TMPFILE)
    list(APPEND LXW_SOURCES third_party/tmpfileplus/tmpfileplus.c)
endif()

if(NOT USE_OPENSSL_MD5 AND NOT USE_NO_MD5)
    list(APPEND LXW_SOURCES third_party/md5/md5.c)
endif()

if(USE_DTOA_LIBRARY)
    list(APPEND LXW_SOURCES third_party/dtoa/emyg_dtoa.c)
endif()

# Set project metadata.
set(LXW_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LXW_LIB_DIR "${LXW_PROJECT_DIR}/lib")
add_library(${PROJECT_NAME} "")
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${SOVERSION})
target_sources(${PROJECT_NAME} PRIVATE ${LXW_SOURCES} PUBLIC ${LXW_HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)

if(MINIZIP_LINK_LIBRARIES)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${MINIZIP_LINK_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${MINIZIP_LIBRARIES})
endif()

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ${LIB_CRYPTO} ${OPENSSL_CRYPTO_LIBRARY}
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE ${LXW_PRIVATE_COMPILE_DEFINITIONS}
)

# Ensure MSVC supports /utf-8.
if(MSVC AND MSVC_VERSION GREATER_EQUAL 1900)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()

if(WINDOWSSTORE)
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE -DIOWIN32_USING_WINRT_API
    )
endif()

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${LXW_PRIVATE_INCLUDE_DIRS}
    PUBLIC include include/xlsxwriter
)

# ------------------------
# Set up and run the tests
# ------------------------

# Macro for handling unit tests.
macro(create_test sources target)
    set(output_name xlsxwriter_${target})
    set(dependencies ${output_name})

    add_executable(${output_name} ${${sources}})
    target_link_libraries(${output_name} ${PROJECT_NAME})
    target_compile_definitions(${output_name} PRIVATE TESTING COLOR_OK)
    add_test(
        NAME ${output_name}
        COMMAND ${output_name}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endmacro(create_test)

file(GLOB LXW_UTILITY_SOURCES test/unit/utility/test*.c)
file(GLOB LXW_XMLWRITER_SOURCES test/unit/xmlwriter/test*.c)
file(GLOB LXW_WORKSHEET_SOURCES test/unit/worksheet/test*.c)
file(GLOB LXW_SST_SOURCES test/unit/sst/test*.c)
file(GLOB LXW_WORKBOOK_SOURCES test/unit/workbook/test*.c)
file(GLOB LXW_APP_SOURCES test/unit/app/test*.c)
file(GLOB LXW_CONTENTTYPES_SOURCES test/unit/content_types/test*.c)
file(GLOB LXW_CORE_SOURCES test/unit/core/test*.c)
file(GLOB LXW_RELATIONSHIPS_SOURCES test/unit/relationships/test*.c)
file(GLOB LXW_FORMAT_SOURCES test/unit/format/test*.c)
file(GLOB LXW_STYLES_SOURCES test/unit/styles/test*.c)
file(GLOB LXW_DRAWING_SOURCES test/unit/drawing/test*.c)
file(GLOB LXW_CHART_SOURCES test/unit/chart/test*.c)
file(GLOB LXW_CUSTOM_SOURCES test/unit/custom/test*.c)
file(GLOB LXW_FUNCTIONAL_SOURCES test/functional/src/*.c)

if(NOT MSVC)
    # Skip unit tests on Windows since ctest.h doesn't support it.
    set(LXW_UNIT_SOURCES
        test/unit/test_all.c
        ${LXW_UTILITY_SOURCES}
        ${LXW_XMLWRITER_SOURCES}
        ${LXW_WORKSHEET_SOURCES}
        ${LXW_SST_SOURCES}
        ${LXW_WORKBOOK_SOURCES}
        ${LXW_APP_SOURCES}
        ${LXW_CONTENTTYPES_SOURCES}
        ${LXW_CORE_SOURCES}
        ${LXW_RELATIONSHIPS_SOURCES}
        ${LXW_FORMAT_SOURCES}
        ${LXW_STYLES_SOURCES}
        ${LXW_DRAWING_SOURCES}
        ${LXW_CHART_SOURCES}
        ${LXW_CUSTOM_SOURCES}
    )
else()
    set(LXW_UNIT_SOURCES test/cpp/test_compilation.cpp)
endif()

if(BUILD_TESTS)
    # Unit tests.
    create_test(LXW_UNIT_SOURCES unit)

    # Functional tests.
    find_package(Python COMPONENTS Interpreter REQUIRED)
    find_program(Pytest_EXECUTABLE NAMES pytest)

    if(NOT Pytest_EXECUTABLE)
        message(
            "Please install the Python pytest library to run the functional tests:"
        )
        message("    pip install pytest\n")
    endif()

    foreach(source ${LXW_FUNCTIONAL_SOURCES})
        get_filename_component(basename ${source} NAME_WE)
        add_executable(${basename} ${source})
        target_link_libraries(${basename} xlsxwriter)
        set_target_properties(
            ${basename}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY "test/functional/src"
        )
    endforeach(source)

    add_custom_command(
        TARGET xlsxwriter_unit
        POST_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/test/functional test/functional
    )

    if(USE_NO_MD5)
        add_test(
            NAME functional
            COMMAND pytest -v test/functional -m "not skipif"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    else()
        add_test(
            NAME functional
            COMMAND pytest -v test/functional
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    endif()
endif()

# ------------------------------------
# Compile and run the example programs
# ------------------------------------

file(GLOB LXW_EXAMPLE_SOURCES examples/*.c)

if(BUILD_EXAMPLES)
    foreach(source ${LXW_EXAMPLE_SOURCES})
        get_filename_component(basename ${source} NAME_WE)
        add_executable(${basename} ${source})
        target_link_libraries(${basename} ${PROJECT_NAME})
        set_target_properties(
            ${basename}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY "examples"
        )
    endforeach(source)
endif()

# -----------------
# Fuzzing harnesses
# -----------------
if(BUILD_FUZZERS AND DEFINED ENV{LIB_FUZZING_ENGINE})
    add_subdirectory(dev/fuzzing)
endif()

# -------------------
# Install the library
# -------------------
include(GNUInstallDirs)

install(
    TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES include/xlsxwriter.h DESTINATION include)

install(
    DIRECTORY include/xlsxwriter
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
    PATTERN "*.h"
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/xlsxwriter.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
