###############################################################################
#
# Makefile for libxlsxwriter testcase.
#
# Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
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

all : $(LIBXLSXWRITER) $(EXES)

$(LIBXLSXWRITER):
	$(Q)$(MAKE) -C ../.. all

clean :
	$(Q)rm -f $(EXES)

# Executable targets.
%: %.cpp $(LIBXLSXWRITER)
	$(Q)$(CXX) -I$(INC_DIR) $(CXXFLAGS) $< -o $@ $(LIBS)
