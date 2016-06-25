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
INC_DIR        = ../../include
LIB_DIR        = ../../src

# Flags passed to the C compiler.
CFLAGS += -DTESTING -DCOLOR_OK -g -Wall -Wextra -Wno-unused-parameter


# All tests produced by this Makefile.
TESTS = test_all

# Objects to link for test_all executable.
SRCS  = $(wildcard utility/test*.c)
SRCS += $(wildcard xmlwriter/test*.c)
SRCS += $(wildcard worksheet/test*.c)
SRCS += $(wildcard sst/test*.c)
SRCS += $(wildcard workbook/test*.c)
SRCS += $(wildcard app/test*.c)
SRCS += $(wildcard content_types/test*.c)
SRCS += $(wildcard core/test*.c)
SRCS += $(wildcard relationships/test*.c)
SRCS += $(wildcard format/test*.c)
SRCS += $(wildcard styles/test*.c)
SRCS += $(wildcard drawing/test*.c)
SRCS += $(wildcard chart/test*.c)
SRCS += $(wildcard custom/test*.c)
# End of SRCS

OBJS  = $(patsubst %.c,%.o,$(SRCS))

# Libs to link.
LIBS_A = $(LIB_DIR)/libxlsxwriter_test.a
LIBS_O = -lz
# End of LIBS

# House-keeping build targets.
all :
	$(Q)$(MAKE) -C utility
	$(Q)$(MAKE) -C xmlwriter
	$(Q)$(MAKE) -C worksheet
	$(Q)$(MAKE) -C sst
	$(Q)$(MAKE) -C workbook
	$(Q)$(MAKE) -C app
	$(Q)$(MAKE) -C content_types
	$(Q)$(MAKE) -C core
	$(Q)$(MAKE) -C relationships
	$(Q)$(MAKE) -C styles
	$(Q)$(MAKE) -C drawing
	$(Q)$(MAKE) -C chart
	$(Q)$(MAKE) -C custom
# END make all

clean :
	$(Q)rm -f $(TESTS) test_all *.o
	$(Q)$(MAKE) clean -C utility
	$(Q)$(MAKE) clean -C xmlwriter
	$(Q)$(MAKE) clean -C worksheet
	$(Q)$(MAKE) clean -C sst
	$(Q)$(MAKE) clean -C workbook
	$(Q)$(MAKE) clean -C app
	$(Q)$(MAKE) clean -C content_types
	$(Q)$(MAKE) clean -C core
	$(Q)$(MAKE) clean -C relationships
	$(Q)$(MAKE) clean -C styles
	$(Q)$(MAKE) clean -C drawing
	$(Q)$(MAKE) clean -C chart
	$(Q)$(MAKE) clean -C custom
# END make clean


###############################################################################
#
# Builds the tests.
#
test_all : test_all.o $(OBJS) $(LIBS_A)
	$(Q)$(CC) $(CFLAGS) -I$(INC_DIR) -o $@ $^ $(LIBS_O)


###############################################################################
#
# Run the tests.
#
test : all test_all
	$(Q)./test_all

