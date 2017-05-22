#  :copyright: (c) 2017 Alex Huszagh.
#  :license: FreeBSD, see LICENSE.txt for more details.

# FindPackage
# -----------
#
# Macros and functions to help find packages. Do not invoke this module
# directly, it merely provides library definitions to be invoked
# by other find utilities.

include(CheckCXXSourceCompiles)


# Return if the package name has previously been found
#
# Args:
#   packageName         Name of the package
#
# Example:
#   ReturnFound(Iconv)
#
macro(ReturnFound packageName)
    if(${packageName}_FOUND)
        return()
    endif()
endmacro(ReturnFound)


# Set the library extensions for a given package dependent on whether
# to search for static or dynamic libraries.
#
# Args:
#   packageName         Name of the package
#
# Example:
#   SetSuffixes(IConv)
#
macro(SetSuffixes packageName)
    if(${packageName}_USE_STATIC_LIBS)
        if(MSVC)
            set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
        else()
            set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
        endif()
    else()
        if(WIN32)
            set(CMAKE_FIND_LIBRARY_SUFFIXES ".dll.a" ".dll" ".lib" ".a")
        else()
            set(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
        endif()
    endif()
endmacro(SetSuffixes)


# Check if the package was found.
#
# Args:
#   packageName         Name of the package
#
# Example:
#   CheckFound(IConv)
#
macro(CheckFound packageName)
    if(${packageName}_INCLUDE_DIRS AND ${packageName}_LIBRARIES)
        set(${packageName}_FOUND TRUE)
    endif()
endmacro(CheckFound)


# Replace a dynamic library with a `.dll.a` extension with the corresponding
# library removing the `.dll`.
#
# Args:
#   libraryName         Variable name for path to found library
#
# Example:
#   ReplaceDynamic(/mingw64/lib/libiconv.dll.a)
#
macro(ReplaceDynamic libraryName)
    if(${libraryName} MATCHES ".dll.a")
        string(REPLACE ".dll.a" ".a" static ${${libraryName}})
        if(EXISTS ${static})
            set(${libraryName} ${static})
        endif()
    endif()
endmacro(ReplaceDynamic)


# Replace a dynamic libraries with the static variants, with integrity
# checks for the package.
#
# Args:
#   packageName         Name of the package
#
# Example:
#   FindStaticLibs(IConv)
#
macro(FindStaticLibs packageName)
    if(${packageName}_USE_STATIC_LIBS AND MSYS)
        # convert `.dll.a` to `.a`
        set(${packageName}_LIBRARY_SOURCE ${${packageName}_LIBRARIES})
        set(${packageName}_LIBRARIES "")
        foreach(library ${${packageName}_LIBRARY_SOURCE})
            # replace each dynamic library with a single one
            set(static_library ${library})
            ReplaceDynamic(static_library)
            list(APPEND ${packageName}_LIBRARIES ${static_library})
        endforeach(library)
    endif()
endmacro(FindStaticLibs)


# Checks if a suitable version for the found library was identified,
# if provided. The library can either force exact or inexact matching.
#
# Args:
#   packageName         Name of the package
#
# Example:
#   MatchVersion(ICU)
#
macro(MatchVersion packageName)
    if(${packageName}_FOUND AND ${packageName}_FIND_VERSION)
        # MATCH VERSION
        if(${packageName}_FIND_VERSION_EXACT)
            # EXACT VERSION
            if(${packageName}_FIND_VERSION VERSION_EQUAL ${packageName}_VERSION)
            else()
                set(${packageName}_FOUND FALSE)
            endif()
        else()
            # GREATER THAN VERSION
            if(${packageName}_VERSION VERSION_LESS ${packageName}_FIND_VERSION)
                set(${packageName}_FOUND FALSE)
            endif()
        endif()
    endif()
endmacro(MatchVersion)


# Check if a sample program compiles, if not, set the library to not found.
#
# Args:
#   packageName         Name of the package
#   code                String of simple program depending on the library
#
# Example:
#   set(IConv_CODE "int main(int argc, char **argv){ return 0; }")
#   CheckCompiles(IConv)
#
macro(CheckCompiles packageName)
    # FLAGS
    set(CMAKE_REQUIRED_INCLUDES ${${packageName}_INCLUDE_DIRS})
    set(CMAKE_REQUIRED_LIBRARIES ${${packageName}_LIBRARIES})

    # COMPILATION
    check_cxx_source_compiles("${${packageName}_CODE}" ${packageName}_COMPILES)
    if(NOT ${${packageName}_COMPILES})
        set(${packageName}_FOUND FALSE)
        message(SEND_ERROR "Cannot compile a simple ${packageName} program.")
    endif()

endmacro(CheckCompiles)


# Send an error if a required package was not found. Otherwise, if the
# package is found, report to the user it was identified.
#
# Args:
#   packageName         Name of the package
#
# Example:
#   RequiredPackageFound(ICU)
#
macro(RequiredPackageFound packageName)
    if(${packageName}_FOUND)
        message("Found ${packageName}.")
    else()
        if(${packageName}_FIND_REQUIRED)
            message(SEND_ERROR "Unable to find requested ${packageName} libraries.")
        endif()
    endif()
endmacro(RequiredPackageFound)
