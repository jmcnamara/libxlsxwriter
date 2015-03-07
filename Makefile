###############################################################################
#
# Makefile for libxlsxwriter library.
#
# Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
#

# Keep the output quiet by default.
Q=@
ifdef V
Q=
endif

.PHONY: docs tags examples

# Build the libs.
all :
	$(Q)$(MAKE) -C third_party/minizip
	$(Q)$(MAKE) -C src

# Build the example programs.
examples :
	$(Q)$(MAKE) -C examples

# Clean src and test directories.
clean :
	$(Q)$(MAKE) clean -C src
	$(Q)$(MAKE) clean -C test/unit
	$(Q)$(MAKE) clean -C test/functional/src
	$(Q)$(MAKE) clean -C examples
	$(Q)$(MAKE) clean -C third_party/minizip
	$(Q)rm -rf docs/html
	$(Q)rm -rf test/functional/__pycache__
	$(Q)rm -f  test/functional/*.pyc
	$(Q)rm -f  lib/*

# Run the unit tests.
test : all test_functional test_unit

# Run the functional tests.
test_functional : all
	$(Q)$(MAKE) -C test/functional/src
	$(Q)py.test test/functional -v

# Run all tests.
test_unit :
	@echo "Compiling unit tests ..."
	$(Q)$(MAKE) -C third_party/minizip
	$(Q)$(MAKE) -C src test_lib
	$(Q)$(MAKE) -C test/unit test

# Test the functional test exes with valgrind.
test_valgrind : all
	$(Q)$(MAKE) -C test/functional/src test_valgrind
	$(Q)$(MAKE) -C examples test_valgrind

# Minimal target for quick compile without creating the libs.
test_compile :
	$(Q)$(MAKE) -C src	test_compile

# Indent the source files with the .indent.pro settings.
indent:
	$(Q)gindent src/*.c include/*.h include/xlsxwriter/*.h

tags:
	$(Q)rm -f TAGS
	$(Q)etags src/*.c include/*.h include/xlsxwriter/*.h

# Build the doxygen docs.
docs:
	$(Q)$(MAKE) -C docs

# Simple minded install.
install:
	$(Q)cp -r include/* /usr/include
	$(Q)cp lib/* /usr/lib

# Run a coverity static analysis.
coverity:
	$(Q)$(MAKE) -C third_party/minizip
	$(Q)$(MAKE) -C src clean
	$(Q)rm -f  lib/*
	$(Q)rm -rf  cov-int
	$(Q)rm -f libxlsxwriter-coverity.tgz
	$(Q)../cov-analysis-macosx-7.6.0/bin/cov-build --dir cov-int make -C src libxlsxwriter.a
	$(Q)tar -czf libxlsxwriter-coverity.tgz cov-int
	$(Q)$(MAKE) -C src clean
	$(Q)rm -f  lib/*
