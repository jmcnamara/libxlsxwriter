#  :copyright: (c) 2017 Alex Huszagh.
#  :license: FreeBSD, see LICENSE.txt for more details.

# FindMINIZIP
# -----------
#
# Find MINIZIP include dirs and libraries
#
# Use this module by invoking find_package with the form::
#
#   find_package(MINIZIP
#     [version] [EXACT]      # Minimum or EXACT version e.g. 1.0.6
#     [REQUIRED]             # Fail with error if MINIZIP is not found
#     )
#
# You may also set `MINIZIP_USE_STATIC_LIBS` to prefer static libraries
# to shared ones.
#
# If found, `MINIZIP_FOUND` will be set to true, and `MINIZIP_LIBRARIES`
# and `MINIZIP_INCLUDE_DIRS` will both be set.
#
# You may optionally set `MINIZIP_ROOT` to specify a custom root directory
# for the MINIZIP installation.
#

include(CheckCXXSourceCompiles)
include(FindPackage)

# PATHS
# -----

set(MINIZIP_SEARCH_PATHS)

if(MINIZIP_ROOT)
    list(APPEND MINIZIP_SEARCH_PATHS ${MINIZIP_ROOT})
endif()

if(WIN32)
    list(APPEND ZLIB_SEARCH_PATHS
        "$ENV{PROGRAMFILES}/minizip"
    )
endif()

unset(MINIZIP_SYSTEM_ROOT)
unset(MINIZIP_CUSTOM_ROOT)
unset(MINIZIP_SEARCH_HKEY)

# FIND
# ----

# INCLUDE DIRECTORY
SetSuffixes(MINIZIP)
foreach(search ${MINIZIP_SEARCH_PATHS})
    FIND_PATH(MINIZIP_INCLUDE_DIR
        NAMES minizip/zip.h
        PATHS ${search}
        PATH_SUFFIXES include
    )
endforeach(search)

if(NOT MINIZIP_INCLUDE_DIR)
    FIND_PATH(MINIZIP_INCLUDE_DIR minizip/zip.h PATH_SUFFIXES include)
endif()

# LIBRARY PATHS
set(MINIZIP_LIBRARY_NAMES minizip)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND MINIZIP_LIBRARY_NAMES minizipd)
endif()

foreach(search ${MINIZIP_SEARCH_PATHS})
    FIND_LIBRARY(MINIZIP_LIBRARY
        NAMES ${MINIZIP_LIBRARY_NAMES}
        PATHS ${search}
        PATH_SUFFIXES lib
    )
endforeach(search)

if(NOT MINIZIP_LIBRARY)
    FIND_LIBRARY(MINIZIP_LIBRARY NAMES ${MINIZIP_LIBRARY_NAMES} PATH_SUFFIXES lib)
endif()

set(MINIZIP_INCLUDE_DIRS ${MINIZIP_INCLUDE_DIR})
set(MINIZIP_LIBRARIES ${MINIZIP_LIBRARY})

CheckFound(MINIZIP)
FindStaticLibs(MINIZIP)

# VERSION
# -------

if(MINIZIP_FOUND)
   file(STRINGS "${MINIZIP_INCLUDE_DIRS}/zlib.h" MINIZIP_VERSION_CONTENTS REGEX "Version [0-9]+\\.[0-9]+(\\.[0-9]+)?")
    string(REGEX REPLACE ".*Version ([0-9]+)\\.[0-9]+" "\\1" MINIZIP_VERSION_MAJOR "${MINIZIP_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*Version [0-9]+\\.([0-9]+)" "\\1" MINIZIP_VERSION_MINOR "${MINIZIP_VERSION_CONTENTS}")
    set(MINIZIP_VERSION_PATCH 0)

    set(MINIZIP_VERSION_STRING "${MINIZIP_VERSION_MAJOR}.${MINIZIP_VERSION_MINOR}.${MINIZIP_VERSION_PATCH}")
    set(MINIZIP_VERSION ${MINIZIP_VERSION_STRING})

    MatchVersion(MINIZIP)
endif()

# COMPILATION
# -----------

set(MINIZIP_CODE "
#include <minizip/zip.h>
int main(void)
{
    zip_fileinfo zipfile_info;

    return 0;
}
"
)

if(MINIZIP_FOUND)
    CheckCompiles(MINIZIP)
endif()
RequiredPackageFound(MINIZIP)
