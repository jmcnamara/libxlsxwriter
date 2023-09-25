#  :copyright: (c) 2017 Alex Huszagh.
#  :license: FreeBSD, see LICENSE.txt for more details.

# Description
# ===========
#
#   Use:
#       Move to a custom directory, ideally out of source, and
#       type `cmake $LXW_SOURCE $FLAGS`, where `LXW_SOURCE` is the
#       path to the libxlsxwriter project, and `FLAGS` are custom
#       flags to pass to the compiler.
#
#   Example:
#       For example, in the project directory, to build libxlsxwriter
#       and the unittests in release mode, type:
#           mkdir build && cd build
#           cmake .. -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release
#           cmake --build . --config Release
#           ctest -C Release -V
#           cmake --build . --config Release --target install
#
#       If using a Makefile generator, you may use the simpler
#           mkdir build && cd build
#           cmake .. -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release
#           make
#           make test
#           make install
#
#   Flags:
#       ZLIB_ROOT
#           The ZLIB root directory can be specified either through
#           an environment variable (`export ZLIB_ROOT=/usr/include`)
#           or using a flag with CMake (`-DZLIB_ROOT:STRING=/usr/include`).
#           This sets the preferred search path for the ZLIB installation.
#
#       BUILD_TESTS
#           Build unittests (default off). To build the unittests,
#           pass `-DBUILD_TESTS=ON` during configuration.
#
#       BUILD_EXAMPLES
#           Build example files (default off). To build the examples,
#           pass `-DBUILD_EXAMPLES=ON` during configuration.
#
#       USE_STANDARD_TMPFILE
#           Use the standard tmpfile() function (default off). To enable
#           the standard tmpfile, pass `-DUSE_STANDARD_TMPFILE=ON`
#           during configuration. This may produce bugs while cross-
#           compiling or using MinGW/MSYS.
#
#       USE_DTOA_LIBRARY
#           Use the third party emyg_dtoa() library (default off). The
#           emyg_dtoa() library is used to avoid sprintf double issues with
#           different locale settings. To enable this library, pass
#           `-DUSE_DTOA_LIBRARY=ON` during configuration.
#
#       USE_NO_MD5
#           Compile without third party MD5 support. This will turn off the
#           functionality of avoiding duplicate image files in the output xlsx
#           file. To enable this option pass `-DUSE_NO_MD5=ON` during
#           configuration.
#
#       USE_OPENSSL_MD5 Compile with OpenSSL MD5 support. This will link
#           against libcrypto for MD5 support rather than using the local MD5
#           support. MD5 support is required to avoid duplicate image files in
#           the output xlsx file. To enable this option pass
#           `-DUSE_OPENSSL_MD5=ON` during configuration.
#
#       USE_STATIC_MSVC_RUNTIME
#           Use the static msvc runtime library when compiling with msvc (default off)
#           To enable, pass `-DUSE_STATIC_MSVC_RUNTIME` during configuration.
#
#   Toolchains:
#       On multiarch Linux systems, which can build and run multiple
#       binary targets on the same system, we include an `i686-toolchain`
#       file to enable building i686 (x86 32-bit) targets on x86_64 systems.
#       To use the i686 toolchain, pass the `-DCMAKE_TOOLCHAIN_FILE` option
#       during CMake configuration. For example, from the build directory,
#       you would use:
#           cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/i686-toolchain.cmake
#
#   CMake Options:
#       CMake sets debug and release builds with the `CMAKE_BUILD_TYPE`
#       option, which can be set as a flag during configuration.
#       To build in release mode, pass `-DCMAKE_BUILD_TYPE=Release`
#       during configuration.
#
#       CMake sets the creation of static and shared libraries with the
#       `BUILD_SHARED_LIBS` option, which can be set as a flag during
#       configuration. To build a static library, pass
#       `-DBUILD_SHARED_LIBS=OFF` during configuration.
#
#   Generators:
#       CMake also supports custom build generators, such as MakeFiles,
#       Ninja, Visual Studio, and XCode. For example, to generate
#       a Visual Studio solution, configure with:
#           cmake .. -G "Visual Studio 14 2015 Win64"
#
#       For more information on using generators, see:
#           https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html
#

set(CMAKE_LEGACY_CYGWIN_WIN32 1)
if(MSVC)
    cmake_minimum_required(VERSION 3.4)
else()
    cmake_minimum_required(VERSION 3.1)
endif()

SET(XLSX_PROJECT_NAME "xlsxwriter" CACHE STRING "Optional project and binary name")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
project(${XLSX_PROJECT_NAME} C)
enable_testing()

# POLICY
# ------

# The use of the word ZLIB_ROOT should still work prior to "3.12.0",
# just it's been generalized for all packages now. Just set the policy
# to new, so we use it, and it will be used prior to 3.12 anyway.
if(${CMAKE_VERSION} VERSION_GREATER "3.12" OR ${CMAKE_VERSION} VERSION_EQUAL "3.12")
    cmake_policy(SET CMP0074 NEW)
endif()

# OPTIONS
# -------
SET(ZLIB_ROOT "" CACHE STRING "Optional root for the ZLIB installation")

option(BUILD_TESTS "Build libxlsxwriter tests" OFF)
option(BUILD_EXAMPLES "Build libxlsxwriter examples" OFF)
option(USE_SYSTEM_MINIZIP "Use system minizip installation" OFF)
option(USE_STANDARD_TMPFILE "Use the C standard library's tmpfile()" OFF)
option(USE_NO_MD5 "Build libxlsxwriter without third party MD5 lib" OFF)
option(USE_OPENSSL_MD5 "Build libxlsxwriter with the OpenSSL MD5 lib" OFF)
option(USE_MEM_FILE "Use fmemopen()/open_memstream() in place of temporary files" OFF)
option(IOAPI_NO_64 "Disable 64-bit filesystem support" OFF)
option(USE_DTOA_LIBRARY "Use the locale independent third party Milo Yip DTOA library" OFF)

if(MSVC)
    option(USE_STATIC_MSVC_RUNTIME "Use the static runtime library" OFF)
endif()

if(DEFINED ENV{${ZLIB_ROOT}})
    set(ZLIB_ROOT $ENV{ZLIB_ROOT})
endif()

if(IOAPI_NO_64)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS IOAPI_NO_64=1)
endif()

# CONFIGURATIONS
# --------------
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

if(NOT BUILD_SHARED_LIBS)
    if(UNIX)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    elseif(MINGW OR MSYS)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -static-libgcc -Wno-char-subscripts -Wno-long-long")
        list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS USE_FILE32API)
    elseif(MSVC)
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Fd\"${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb\"")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ox /Zi /Fd\"${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb\"")
        set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /Zi /Fd\"${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb\"")
        set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /Fd\"${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb\"")
    endif()
endif()

if(MSVC AND USE_STATIC_MSVC_RUNTIME)
    foreach(flag_var CMAKE_C_FLAGS
                     CMAKE_C_FLAGS_DEBUG
                     CMAKE_C_FLAGS_RELEASE
                     CMAKE_C_FLAGS_MINSIZEREL
                     CMAKE_C_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
endif()

# Configure pkg-config
file(READ "include/xlsxwriter.h" ver)

string(REGEX MATCH "LXW_VERSION \"([^\"]+)\"" _ ${ver})
set(VERSION ${CMAKE_MATCH_1})
string(REGEX MATCH "LXW_SOVERSION \"([^\"]+)\"" _ ${ver})
set(SOVERSION ${CMAKE_MATCH_1})
set(PREFIX ${CMAKE_INSTALL_PREFIX})

configure_file(dev/release/pkg-config.txt xlsxwriter.pc @ONLY)

# INCLUDES
# --------
enable_language(CXX)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# ZLIB
find_package(ZLIB "1.0" REQUIRED)
list(APPEND LXW_PRIVATE_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
message("zlib version: " ${ZLIB_VERSION})

# MINIZIP
if (USE_SYSTEM_MINIZIP)
    find_package(MINIZIP "1.0" REQUIRED)
    list(APPEND LXW_PRIVATE_INCLUDE_DIRS ${MINIZIP_INCLUDE_DIRS})
endif()

# LIBRARY
# -------
list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS NOCRYPT NOUNCRYPT)

# Ensure CRT Secure warnings are disabled
if(MSVC)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
endif()

# Ensure "TESTING" macro is defined if building tests
if(BUILD_TESTS)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS TESTING)
endif()

# Define "LXW_BIG_ENDIAN" macro on big-endian architectures
include(TestBigEndian)
TEST_BIG_ENDIAN(LXW_TARGET_BIG_ENDIAN)
if(LXW_TARGET_BIG_ENDIAN)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS LXW_BIG_ENDIAN)
endif()

file(GLOB LXW_SOURCES src/*.c)
file(GLOB_RECURSE LXW_HEADERS RELATIVE include *.h)

if(NOT USE_SYSTEM_MINIZIP)
    list(APPEND LXW_SOURCES third_party/minizip/ioapi.c third_party/minizip/zip.c)
    if(MSVC)
        list(APPEND LXW_SOURCES third_party/minizip/iowin32.c)
    endif()
endif()

if (NOT USE_STANDARD_TMPFILE)
    list(APPEND LXW_SOURCES third_party/tmpfileplus/tmpfileplus.c)
endif()

if(NOT USE_OPENSSL_MD5 AND NOT USE_NO_MD5)
    list(APPEND LXW_SOURCES third_party/md5/md5.c)
endif()

if(USE_OPENSSL_MD5)
    find_package(OpenSSL REQUIRED)
    if(OpenSSL_FOUND)
        include_directories(${OPENSSL_INCLUDE_DIR})
        message(STATUS "OpenSSL version: ${OPENSSL_VERSION}")
    endif()
endif()

if (USE_DTOA_LIBRARY)
    list(APPEND LXW_SOURCES third_party/dtoa/emyg_dtoa.c)
endif()

set(LXW_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LXW_LIB_DIR "${LXW_PROJECT_DIR}/lib")
add_library(${PROJECT_NAME} "")
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${SOVERSION})
target_sources(${PROJECT_NAME}
    PRIVATE ${LXW_SOURCES}
    PUBLIC ${LXW_HEADERS}
)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${ZLIB_LIBRARIES} ${MINIZIP_LIBRARIES} ${LIB_CRYPTO} ${OPENSSL_CRYPTO_LIBRARY})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LXW_PRIVATE_COMPILE_DEFINITIONS})

# /utf-8 needs VS2015 Update 2 or above.
# In CMake 3.7 and above, we can use (MSVC_VERSION GREATER_EQUAL 1900) here.
if(MSVC AND NOT (MSVC_VERSION LESS 1900))
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()

if (WINDOWSSTORE)
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DIOWIN32_USING_WINRT_API)
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE ${LXW_PRIVATE_INCLUDE_DIRS}
    PUBLIC include include/xlsxwriter
)

# TESTS
# -----

# Create test and runner.
#
# Args:
#   sources       Name of variable holding source files
#   target        Test name
#

macro(CreateTest sources target)
    set(output_name xlsxwriter_${target})
    set(dependencies ${output_name})

    add_executable(${output_name} ${${sources}})
    target_link_libraries(${output_name} ${PROJECT_NAME})
    target_compile_definitions(${output_name} PRIVATE TESTING COLOR_OK)
    add_test(NAME ${output_name}
        COMMAND ${output_name}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endmacro(CreateTest)

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
set(LXW_UNIT_SOURCES
    test/cpp/test_compilation.cpp
)
endif()

if(BUILD_TESTS)
    # unit tests
    CreateTest(LXW_UNIT_SOURCES unit)

    # functional tests
    find_package(Python COMPONENTS Interpreter REQUIRED)
    find_program(Pytest_EXECUTABLE NAMES pytest)

    if (NOT Pytest_EXECUTABLE)
        message("Please install the Python pytest library to run functional tests:")
        message("    pip install pytest\n")
    endif()

    foreach(source ${LXW_FUNCTIONAL_SOURCES})
       get_filename_component(basename ${source} NAME_WE)
       add_executable(${basename} ${source})
       target_link_libraries(${basename} xlsxwriter)
       set_target_properties(${basename} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "test/functional/src")
    endforeach(source)

    add_custom_command(TARGET xlsxwriter_unit POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/test/functional test/functional
    )

    if(USE_NO_MD5)
        add_test(NAME functional
            COMMAND pytest -v test/functional -m "not skipif"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    else()
        add_test(NAME functional
            COMMAND pytest -v test/functional
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    endif()

endif()

# EXAMPLES
# --------
file(GLOB LXW_EXAMPLE_SOURCES examples/*.c)

if(BUILD_EXAMPLES)
    foreach(source ${LXW_EXAMPLE_SOURCES})
        get_filename_component(basename ${source} NAME_WE)
        add_executable(${basename} ${source})
        target_link_libraries(${basename} ${PROJECT_NAME})
        set_target_properties(${basename} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "examples")
    endforeach(source)
endif()

# INSTALL
# -------
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES include/xlsxwriter.h DESTINATION include)
install(DIRECTORY include/xlsxwriter
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/xlsxwriter.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
