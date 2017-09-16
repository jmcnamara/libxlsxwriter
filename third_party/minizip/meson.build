###############################################################################
#
# Meson build file for the libxlsxwriter library.
#
# Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
#

minizip_sources = ['ioapi.c', 'zip.c']

minizip = shared_library('minizip',
                          minizip_sources,
                          dependencies : zlib_dep,
                          install : false)
