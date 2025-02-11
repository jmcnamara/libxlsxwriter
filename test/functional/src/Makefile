###############################################################################
#
# Makefile for libxlsxwriter functional test executables.
#
# SPDX-License-Identifier: BSD-2-Clause
# Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
#

# Keep the output quiet by default.
Q=@
ifdef V
Q=
endif


# Directory variables.
INC_DIR     = ../../../include

# Flags passed to the C++ compiler.
CFLAGS += -g -Wall -Wextra

# Source files to compile.
SRCTESTFILES ?= *.c
SRCS = $(wildcard $(SRCTESTFILES))
EXES = $(patsubst %.c,%,$(SRCS))


ifndef GCOV
LIBXLSXWRITER = ../../../src/libxlsxwriter.a
else
# Link against the same lib used by the unit tests.
LIBXLSXWRITER = ../../../src/libxlsxwriter_test.a
endif


LIBS = $(LIBXLSXWRITER) -lz
ifdef USE_SYSTEM_MINIZIP
LIBS   += -lminizip
CFLAGS += -DUSE_SYSTEM_MINIZIP
endif

ifdef USE_OPENSSL_MD5
LIBS   += -lcrypto
endif

# Use a third party double number formatting function.
ifdef USE_DTOA_LIBRARY
CFLAGS += -DUSE_DTOA_LIBRARY
endif

all : $(LIBXLSXWRITER) $(EXES)

$(LIBXLSXWRITER):
ifndef USE_SYSTEM_MINIZIP
	$(Q)$(MAKE) -C ../../../third_party/minizip
endif
	$(Q)$(MAKE) -C ../../../src

clean :
	$(Q)rm -f $(EXES)
	$(Q)rm -rf *.dSYM *.gcno *.gcda

# Executable targets.
%: %.c $(LIBXLSXWRITER)
	$(Q)$(CC) -I$(INC_DIR) $(CFLAGS) $(LDFLAGS) $(GCOV) $< -o $@ $(LIBS)

test_valgrind: all
	$(Q)$(foreach exe,$(EXES),valgrind -q --error-exitcode=1 --leak-check=full ./$(exe) || exit;)
