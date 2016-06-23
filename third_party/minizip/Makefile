###############################################################################
#
# Simplied Makefile to build the minizip objects for the libxlsxwriter library.
#

# Keep the output quiet by default.
Q=@
ifdef V
Q=
endif

UNAME := $(shell uname)

FPIC = -fPIC
CFLAGS = -O3 -DNOCRYPT -DNOUNCRYPT

# Change make options on MinGW.
ifneq (,$(findstring MINGW, $(UNAME)))
FPIC =
endif

# Change make options on Cygwin.
ifneq (,$(findstring CYGWIN, $(UNAME)))
FPIC =
CFLAGS += -DUSE_FILE32API
endif

all: ioapi.o zip.o  ioapi.so zip.so

%.o  : %.c
	$(Q)$(CC)         -c $(CFLAGS) $<

%.so : %.c
	$(Q)$(CC) $(FPIC) -c $(CFLAGS) $< -o $@

clean:
	$(Q)/bin/rm -f *.o *.so
