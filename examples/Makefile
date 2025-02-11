###############################################################################
#
# Makefile for libxlsxwriter examples.
#
# Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
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
ifdef USE_SYSTEM_MINIZIP
LIBS += -lminizip
endif
ifdef USE_OPENSSL_MD5
LIBS += -lcrypto
endif

all : $(LIBXLSXWRITER) $(EXES)

$(LIBXLSXWRITER):
	$(Q)$(MAKE) -C ../third_party/minizip
ifndef USE_STANDARD_TMPFILE
	$(Q)$(MAKE) -C ../third_party/tmpfileplus
endif
ifndef USE_STANDARD_DOUBLE
	$(Q)$(MAKE) -C ../third_party/dtoa
endif
ifndef USE_NO_MD5
	$(Q)$(MAKE) -C ../third_party/md5
endif
	$(Q)$(MAKE) -C ../src

clean :
	$(Q)rm -f $(EXES)

# Executable targets.
%: %.c $(LIBXLSXWRITER)
	$(Q)$(CC) -I$(INC_DIR) $(CFLAGS) $< -o $@ $(LIBS)

test_valgrind: all
	$(Q)$(foreach exe,$(EXES),valgrind -q --error-exitcode=1 --leak-check=full ./$(exe) || exit;)
