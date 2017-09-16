###############################################################################
#
# Meson build file for the libxlsxwriter library.
#
# Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
#

sources = [
    'app.c',
    'chart.c',
    'content_types.c',
    'core.c',
    'custom.c',
    'drawing.c',
    'format.c',
    'hash_table.c',
    'packager.c',
    'relationships.c',
    'shared_strings.c',
    'styles.c',
    'theme.c',
    'utility.c',
    'workbook.c',
    'worksheet.c',
    'xmlwriter.c'
]


libxlsxwriter = shared_library('xlsxwriter',
                               sources,
                               include_directories : inc,
                               dependencies : zlib_dep,
                               link_with : [minizip, tmpfileplus],
                               install : true)
