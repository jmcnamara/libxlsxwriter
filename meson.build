###############################################################################
#
# Meson build file for the libxlsxwriter library.
#
# Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
#

project('libxlsxwriter', 'c',
        version: '0.7.4',
        default_options: [],
        license: 'BSD')

zlib_dep = dependency('zlib', version : '>=1.2.8')

inc = include_directories('include')

subdir('third_party/minizip')
subdir('third_party/tmpfileplus')
subdir('include')
subdir('src')

if get_option('examples')
    subdir('examples')
endif
