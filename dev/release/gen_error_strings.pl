#!/usr/bin/perl

#
# Simple program to generate the string array for the lxw_strerror() function
# from the Doxygen comments in the lxw_error enum:
#
#    perl dev/release/gen_error_strings.pl include/xlsxwriter/common.h
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
#
use warnings;
use strict;

my $in_enum = 0;
my @strings;

my $filename = shift || 'include/xlsxwriter/common.h';
open my $fh, '<', $filename or die "Couldn't open $filename: $!\n";


while (<$fh>) {

    $in_enum = 1 if /typedef enum lxw_error/;
    $in_enum = 0 if /} lxw_error;/;

    # Match doxygen strings in the enum.
    if ($in_enum && m{/\*\*}) {
        # Strip the comment parts.
        s{/\*\*}{};
        s{\*/}{};
        s{^\s+}{};
        s{\s+$}{};
        push @strings, $_;
    }
}

# Print out an array of strings based on the doxygen comments.
print "\n";
print "// Copy to src/utility.c\n\n";
print "char *error_strings[LXW_MAX_ERRNO + 1] = {\n";
for my $string (@strings) {
    print qq{    "$string",\n};
}
print qq{    "Unknown error number."\n};
print "};\n\n";




