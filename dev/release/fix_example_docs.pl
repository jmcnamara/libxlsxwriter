#!/usr/bin/perl

#
# Simple program to arrange the example programs in a user defined order
# instead or sorted order. Also add a caption.
#
# Copyright 2014-2020, John McNamara, jmcnamara@cpan.org
#
use warnings;
use strict;

# The required example order and descriptions.
my @examples = (
    [ 'hello.c',                'A simple hello world example' ],
    [ 'anatomy.c',              'The anatomy of a libxlsxwriter program' ],
    [ 'demo.c',                 'Demo of some of the libxlsxwriter features' ],
    [ 'tutorial1.c',            'Tutorial 1 from the documentation' ],
    [ 'tutorial2.c',            'Tutorial 2 from the documentation' ],
    [ 'tutorial3.c',            'Tutorial 3 from the documentation' ],
    [ 'format_font.c',          'Example of writing data with font formatting' ],
    [ 'format_num_format.c',    'Example of writing data with number formatting' ],
    [ 'dates_and_times01.c',    'Writing dates and times with numbers' ],
    [ 'dates_and_times02.c',    'Writing dates and times with datetime' ],
    [ 'dates_and_times03.c',    'Dates and times with different formats' ],
    [ 'hyperlinks.c',           'A example of writing urls/hyperlinks' ],
    [ 'rich_strings.c',         'A example of writing "rich" multi-format strings' ],
    [ 'array_formula.c',        'A example of using array formulas' ],
    [ 'utf8.c',                 'A example of some UTF-8 text' ],
    [ 'constant_memory.c',      'Write a large file with constant memory usage' ],
    [ 'merge_range.c',          'Create a merged range of cells' ],
    [ 'merge_rich_string.c',    'Create a merged range with a rich string' ],
    [ 'autofilter.c',           'An example of a worksheet autofilter' ],
    [ 'data_validate.c',        'Examples of worksheet data validation and drop down lists' ],
    [ 'conditional_format1.c',  'A simple conditional formatting example' ],
    [ 'conditional_format2.c',  'An advanced conditional formatting example' ],
    [ 'images.c',               'Example of adding images to a worksheet.' ],
    [ 'image_buffer.c',         'Example of adding an image from a memory buffer.' ],
    [ 'headers_footers.c',      'Example of adding worksheet headers/footers' ],
    [ 'defined_name.c',         'Example of how to create defined names' ],
    [ 'outline.c',              'Example of grouping and outlines' ],
    [ 'outline_collapsed.c',    'Example of grouping and collapsed outlines' ],
    [ 'tab_colors.c',           'Example of how to set worksheet tab colors' ],
    [ 'diagonal_border.c',      'Example of how to set a worksheet cell diagonal border.' ],
    [ 'hide_sheet.c',           'Example of hiding a worksheet' ],
    [ 'doc_properties.c',       'Example of setting workbook doc properties' ],
    [ 'doc_custom_properties.c','Example of setting custom doc properties' ],
    [ 'worksheet_protection.c', 'Example of enabling worksheet protection' ],
    [ 'hide_row_col.c',         'Example of hiding worksheet rows and columns' ],
    [ 'comments1.c',            'Example of adding cell comments to a worksheet' ],
    [ 'comments2.c',            'Example of adding cell comments with options' ],
    [ 'macro.c',                'Example of adding a VBA macro to a workbook' ],
    [ 'panes.c',                'Example of how to create worksheet panes' ],
    [ 'ignore_errors.c',        'Example of ignoring worksheet errors/warnings' ],
    [ 'chart.c',                'Example of a simple column chart' ],
    [ 'chart_area.c',           'Examples of area charts' ],
    [ 'chart_bar.c',            'Examples of bar charts' ],
    [ 'chart_column.c',         'Examples of column charts' ],
    [ 'chart_line.c',           'Example of a line chart' ],
    [ 'chart_scatter.c',        'Examples of scatter charts' ],
    [ 'chart_radar.c',          'Examples of radar charts' ],
    [ 'chart_pie.c',            'Examples of pie charts' ],
    [ 'chart_doughnut.c',       'Examples of doughnut charts' ],
    [ 'chart_clustered.c',      'Examples of clustered category chart' ],
    [ 'chart_data_table.c',     'Examples of charts with data tables' ],
    [ 'chart_data_tools.c',     'Examples of charts data tools' ],
    [ 'chart_data_labels.c',    'Examples of charts data labels' ],
    [ 'chart_fonts.c',          'Examples of using charts fonts' ],
    [ 'chart_pattern.c',        'Examples of using charts patterns' ],
    [ 'chart_styles.c',         'Examples of built-in charts styles' ],
    [ 'chartsheet.c',           'Example of a chartsheet chart' ],
);

# Convert the array refs to a hash for lookups.
my %examples;
for my $example (@examples) {
    $examples{$example->[0]} = 1;
}

my $in_list = 0;

while ( my $line = <> ) {

    # Print all lines not in the <ul> list.
    print $line if !$in_list;

    # Check for <ul> list.
    if ( $line =~ /<div class="textblock">/ ) {
        $in_list = 1;
    }

    # Capture the <li> items of the list.
    if ( $line =~ /<li><a class="el" href="[^"]+">([^<]+)/ ) {
        my $example = $1;

        # Warn if there are any new/unkown items.
        if ( !exists $examples{$example} ) {
            warn "$0 Unknown example: $example\n";
        }
        next;
    }

    # At the end of the <ul> list print out the <li> items in user defined order.
    if ( $line =~ m{^</ul>} ) {
        $in_list = 0;

        for my $aref ( @examples ) {
            my $example  = $aref->[0];
            my $filename = $aref->[0];
            my $desc     = $aref->[1];

            $example =~ s/\.c/_8c-example.html/;

            printf qq(<li><a class="el" href="%s">%s</a> %s</li>\n\n),
              $example, $filename, $desc;
        }
        print $line;
    }
}

__END__
