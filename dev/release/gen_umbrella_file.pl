#!/usr/bin/perl

#
# Simple program to generate the coccoapods unbrella file.
# Run from the libxlsxwriter root dir.
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
#
use warnings;
use strict;
use File::Find;

my @includes;

# Callback to match header files.
sub match_include {
    push @includes, $File::Find::name if /^.*\.h\z/s;
}

# Use File::Find to find header files.
find({wanted => \&match_include}, 'include/xlsxwriter');

# Sort and remove leading dirs from the include files.
@includes = sort @includes;
s{^include/xlsxwriter/}{} for @includes;


# Generate the unbrella file.
print qq{#import <UIKit/UIKit.h>\n\n};
print qq{#import "../xlsxwriter.h"\n};

print qq{#import "$_"\n} for @includes;

print qq{\n};
print qq{FOUNDATION_EXPORT double xlsxwriterVersionNumber;\n};
print qq{FOUNDATION_EXPORT const unsigned char xlsxwriterVersionString[];\n\n};
