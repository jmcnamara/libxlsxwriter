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

SET(PROJECT_NAME "xlsxwriter" CACHE STRING "Optional project and binary name")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
project(${PROJECT_NAME} C)
enable_testing()

# OPTIONS
# -------

SET(ZLIB_ROOT "" CACHE STRING "Optional root for the ZLIB installation")

option(BUILD_TESTS "Build libxlsxwriter tests" OFF)
option(BUILD_EXAMPLES "Build libxlsxwriter examples" OFF)
option(USE_SYSTEM_MINIZIP "Use system minizip installation" OFF)
option(USE_STANDARD_TMPFILE "Use the C standard library's tmpfile()" OFF)
option(IOAPI_NO_64 "Disable 64-bit filesystem support" OFF)
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

if(NOT BUILD_SHARED_LIBS)
    if(UNIX)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    elseif(MINGW OR MSYS)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -static-libgcc -Wno-char-subscripts -Wno-long-long")
        list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS USE_FILE32API)
    elseif(MSVC)
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /O0 /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ox /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
    endif()
endif()

message(status "BEFORE: ${CMAKE_C_FLAGS_DEBUG}")

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

# INCLUDES
# --------

enable_language(CXX)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# ZLIB
find_package(ZLIB REQUIRED "1.0")
list(APPEND LXW_PRIVATE_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
message("zlib version: " ${ZLIB_VERSION})

# MINIZIP
if (USE_SYSTEM_MINIZIP)
    find_package(MINIZIP REQUIRED "1.0")
    list(APPEND LXW_PRIVATE_INCLUDE_DIRS ${MINIZIP_INCLUDE_DIRS})
endif()

# LIBRARY
# -------

list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS NOCRYPT NOUNCRYPT)

# Fix for modified zconf.h on Gentoo.
if(${CMAKE_HOST_SYSTEM} MATCHES gentoo)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS OF=_Z_OF)
endif()

# Ensure CRT Secure warnings are disabled
if(MSVC)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
endif()

# Ensure "TESTING" macro is defined if building tests
if(BUILD_TESTS)
    list(APPEND LXW_PRIVATE_COMPILE_DEFINITIONS TESTING)
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

set(LXW_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LXW_LIB_DIR "${LXW_PROJECT_DIR}/lib")
add_library(${PROJECT_NAME} "")
target_sources(${PROJECT_NAME}
    PRIVATE ${LXW_SOURCES}
    PUBLIC ${LXW_HEADERS}
)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${ZLIB_LIBRARIES} ${MINIZIP_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LXW_PRIVATE_COMPILE_DEFINITIONS})
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

if(BUILD_TESTS)
    # unit tests
    CreateTest(LXW_UNIT_SOURCES unit)

    # functional tests
    # WARNING: currently doesn't work, since the Python tests expect
    # in-source builds
    #find_program(PYTHON python)
    #foreach(source ${LXW_FUNCTIONAL_SOURCES})
    #    get_filename_component(basename ${source} NAME_WE)
    #    add_executable(${basename} ${source})
    #    target_link_libraries(${basename} xlsxwriter)
    #endforeach(source)
endif()

# EXAMPLES
# --------

file(GLOB LXW_EXAMPLE_SOURCES examples/*.c)

if(BUILD_EXAMPLES)
    foreach(source ${LXW_EXAMPLE_SOURCES})
        get_filename_component(basename ${source} NAME_WE)
        add_executable(${basename} ${source})
        target_link_libraries(${basename} ${PROJECT_NAME})
    endforeach(source)
endif()

# INSTALL
# -------

include(GNUInstallDirs)

if(MSVC)
    if(CMAKE_CL_64)
        set(MSVC_FOLDER_PREFIX x64)
    else()
        set(MSVC_FOLDER_PREFIX Win32)
    endif()

    install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION "lib/${MSVC_FOLDER_PREFIX}/\${CMAKE_INSTALL_CONFIG_NAME}"
        ARCHIVE DESTINATION "lib/${MSVC_FOLDER_PREFIX}/\${CMAKE_INSTALL_CONFIG_NAME}"
        RUNTIME DESTINATION "bin/${MSVC_FOLDER_PREFIX}/\${CMAKE_INSTALL_CONFIG_NAME}"
    )
    if (NOT BUILD_SHARED_LIBS)
        install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb
            DESTINATION "lib/${MSVC_FOLDER_PREFIX}/\${CMAKE_INSTALL_CONFIG_NAME}"
        )
    endif()

else(MSVC)
    install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif(MSVC)
install(FILES include/xlsxwriter.h DESTINATION include)
install(DIRECTORY include/xlsxwriter
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)
