###############################################################################
#
# Makefile for libxlsxwriter testcase.
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
INC_DIR = ../../include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra

# Source files to compile.
SRCS = $(wildcard *.cpp)
EXES = $(patsubst %.cpp,%,$(SRCS))

LIBXLSXWRITER = ../../src/libxlsxwriter.a
LIBS = $(LIBXLSXWRITER) -lz

ifdef USE_SYSTEM_MINIZIP
LIBS += -lminizip
endif

ifdef USE_OPENSSL_MD5
LIBS += -lcrypto
endif

all : $(LIBXLSXWRITER) $(EXES)

$(LIBXLSXWRITER):
	$(Q)$(MAKE) -C ../.. all

clean :
	$(Q)rm -f $(EXES)

# Executable targets.
%: %.cpp $(LIBXLSXWRITER)
	$(Q)$(CXX) -I$(INC_DIR) $(CXXFLAGS) $(LDFLAGS) $< -o $@ $(LIBS)
