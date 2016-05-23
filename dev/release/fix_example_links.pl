#!/usr/bin/perl

#
# Simple program to generate the examples.dox file from a simple text file,
# with links to the next/previous examples.
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
#
use strict;
use warnings;

my @examples;
my @sections;
my @links;
my $buffer = '';


# Sample through the example sections and break the text into blocks.
while ( my $line = <> ) {

    # Ignore comments in the input file.
    next if $line =~ /^#/;

    # Match the start of an example block.
    if ( $line =~ /^\@example/ ) {
        chomp $buffer;

        # Store the example name and the section body.
        push @examples, $line;
        push @sections, $buffer;
        $buffer = '';
        next;
    }

    $buffer .= $line;
}

# Store the last example section and omit the first blank element.
push @sections, $buffer;
shift @sections;

# Generate a set of @ref links targets from the example program names.
for ( @examples ) {
    my $link = $_;
    chomp $link;
    $link =~ s/\@example //;
    push @links, [ $link, $link ];
}

# Add the first and last links back to the examples.
unshift @links, [ "examples", "Examples page" ];
push @links,    [ "examples", "Examples page" ];

# Add the start of the Doxygen header.
print "/**\n";
print "\@page examples Example Programs\n\n";

# Print out each section.
for my $i ( 0 .. @examples - 1 ) {

    print $examples[$i];

    # Add a simple header table with next/previous links.
    printf qq{\n<table  width="600">\n};
    printf qq{<tr>\n};
    printf qq{  <td>\@ref %s "&lt;&lt; %s"</td>\n},
           $links[$i]->[0], $links[$i]->[1];
    printf qq{  <td align="right">\@ref %s "%s &gt;&gt;"</td>\n},
           $links[ $i + 2 ]->[0], $links[ $i + 2 ]->[1];
    printf qq{</tr>\n};
    printf qq{</table>\n};

    print $sections[$i], "\n\n\n\n";
}

# Print the end of the doxygen comment.
print "*/\n";

