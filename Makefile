###############################################################################
#
# Makefile for libxlsxwriter library.
#
# SPDX-License-Identifier: BSD-2-Clause
# Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
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

VERSION   = $(shell sed -n -e 's/.*LXW_VERSION \"\(.*\)\"/\1/p'   include/xlsxwriter.h)
SOVERSION = $(shell sed -n -e 's/.*LXW_SOVERSION \"\(.*\)\"/\1/p' include/xlsxwriter.h)

ENABLED_OPTIONS = zlib
ifdef USE_SYSTEM_MINIZIP
    ENABLED_OPTIONS += minizip
endif
ifdef USE_OPENSSL_MD5
	ENABLED_OPTIONS += libcrypto
endif

.PHONY: docs tags examples third_party

# Build libxlsxwriter.
all : third_party
	$(Q)$(MAKE) -C src

# Build the third party libs.
third_party :
ifndef USE_SYSTEM_MINIZIP
	$(Q)$(MAKE) -C third_party/minizip
endif
ifndef USE_STANDARD_TMPFILE
	$(Q)$(MAKE) -C third_party/tmpfileplus
endif
ifndef USE_NO_MD5
ifndef USE_OPENSSL_MD5
	$(Q)$(MAKE) -C third_party/md5
endif
endif
ifdef USE_DTOA_LIBRARY
	$(Q)$(MAKE) -C third_party/dtoa
endif

# Build a macOS universal binary.
universal_binary :
	$(Q)$(MAKE) clean
	$(Q)TARGET_ARCH="-target x86_64-apple-macos10.12" $(MAKE) all
	$(Q)mv lib/libxlsxwriter.a     libxlsxwriter_x86_64.a
	$(Q)mv lib/libxlsxwriter.$(SOVERSION).dylib libxlsxwriter_x86_64.dylib

	$(Q)$(MAKE) clean
	$(Q)TARGET_ARCH="-target arm64-apple-macos11" $(MAKE) all
	$(Q)mv lib/libxlsxwriter.a     lib/libxlsxwriter_arm64.a
	$(Q)mv lib/libxlsxwriter.$(SOVERSION).dylib lib/libxlsxwriter_arm64.dylib
	$(Q)mv libxlsxwriter_x86_64.a libxlsxwriter_x86_64.dylib lib

	$(Q)lipo -create -output lib/libxlsxwriter.a                  lib/libxlsxwriter_x86_64.a     lib/libxlsxwriter_arm64.a
	$(Q)lipo -create -output lib/libxlsxwriter.$(SOVERSION).dylib lib/libxlsxwriter_x86_64.dylib lib/libxlsxwriter_arm64.dylib
	$(Q)rm -f lib/libxlsxwriter_x86_64.* lib/libxlsxwriter_arm64.*

# Build the example programs.
examples : all
	$(Q)$(MAKE) -C examples

# Build the example programs with CPP for compatibility checking.
examples_cpp : all
	$(Q)$(MAKE) -C examples CC=$(CXX)

# Clean src and test directories.
clean :
	$(Q)$(MAKE) clean -C src
	$(Q)$(MAKE) clean -C test/unit
	$(Q)$(MAKE) clean -C test/functional/src
	$(Q)$(MAKE) clean -C test/cpp
	$(Q)$(MAKE) clean -C examples
	$(Q)rm -rf docs/html
	$(Q)rm -rf test/functional/__pycache__
	$(Q)rm -f  test/functional/*.pyc
	$(Q)rm -f  lib/*
	$(Q)$(MAKE) clean -C third_party/minizip
	$(Q)$(MAKE) clean -C third_party/tmpfileplus
	$(Q)$(MAKE) clean -C third_party/md5
	$(Q)$(MAKE) clean -C third_party/dtoa

# Clean src and lib dir only, as a precursor for static analysis.
clean_src :
	$(Q)$(MAKE) clean -C src
	$(Q)rm -f  lib/*

# Run the unit tests.
test : all test_cpp test_unit test_functional

# Test for C++ const correctness on APIs.
test_const : all
	$(Q)$(MAKE) clean -C test/functional/src
	$(Q)! $(MAKE) -C test/functional/src CFLAGS=-Wwrite-strings 2>&1 | grep -A 1 "note:"


# Run the functional tests.
test_functional : all
	$(Q)$(MAKE) -C test/functional/src
	$(Q)$(PYTEST) test/functional -v -k $(PYTESTFILES)

# Run all tests.
test_unit : all
	$(Q)$(MAKE) -C src test_lib
	$(Q)$(MAKE) -C test/unit test

# Test C++ compilation.
test_cpp : all
	$(Q)$(MAKE) -C test/cpp

# Test Cmake. This test should really be done with Cmake in the cmake dir but
# this is a workaround for now.
test_cmake :
ifneq ($(findstring m32,$(CFLAGS)),m32)
	$(Q)$(MAKE) -C src clean
	$(Q)mkdir -p build
	$(Q)cd build
	$(Q)cmake .. -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON
	$(Q)make clean; make; cp libxlsxwriter.a ../src/
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
	$(Q)gersemi --no-warn-about-unknown-commands -i CMakeLists.txt

tags:
	$(Q)rm -f TAGS
	$(Q)etags src/*.c include/*.h include/xlsxwriter/*.h

# Build the doxygen docs.
doc: docs
docs:
	$(Q)$(MAKE) -C docs
	@echo "Docs built."

docs_doxygen_only:
	$(Q)$(MAKE) -C docs docs_doxygen_only

docs_external:
	$(Q)make -C ../libxlsxwriter.github.io release

# Simple install.
install: all
	$(Q)mkdir -p        $(DESTDIR)$(PREFIX)/include
	$(Q)cp -R include/* $(DESTDIR)$(PREFIX)/include
	$(Q)mkdir -p        $(DESTDIR)$(PREFIX)/lib
	$(Q)cp -R lib/*     $(DESTDIR)$(PREFIX)/lib
	$(Q)mkdir -p        $(DESTDIR)$(PREFIX)/lib/pkgconfig
	$(Q)sed             -e 's|@PREFIX@|$(PREFIX)|g'                   \
	                    -e 's|@VERSION@|$(VERSION)|g'                 \
	                    -e 's|@ENABLED_OPTIONS@|$(ENABLED_OPTIONS)|g' \
	                        dev/release/pkg-config.txt                \
	                        > $(DESTDIR)$(PREFIX)/lib/pkgconfig/xlsxwriter.pc

# Simpler uninstall.
uninstall:
	$(Q)rm -rf $(DESTDIR)$(PREFIX)/include/xlsxwriter*
	$(Q)rm     $(DESTDIR)$(PREFIX)/lib/libxlsxwriter.*
	$(Q)rm     $(DESTDIR)$(PREFIX)/lib/pkgconfig/xlsxwriter.pc

# Strip the lib files.
strip:
	$(Q)strip lib/*

# Run a coverity static analysis.
coverity: clean_src third_party
	$(Q)rm -rf  cov-int
	$(Q)rm -f libxlsxwriter-coverity.tgz
	$(Q)../../cov-analysis-linux64-2019.03/bin/cov-build --dir cov-int make -C src libxlsxwriter.a
	$(Q)tar -czf libxlsxwriter-coverity.tgz cov-int
	$(Q)$(MAKE) -C src clean
	$(Q)rm -f  lib/*

# Run address sanitiser.
address_sanitiser: third_party
	$(Q)$(MAKE) -C src libxlsxwriter.a CFLAGS="-fsanitize=address -O1 -fsanitize-coverage=trace-pc-guard -fno-omit-frame-pointer -g"
	$(Q)$(MAKE) -C examples            CFLAGS="-fsanitize=address -O1 -fsanitize-coverage=trace-pc-guard -fno-omit-frame-pointer -g"

# Run gcov coverage analysis.
gcov: third_party
	$(Q)$(MAKE) -C src clean
	$(Q)$(MAKE) -C src                 GCOV="--coverage" OPT_LEVEL="-O0"
	$(Q)$(MAKE) -C src test_lib        GCOV="--coverage"
	$(Q)$(MAKE) -C test/unit test      GCOV="--coverage"
	$(Q)$(MAKE) -C test/functional/src GCOV="--coverage"
	$(Q)$(PYTEST) test/functional -v -k $(PYTESTFILES)
	$(Q)mkdir -p build
	$(Q)gcovr -r src --html-details -o build/libxlsxwriter_gcov.html
	$(Q)gcovr -r . -f src --sonarqube build/coverage.xml

# Run sonarcloud analysis.
sonarcloud: gcov
ifndef SONAR_TOKEN
	@echo "Please define SONAR_TOKEN to run this analysis."
	@exit 1
endif
	$(Q)$(MAKE) clean
	$(Q)../sonar-scanner-4.6.1.2450-macosx/bin/build-wrapper-macosx-x86 --out-dir build make all
	$(Q)../sonar-scanner-4.6.1.2450-macosx/bin/sonar-scanner \
        -Dsonar.organization=jmcnamara-github \
        -Dsonar.projectKey=jmcnamara_libxlsxwriter \
        -Dsonar.projectName=libxlsxwriter \
        -Dsonar.projectVersion=$(VERSION) \
        -Dsonar.sources=src \
        -Dsonar.sourceEncoding=UTF-8 \
        -Dsonar.cfamily.build-wrapper-output=build \
        -Dsonar.working.directory=build/scannerwork \
        -Dsonar.host.url=https://sonarcloud.io \
        -Dsonar.coverageReportPaths=build/coverage.xml

sonarcloud_no_gcov:
ifndef SONAR_TOKEN
	@echo "Please define SONAR_TOKEN to run this analysis."
	@exit 1
endif
	$(Q)$(MAKE) clean
	$(Q)../sonar-scanner-4.6.1.2450-macosx/bin/build-wrapper-macosx-x86 --out-dir build make all
	$(Q)../sonar-scanner-4.6.1.2450-macosx/bin/sonar-scanner \
        -Dsonar.organization=jmcnamara-github \
        -Dsonar.projectKey=jmcnamara_libxlsxwriter \
        -Dsonar.projectName=libxlsxwriter \
        -Dsonar.projectVersion=$(VERSION) \
        -Dsonar.sources=src \
        -Dsonar.sourceEncoding=UTF-8 \
        -Dsonar.cfamily.build-wrapper-output=build \
        -Dsonar.working.directory=build/scannerwork \
        -Dsonar.host.url=https://sonarcloud.io


# Run a scan-build static analysis.
scan_build: clean_src third_party
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
	@echo "Pushing to git main ..."
	$(Q)git push origin main
	$(Q)git push --tags

	@echo
	@echo "Pushing updated docs ..."
	$(Q)make -C ../libxlsxwriter.github.io release

	@echo
	@echo "Pushing the cocoapod ..."
	$(Q)pod trunk push libxlsxwriter.podspec --use-libraries

	@echo
	@echo "Finished. Opening files."
	$(Q)open https://libxlsxwriter.github.io/changes.html
	$(Q)open https://cocoadocs.org/docsets/libxlsxwriter
	$(Q)open https://github.com/jmcnamara/libxlsxwriter
	$(Q)open https://github.com/jmcnamara/libxlsxwriter/releases
