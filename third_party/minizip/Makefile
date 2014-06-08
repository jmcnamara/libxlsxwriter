###############################################################################
#
# Simplied Makefile to build the minizip objects for the libxlsxwriter library.
#

# Keep the output quiet by default.
Q=@
ifdef V
Q=
endif

CFLAGS = -O3

all: ioapi.o zip.o  ioapi.so zip.so

%.o  : %.c
	$(Q)$(CC)       -c $(CFLAGS) $<

%.so : %.c
	$(Q)$(CC) -fPIC -c $(CFLAGS) $< -o $@

clean:
	$(Q)/bin/rm -f *.o *.so
