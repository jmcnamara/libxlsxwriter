###############################################################################
#
# Makefile for libxlsxwriter library.
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
#

# Keep the output quiet by default.
Q=@
ifdef V
Q=
endif

# Directory variables.
OBJS_DIR    = .
INC_DIR     = ../include

# The minizip directory.
MINIZIP_DIR = ../third_party/minizip


ifdef USE_STANDARD_TMPFILE
# Use standard/C tmpfile().
CFLAGS += -DUSE_STANDARD_TMPFILE
else
# Use tmpfileplus (the default).
TMPFILEPLUS_DIR = ../third_party/tmpfileplus
TMPFILEPLUS_OBJ = $(TMPFILEPLUS_DIR)/tmpfileplus.o
TMPFILEPLUS_SO  = $(TMPFILEPLUS_DIR)/tmpfileplus.so
endif

# Flags passed to compiler.
CFLAGS   += -g -O3 -Wall -Wextra -pedantic -ansi

# Library names.
LIBXLSXWRITER_A  = libxlsxwriter.a
LIBXLSXWRITER_SO = libxlsxwriter.so

# Library with additional non-static functions for testing.
LIBXLSXWRITER_TO = libxlsxwriter_test.a

# Flags passed to static linker.
ARFLAGS = rc

# Flags passed to dynamic linker.
FPIC = -fPIC
SOFLAGS = -shared $(FPIC)

# Get Env/OS name.
UNAME := $(shell uname)

# Change make options on OS X.
ifeq ($(UNAME), Darwin)
LIBXLSXWRITER_SO = libxlsxwriter.dylib
SOFLAGS = -dynamiclib $(FPIC) -install_name /usr/lib/$(LIBXLSXWRITER_SO)
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

test_lib : libxlsxwriter_test.a

# The static library.
$(LIBXLSXWRITER_A) : $(OBJS)
	$(Q)$(AR) $(ARFLAGS) $@ $(MINIZIP_DIR)/ioapi.o  $(MINIZIP_DIR)/zip.o $(TMPFILEPLUS_OBJ) $^

# The dynamic library.
ifeq ($(findstring m32,$(CFLAGS)),m32)
ARCH = -m32
endif

$(LIBXLSXWRITER_SO) : $(SOBJS)
	$(Q)$(CC) $(SOFLAGS) $(ARCH) -o $@ $(MINIZIP_DIR)/ioapi.so $(MINIZIP_DIR)/zip.so $(TMPFILEPLUS_SO) $^ -lz

# The test library.
$(LIBXLSXWRITER_TO) : $(TOBJS)
	$(Q)$(AR) $(ARFLAGS) $@ $(MINIZIP_DIR)/ioapi.o  $(MINIZIP_DIR)/zip.o  $(TMPFILEPLUS_OBJ) $^

# Minimal target for quick compile without creating the libs.
test_compile : $(OBJS)

# Targets for the object files.
%.o  : %.c $(HDRS)
	$(Q)$(CC) -I$(INC_DIR) $(CFLAGS) $(CXXFLAGS) -c $<

%.so : %.c $(HDRS)
	$(Q)$(CC) $(FPIC) -I$(INC_DIR) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

%.to : %.c $(HDRS)
	$(Q)$(CC) -g -O0 -DTESTING -I$(INC_DIR) $(CFLAGS) $(CXXFLAGS) -c $< -o $@


# Clean up any temp/build files.
clean :
	$(Q)rm -f *.o *.a *.so *.to *.dylib *.dll
