###############################################################################
#
# Meson build file for the libxlsxwriter library.
#
# Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
#

examples = ['anatomy',
            'array_formula',
            'autofilter',
            'chart',
            'chart_area',
            'chart_bar',
            'chart_clustered',
            'chart_column',
            'chart_data_table',
            'chart_data_tools',
            'chart_doughnut',
            'chart_fonts',
            'chart_line',
            'chart_pattern',
            'chart_pie',
            'chart_pie_colors',
            'chart_radar',
            'chart_scatter',
            'chart_styles',
            'chart_working_with_example',
            'constant_memory',
            'dates_and_times01',
            'dates_and_times02',
            'dates_and_times03',
            'defined_name',
            'demo',
            'doc_custom_properties',
            'doc_properties',
            'format_font',
            'format_num_format',
            'headers_footers',
            'hello',
            'hide_row_col',
            'hide_sheet',
            'hyperlinks',
            'images',
            'merge_range',
            'panes',
            'tab_colors',
            'tutorial1',
            'tutorial2',
            'tutorial3',
            'utf8',
            'worksheet_protection']

foreach example : examples
    executable(example,
               example + '.c',
               include_directories : inc,
               link_with : libxlsxwriter)
endforeach
