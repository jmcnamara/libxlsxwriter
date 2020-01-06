#!/usr/bin/perl

# Simple script to increment x.y.z style version numbers in a file.

use strict;
use warnings;
use Perl::Version;

while (<>) {

    # Increment any x.y.z version strings.
    if (m/(\d\.\d\.\d)/) {
        my $version = Perl::Version->new( $1 );

        # Components are: revision, version and subversion.
        if ( $version->version == 9 && $version->subversion == 9 ) {
            $version->inc_revision();
        }
        elsif ( $version->subversion == 9 ) {
            $version->inc_version();
        }
        else {
            $version->inc_subversion();
        }

        my $new_version = $version->stringify();
        s/\d\.\d\.\d/$new_version/;
    }

    # Increment the LXW_VERSION_ID number in xlsxwriter.h
    if (m/LXW_VERSION_ID (\d+)/) {
        my $version = $1;
        my $new_version = $version + 1;

        s/\d+/$new_version/;
    }

    print;
}


__END__
