###############################################################################
#
# Makefile for libxlsxwriter library.
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
#

# Keep the output quiet by default.
Q=@
ifdef V
Q=
endif

# Make everything.
all : docs

# Clean up.
clean :
	$(Q)rm -rf html/*

# Build the doxygen docs.
docs:
	$(Q)perl ../dev/release/fix_example_links.pl src/examples.txt > src/examples.dox
	$(Q)doxygen
	$(Q)../dev/release/fix_dox.sh

