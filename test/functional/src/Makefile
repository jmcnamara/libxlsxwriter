###############################################################################
#
# Makefile for libxlsxwriter functional test executables.
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
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
SRCS = $(wildcard *.c)
EXES = $(patsubst %.c,%,$(SRCS))

LIBXLSXWRITER = ../../../src/libxlsxwriter.a

LIBS = $(LIBXLSXWRITER) -lz

all : $(LIBXLSXWRITER) $(EXES)

$(LIBXLSXWRITER):
	$(Q)$(MAKE) -C ../../../third_party/minizip
	$(Q)$(MAKE) -C ../../../src

clean :
	$(Q)rm -f $(EXES)
	$(Q)rm -rf *.dSYM

# Executable targets.
%: %.c $(LIBXLSXWRITER)
	$(Q)$(CC) -I$(INC_DIR) $(CFLAGS) $< -o $@ $(LIBS)

test_valgrind: all
	$(Q)$(foreach exe,$(EXES),valgrind -q --error-exitcode=1 --leak-check=full ./$(exe) || exit;)

