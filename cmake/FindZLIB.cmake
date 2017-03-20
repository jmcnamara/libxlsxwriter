#  :copyright: (c) 2017 Alex Huszagh.
#  :license: FreeBSD, see LICENSE.txt for more details.

# FindZLIB
# --------
#
# Find ZLIB include dirs and libraries
#
# Use this module by invoking find_package with the form::
#
#   find_package(ZLIB
#     [version] [EXACT]      # Minimum or EXACT version e.g. 1.0.6
#     [REQUIRED]             # Fail with error if ZLIB is not found
#     )
#
# You may also set `ZLIB_USE_STATIC_LIBS` to prefer static libraries
# to shared ones.
#
# If found, `ZLIB_FOUND` will be set to true, and `ZLIB_LIBRARIES`
# and `ZLIB_INCLUDE_DIRS` will both be set.
#
# You may optionally set `ZLIB_ROOT` to specify a custom root directory
# for the ZLIB installation.
#

include(CheckCXXSourceCompiles)
include(FindPackage)

# PATHS
# -----

set(ZLIB_SEARCH_PATHS)

if(ZLIB_ROOT)
    list(APPEND ZLIB_SEARCH_PATHS ${ZLIB_ROOT})
endif()

if(WIN32)
    list(APPEND ZLIB_SEARCH_PATHS
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GnuWin32\\Zlib;InstallPath]"
        "$ENV{PROGRAMFILES}/zlib"
    )
endif()

unset(ZLIB_SYSTEM_ROOT)
unset(ZLIB_CUSTOM_ROOT)
unset(ZLIB_SEARCH_HKEY)

# FIND
# ----

# INCLUDE DIRECTORY
SetSuffixes(ZLIB)
foreach(search ${ZLIB_SEARCH_PATHS})
    FIND_PATH(ZLIB_INCLUDE_DIR
        zlib.h ${search}
        PATH_SUFFIXES include
    )
endforeach(search)

if(NOT ZLIB_INCLUDE_DIR)
    FIND_PATH(ZLIB_INCLUDE_DIR zlib.h PATH_SUFFIXES include)
endif()

# LIBRARY PATHS
set(ZLIB_LIBRARY_NAMES z libz zlib zlib1)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND ZLIB_LIBRARY_NAMES zlibd zlibd1)
endif()

foreach(search ${ZLIB_SEARCH_PATHS})
    FIND_LIBRARY(ZLIB_LIBRARY
        NAMES ${ZLIB_LIBRARY_NAMES}
        ${search}
        PATH_SUFFIXES lib
    )
endforeach(search)

if(NOT ZLIB_LIBRARY)
    FIND_LIBRARY(ZLIB_LIBRARY NAMES ${ZLIB_LIBRARY_NAMES} PATH_SUFFIXES lib)
endif()

set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})
set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})

CheckFound(ZLIB)
FindStaticLibs(ZLIB)

# VERSION
# -------

if(ZLIB_FOUND)
    file(STRINGS "${ZLIB_INCLUDE_DIRS}/zlib.h" ZLIB_VERSION_CONTENTS REGEX "#define ZLIB_VERSION \"[0-9]+\\.[0-9]+\\.[0-9]+\"")
    string(REGEX REPLACE ".*ZLIB_VERSION \"([0-9]+)\\.[0-9]+\\.[0-9]+\"" "\\1" ZLIB_VERSION_MAJOR "${ZLIB_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*ZLIB_VERSION \"[0-9]+\\.([0-9]+)\\.[0-9]+\"" "\\1" ZLIB_VERSION_MINOR "${ZLIB_VERSION_CONTENTS}")
    string(REGEX REPLACE ".*ZLIB_VERSION \"[0-9]+\\.[0-9]+\\.([0-9]+)\"" "\\1" ZLIB_VERSION_PATCH "${ZLIB_VERSION_CONTENTS}")

    set(ZLIB_VERSION_STRING "${ZLIB_VERSION_MAJOR}.${ZLIB_VERSION_MINOR}.${ZLIB_VERSION_PATCH}")
    set(ZLIB_VERSION ${ZLIB_VERSION_STRING})

    MatchVersion(ZLIB)
endif()

# COMPILATION
# -----------

set(ZLIB_CODE "
#include <zlib.h>
int main(void)
{
    const char *version;
    version  = zlibVersion();

    return 0;
}
"
)

if(ZLIB_FOUND)
    CheckCompiles(ZLIB)
endif()
RequiredPackageFound(ZLIB)
