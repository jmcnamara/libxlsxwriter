###############################################################################
#
# Makefile for libxlsxwriter library.
#
# Copyright 2014, John McNamara, jmcnamara@cpan.org
#

# Keep the output quiet by default.
Q=@
ifdef V
Q=
endif

.PHONY: docs tags examples

# Build the libs.
all :
	$(Q)make -C third_party/minizip
	$(Q)make -C src

# Build the example programs.
examples:
	$(Q)make -C examples

# Clean src and test directories.
clean :
	$(Q)make clean -C src
	$(Q)make clean -C test/unit
	$(Q)make clean -C test/functional/src
	$(Q)make clean -C examples
	$(Q)make clean -C third_party/minizip
	$(Q)rm -rf docs/html
	$(Q)rm -rf test/functional/__pycache__
	$(Q)rm -f  test/functional/*.pyc
	$(Q)rm -f  lib/*

# Run the unit tests.
test : all test_functional test_unit
	$(Q)make test -C test/unit

# Run the functional tests.
test_functional : all
	$(Q)make -C test/functional/src
	$(Q)py.test test/functional -v

# Run all tests.
test_unit :
	@echo "Compiling unit tests ..."
	$(Q)make -C third_party/minizip
	$(Q)make -C src test_lib
	$(Q)make -C test/unit test

# Test the functional test exes with valgrind.
test_valgrind : all
	$(Q)make -C test/functional/src test_valgrind
	$(Q)make -C examples test_valgrind

# Indent the source files with the .indent.pro settings.
indent:
	$(Q)gindent src/*.c include/*.h

tags:
	$(Q)rm -f TAGS
	$(Q)etags src/*.c include/*.h include/xlsxwriter/*.h

# Build the doxygen docs.
docs:
	$(Q)make -C docs

# Simple minded install.
install:
	$(Q)cp -r include/* /usr/include
	$(Q)cp lib/* /usr/lib
