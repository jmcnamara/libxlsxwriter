#!/usr/bin/perl

#
# Simple program to generate a Windows .def file from the exported symbols in
# libxlsxwriter.a.
#
#    perl dev/release/gen_windows_def_file.pl lib/libxlsxwriter.a
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
#
use warnings;
use strict;

my $lib_file = shift;

die "$0: Path to .a lib file required.\n" if !$lib_file;
die "$0: File '$lib_file' not found\n" if !-e $lib_file;

# Get the symbols from the libxlsxwriter.a file.
my @symbols = `nm $lib_file`;
my %unique;

for my $symbol ( @symbols ) {

    chomp $symbol;

    # Get the last field in the row.
    my @fields = split " ", $symbol;
    $symbol = $fields[-1];

    next unless $symbol;

    # Skip symbols not belonging to libxlsxwriter.
    next if $symbol !~ /^_(lxw|work|format|chart|new)/;

    # Skip some the RedBlack functions.
    next if $symbol =~ m{RB};

    # Strip the leading underscore.
    $symbol =~ s/^_//;

    # Remove duplicate instances of some symbols.
    $unique{$symbol}++;
}

# Generate the .def file.
print "EXPORTS\r\n";
for my $symbol ( sort keys %unique ) {
    print "    ", $symbol, "\r\n";
}
