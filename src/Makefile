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
SOFLAGS = -shared -fPIC

# Change shared lib options on OS X.
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
LIBXLSXWRITER_SO = libxlsxwriter.dylib
SOFLAGS = -dynamiclib -fPIC -install_name /usr/lib/$(LIBXLSXWRITER_SO)
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
	$(Q)$(AR) $(ARFLAGS)     $@ $(MINIZIP_DIR)/ioapi.o  $(MINIZIP_DIR)/zip.o  $^

# The dynamic library.
$(LIBXLSXWRITER_SO) : $(SOBJS)
	$(Q)$(CC) $(SOFLAGS)  -o $@ $(MINIZIP_DIR)/ioapi.so $(MINIZIP_DIR)/zip.so $^ -lz

# The test library.
$(LIBXLSXWRITER_TO) : $(TOBJS)
	$(Q)$(AR) $(ARFLAGS)     $@ $(MINIZIP_DIR)/ioapi.o  $(MINIZIP_DIR)/zip.o  $^

# Minimal target for quick compile without creating the libs.
test_compile : $(OBJS)

# Targets for the object files.
%.o  : %.c $(HDRS)
	$(Q)$(CC)       -I$(INC_DIR) $(CFLAGS) $(CXXFLAGS) -c $<

%.so : %.c $(HDRS)
	$(Q)$(CC) -fPIC -I$(INC_DIR) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

%.to : %.c $(HDRS)
	$(Q)$(CC) -g -O0 -DTESTING -I$(INC_DIR) $(CFLAGS) $(CXXFLAGS) -c $< -o $@


# Clean up any temp/build files.
clean :
	$(Q)rm -f *.o *.a *.so *.to *.dylib
