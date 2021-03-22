###############################################################################
#
# Makefile for libxlsxwriter library.
#
# Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
#

# Keep the output quiet by default.
Q=@
ifdef V
Q=
endif

DESTDIR ?=
PREFIX  ?= /usr/local

PYTEST ?= py.test
PYTESTFILES ?= test

VERSION = $(shell sed -n -e '/VERSION "/s/.*"\(.*\)".*/\1/p' < include/xlsxwriter.h)

.PHONY: docs tags examples

# Build the libs.
all :
ifndef USE_SYSTEM_MINIZIP
	$(Q)$(MAKE) -C third_party/minizip
endif
ifndef USE_STANDARD_TMPFILE
	$(Q)$(MAKE) -C third_party/tmpfileplus
endif
ifndef USE_NO_MD5
	$(Q)$(MAKE) -C third_party/md5
endif
	$(Q)$(MAKE) -C src

universal_binary :
	$(Q)$(MAKE) clean
	$(Q)TARGET_ARCH="-target x86_64-apple-macos10.12" $(MAKE) all
	$(Q)mv lib/libxlsxwriter.a     libxlsxwriter_x86_64.a
	$(Q)mv lib/libxlsxwriter.dylib libxlsxwriter_x86_64.dylib

	$(Q)$(MAKE) clean
	$(Q)TARGET_ARCH="-target arm64-apple-macos11" $(MAKE) all
	$(Q)mv lib/libxlsxwriter.a     lib/libxlsxwriter_arm64.a
	$(Q)mv lib/libxlsxwriter.dylib lib/libxlsxwriter_arm64.dylib
	$(Q)mv libxlsxwriter_x86_64.a libxlsxwriter_x86_64.dylib lib

	$(Q)lipo -create -output lib/libxlsxwriter.a     lib/libxlsxwriter_x86_64.a     lib/libxlsxwriter_arm64.a
	$(Q)lipo -create -output lib/libxlsxwriter.dylib lib/libxlsxwriter_x86_64.dylib lib/libxlsxwriter_arm64.dylib

# Build the example programs.
examples :
	$(Q)$(MAKE) -C examples

# Clean src and test directories.
clean :
	$(Q)$(MAKE) clean -C src
	$(Q)$(MAKE) clean -C test/unit
	$(Q)$(MAKE) clean -C test/functional/src
	$(Q)$(MAKE) clean -C examples
	$(Q)rm -rf docs/html
	$(Q)rm -rf test/functional/__pycache__
	$(Q)rm -f  test/functional/*.pyc
	$(Q)rm -f  lib/*
ifndef USE_SYSTEM_MINIZIP
	$(Q)$(MAKE) clean -C third_party/minizip
endif
ifndef USE_STANDARD_TMPFILE
	$(Q)$(MAKE) clean -C third_party/tmpfileplus
endif
ifndef USE_NO_MD5
	$(Q)$(MAKE) clean -C third_party/md5
endif

# Run the unit tests.
test : all test_unit test_functional

# Test for C++ const correctness on APIs.
test_const : all
	$(Q)$(MAKE) clean -C test/functional/src
	$(Q)! $(MAKE) -C test/functional/src CFLAGS=-Wwrite-strings 2>&1 | grep -A 1 "note:"


# Run the functional tests.
test_functional : all
	$(Q)$(MAKE) -C test/functional/src
	$(Q)$(PYTEST) test/functional -v -k $(PYTESTFILES)

# Run all tests.
test_unit :
	@echo "Compiling unit tests ..."
ifndef USE_SYSTEM_MINIZIP
	$(Q)$(MAKE) -C third_party/minizip
endif
ifndef USE_STANDARD_TMPFILE
	$(Q)$(MAKE) -C third_party/tmpfileplus
endif
ifndef USE_NO_MD5
	$(Q)$(MAKE) -C third_party/md5
endif
	$(Q)$(MAKE) -C src test_lib
	$(Q)$(MAKE) -C test/unit test

# Test Cmake. This test should really be done with Cmake in the cmake dir but
# this is a workaround for now.
test_cmake :
ifneq ($(findstring m32,$(CFLAGS)),m32)
	$(Q)$(MAKE) -C src clean
	$(Q)cd cmake; cmake .. -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON; make clean; make; cp libxlsxwriter.a ../src/
	$(Q)cmake/xlsxwriter_unit
	$(Q)$(MAKE) -C test/functional/src
	$(Q)$(PYTEST) test/functional -v -k $(PYTESTFILES)
else
	@echo "Skipping Cmake tests on 32 bit target."
endif

# Test the functional test exes with valgrind (in 64bit mode only).
test_valgrind : all
ifndef NO_VALGRIND
	$(Q)$(MAKE) -C test/functional/src test_valgrind
	$(Q)$(MAKE) -C examples test_valgrind
endif

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
doc: docs
docs:
	$(Q)$(MAKE) -C docs

docs_doxygen_only:
	$(Q)$(MAKE) -C docs docs_doxygen_only

docs_external:
	$(Q)make -C ../libxlsxwriter.github.io release

# Simple install.
install: all
	$(Q)mkdir -p        $(DESTDIR)$(PREFIX)/include
	$(Q)cp -R include/* $(DESTDIR)$(PREFIX)/include
	$(Q)mkdir -p        $(DESTDIR)$(PREFIX)/lib
	$(Q)cp lib/*        $(DESTDIR)$(PREFIX)/lib
	$(Q)mkdir -p        $(DESTDIR)$(PREFIX)/lib/pkgconfig
	$(Q)sed -e          's|@PREFIX@|$(PREFIX)|g'  -e 's|@VERSION@|$(VERSION)|g' dev/release/pkg-config.txt > $(DESTDIR)$(PREFIX)/lib/pkgconfig/xlsxwriter.pc

# Simpler uninstall.
uninstall:
	$(Q)rm -rf $(DESTDIR)$(PREFIX)/include/xlsxwriter*
	$(Q)rm     $(DESTDIR)$(PREFIX)/lib/libxlsxwriter.*
	$(Q)rm     $(DESTDIR)$(PREFIX)/lib/pkgconfig/xlsxwriter.pc

# Strip the lib files.
strip:
	$(Q)strip lib/*

# Run a coverity static analysis.
coverity:
ifndef USE_SYSTEM_MINIZIP
	$(Q)$(MAKE) -C third_party/minizip
endif
ifndef USE_STANDARD_TMPFILE
	$(Q)$(MAKE) -C third_party/tmpfileplus
endif
ifndef USE_NO_MD5
	$(Q)$(MAKE) -C third_party/md5
endif
	$(Q)$(MAKE) -C src clean
	$(Q)rm -f  lib/*
	$(Q)rm -rf  cov-int
	$(Q)rm -f libxlsxwriter-coverity.tgz
	$(Q)../../cov-analysis-linux64-2019.03/bin/cov-build --dir cov-int make -C src libxlsxwriter.a
	$(Q)tar -czf libxlsxwriter-coverity.tgz cov-int
	$(Q)$(MAKE) -C src clean
	$(Q)rm -f  lib/*

# Run a scan-build static analysis.
scan_build:
ifndef USE_SYSTEM_MINIZIP
	$(Q)$(MAKE) -C third_party/minizip
endif
ifndef USE_STANDARD_TMPFILE
	$(Q)$(MAKE) -C third_party/tmpfileplus
endif
ifndef USE_NO_MD5
	$(Q)$(MAKE) -C third_party/md5
endif
	$(Q)$(MAKE) -C src clean
	$(Q)rm -f  lib/*
	$(Q)scan-build make -C src libxlsxwriter.a
	$(Q)$(MAKE) -C src clean
	$(Q)rm -f  lib/*

spellcheck:
	$(Q)for f in docs/src/*.dox;         do aspell --lang=en_US --check $$f; done
	$(Q)for f in include/xlsxwriter/*.h; do aspell --lang=en_US --check $$f; done
	$(Q)for f in src/*.c;                do aspell --lang=en_US --check $$f; done
	$(Q)for f in examples/*.c;           do aspell --lang=en_US --check $$f; done
	$(Q)aspell --lang=en_US --check Changes.txt
	$(Q)aspell --lang=en_US --check Readme.md
	$(Q)aspell --lang=en_US --check docs/src/examples.txt

releasecheck:
	$(Q)dev/release/release_check.sh

release: releasecheck
	@echo
	@echo "Pushing to git master ..."
	$(Q)git push origin master
	$(Q)git push --tags

	@echo
	@echo "Pushing updated docs ..."
	$(Q)make -C ../libxlsxwriter.github.io release

	@echo
	@echo "Pushing the cocoapod ..."
	$(Q)pod trunk push libxlsxwriter.podspec --use-libraries

	@echo
	@echo "Finished. Opening files."
	$(Q)open http://libxlsxwriter.github.io/changes.html
	$(Q)open http://cocoadocs.org/docsets/libxlsxwriter
	$(Q)open https://github.com/jmcnamara/libxlsxwriter
	$(Q)open https://github.com/jmcnamara/libxlsxwriter/releases
