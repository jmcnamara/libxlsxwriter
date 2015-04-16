###############################################################################
#
# Makefile for libxlsxwriter examples.
#
# Copyright 2014, John McNamara, jmcnamara@cpan.org
#

# Keep the output quiet by default.
Q=@
ifdef V
Q=
endif

# Directory variables.
INC_DIR = ../include

# Flags passed to the C++ compiler.
CFLAGS += -g -Wall -Wextra

# Source files to compile.
SRCS = $(wildcard *.c)
EXES = $(patsubst %.c,%,$(SRCS))

LIBXLSXWRITER = ../src/libxlsxwriter.a

LIBS = $(LIBXLSXWRITER) -lz

all : $(LIBXLSXWRITER) $(EXES)

$(LIBXLSXWRITER):
	$(Q)make -C ../third_party/minizip
	$(Q)make -C ../src

clean :
	$(Q)rm -f $(EXES)

# Executable targets.
%: %.c $(LIBXLSXWRITER)
	$(Q)$(CC) -I$(INC_DIR) $(CFLAGS) $< -o $@ $(LIBS)

test_valgrind: all
	$(Q)$(foreach exe,$(EXES),valgrind -q --error-exitcode=1 --leak-check=full ./$(exe) || exit;)
