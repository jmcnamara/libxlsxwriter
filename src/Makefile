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
OPT_LEVEL ?= -O3

SOVERSION = $(shell sed -n -e 's/.*LXW_SOVERSION \"\(.*\)\"/\1/p' ../include/xlsxwriter.h)


# Directory variables.
OBJS_DIR    = .
INC_DIR     = ../include

LIBS = -lz

ifdef USE_SYSTEM_MINIZIP
LIBS   += -lminizip
CFLAGS += -DUSE_SYSTEM_MINIZIP
else
# Use the local copy of minizip.
MINIZIP_DIR = ../third_party/minizip
MINIZIP_OBJ = $(MINIZIP_DIR)/ioapi.o  $(MINIZIP_DIR)/zip.o
MINIZIP_SO  = $(MINIZIP_DIR)/ioapi.so $(MINIZIP_DIR)/zip.so
endif

ifdef USE_STANDARD_TMPFILE
# Use standard/C tmpfile().
CFLAGS += -DUSE_STANDARD_TMPFILE
else
# Use tmpfileplus (the default).
TMPFILEPLUS_DIR = ../third_party/tmpfileplus
TMPFILEPLUS_OBJ = $(TMPFILEPLUS_DIR)/tmpfileplus.o
TMPFILEPLUS_SO  = $(TMPFILEPLUS_DIR)/tmpfileplus.so
endif

ifdef USE_NO_MD5
# Don't use MD5 to avoid duplicate image files.
CFLAGS += -DUSE_NO_MD5
else
ifdef USE_OPENSSL_MD5
CFLAGS += -DUSE_OPENSSL_MD5 -Wno-deprecated-declarations
LIBS   += -lcrypto
else
# Use md5 (the default).
MD5_DIR = ../third_party/md5
MD5_OBJ = $(MD5_DIR)/md5.o
MD5_SO  = $(MD5_DIR)/md5.so
endif
endif

# Set flag for big endian architecture.
ifdef USE_BIG_ENDIAN
CFLAGS += -DLXW_BIG_ENDIAN
endif

# Use a third party double number formatting function.
ifdef USE_DTOA_LIBRARY
CFLAGS += -DUSE_DTOA_LIBRARY
DTOA_LIB_DIR = ../third_party/dtoa
DTOA_LIB_OBJ = $(DTOA_LIB_DIR)/emyg_dtoa.o
DTOA_LIB_SO  = $(DTOA_LIB_DIR)/emyg_dtoa.so
endif

# Use fmemopen()/open_memstream() to avoid creating temporary files
ifdef USE_MEM_FILE
USE_FMEMOPEN = 1
endif

ifdef USE_FMEMOPEN
CFLAGS += -DUSE_FMEMOPEN
endif

# Flags passed to compiler.
CFLAGS   += -g $(OPT_LEVEL) -Wall -Wextra -Wstrict-prototypes -pedantic -ansi

# Ignore icc remarks about inlining limits.
ifeq ($(CC),icc)
CFLAGS   +=-diag-disable=11074,11076
endif

# Library names.
LIBXLSXWRITER_A  = libxlsxwriter.a
LIBXLSXWRITER_SO = libxlsxwriter.so.$(SOVERSION)

# Library with additional non-static functions for testing.
LIBXLSXWRITER_TO = libxlsxwriter_test.a

# Flags passed to static linker.
ARFLAGS = rc

# Flags passed to dynamic linker.
FPIC = -fPIC
SOFLAGS = -shared $(FPIC) -Wl,-soname,$(LIBXLSXWRITER_SO)

# Get Env/OS name.
UNAME := $(shell uname)

# Change make options on OS X.
ifeq ($(UNAME), Darwin)
LIBXLSXWRITER_SO = libxlsxwriter.$(SOVERSION).dylib
SOFLAGS = -dynamiclib $(FPIC) -compatibility_version $(SOVERSION) -install_name $(PREFIX)/lib/$(LIBXLSXWRITER_SO) $(GCOV)
endif

# Check for MinGW/MinGW64/Cygwin environments.
ifneq (,$(findstring MINGW, $(UNAME)))
MING_LIKE = y
endif
ifneq (,$(findstring MSYS, $(UNAME)))
MING_LIKE = y
endif
ifneq (,$(findstring CYGWIN, $(UNAME)))
MING_LIKE = y
endif

# Change make options on MinGW/MinGW64/Cygwin.
ifdef MING_LIKE
LIBXLSXWRITER_SO = libxlsxwriter.dll
FPIC =
CC = gcc
CFLAGS += -Wno-char-subscripts -Wno-long-long
endif

# Headers as dependecies.
HDRS = $(wildcard ../include/xlsxwriter/*.h)

# Ojects to compile.
SRCS  = $(wildcard *.c)
OBJS  = $(patsubst %.c,%.o,$(SRCS))
SOBJS = $(patsubst %.c,%.so,$(SRCS))
TOBJS = $(patsubst %.c,%.to,$(SRCS))
# End of OBJS

# Build the object files and the libraries.
all : $(LIBXLSXWRITER_A) $(LIBXLSXWRITER_SO)
	$(Q)cp $(LIBXLSXWRITER_A) $(LIBXLSXWRITER_SO) ../lib

ifeq ($(UNAME), Darwin)
	$(Q)rm -f ../lib/libxlsxwriter.dylib
	$(Q)ln -s $(LIBXLSXWRITER_SO) ../lib/libxlsxwriter.dylib
endif

ifneq ($(UNAME), Darwin)
	$(Q)rm -f ../lib/libxlsxwriter.so
	$(Q)ln -s $(LIBXLSXWRITER_SO) ../lib/libxlsxwriter.so
endif

test_lib : libxlsxwriter_test.a

# The static library.
$(LIBXLSXWRITER_A) : $(OBJS)
	$(Q)$(AR) $(ARFLAGS) $@ $(MINIZIP_OBJ) $(TMPFILEPLUS_OBJ) $(DTOA_LIB_OBJ) $(MD5_OBJ) $^

# The dynamic library.
ifeq ($(findstring m32,$(CFLAGS)),m32)
ARCH = -m32
endif

$(LIBXLSXWRITER_SO) : $(SOBJS)
	$(Q)$(CC) $(LDFLAGS) $(SOFLAGS) $(ARCH) $(TARGET_ARCH) -o $@ $(MINIZIP_SO) $(TMPFILEPLUS_SO) $(MD5_SO) $(DTOA_LIB_SO) $^ $(LIBS)

# The test library.
$(LIBXLSXWRITER_TO) : $(TOBJS)
	$(Q)$(AR) $(ARFLAGS) $@ $(MINIZIP_OBJ) $(TMPFILEPLUS_OBJ) $(DTOA_LIB_SO) $(MD5_OBJ) $^

# Minimal target for quick compile without creating the libs.
test_compile : $(OBJS)

# Targets for the object files.
%.o  : %.c $(HDRS)
	$(Q)$(CC) -I$(INC_DIR) $(CFLAGS) $(CXXFLAGS) $(TARGET_ARCH) $(GCOV) -c $<

%.so : %.c $(HDRS)
	$(Q)$(CC) $(FPIC) -I$(INC_DIR) $(CFLAGS) $(CXXFLAGS) $(TARGET_ARCH) $(GCOV) -c $< -o $@

%.to : %.c $(HDRS)
	$(Q)$(CC) -g $(OPT_LEVEL) -DTESTING -I$(INC_DIR) $(CFLAGS) $(CXXFLAGS) $(GCOV) -c $< -o $@


# Clean up any temp/build files.
clean :
	$(Q)rm -f *.o *.a *.so *.so.* *.to *.dylib *.dll *.gcno *.gcda
