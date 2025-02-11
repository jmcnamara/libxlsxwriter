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

# Directory variables.
INC_DIR        = ../../include
LIB_DIR        = ../../src

# Flags passed to the C compiler.
CFLAGS += -DTESTING -DCOLOR_OK -g -Wall -Wextra -Wno-unused-parameter $(GCOV)


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
SRCS += $(wildcard chartsheet/test*.c)
SRCS += $(wildcard vml/test*.c)
SRCS += $(wildcard comment/test*.c)
SRCS += $(wildcard metadata/test*.c)
SRCS += $(wildcard table/test*.c)
SRCS += $(wildcard rich_value/test*.c)
SRCS += $(wildcard rich_value_rel/test*.c)
SRCS += $(wildcard rich_value_types/test*.c)
SRCS += $(wildcard rich_value_structure/test*.c)
# End of SRCS

OBJS  = $(patsubst %.c,%.o,$(SRCS))

# Libs to link.
LIBS_A = $(LIB_DIR)/libxlsxwriter_test.a
LIBS_O = -lz
ifdef USE_SYSTEM_MINIZIP
LIBS_O += -lminizip
CFLAGS += -DUSE_SYSTEM_MINIZIP
endif
ifdef USE_OPENSSL_MD5
LIBS_O += -lcrypto
endif

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
	$(Q)$(MAKE) -C chartsheet
	$(Q)$(MAKE) -C vml
	$(Q)$(MAKE) -C comment
	$(Q)$(MAKE) -C metadata
	$(Q)$(MAKE) -C table
	$(Q)$(MAKE) -C rich_value
	$(Q)$(MAKE) -C rich_value_rel
	$(Q)$(MAKE) -C rich_value_types
	$(Q)$(MAKE) -C rich_value_structure
# END make all

clean :
	$(Q)rm -f $(TESTS) test_all *.o *.gcno *.gcda
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
	$(Q)$(MAKE) clean -C chartsheet
	$(Q)$(MAKE) clean -C vml
	$(Q)$(MAKE) clean -C comment
	$(Q)$(MAKE) clean -C metadata
	$(Q)$(MAKE) clean -C table
	$(Q)$(MAKE) clean -C rich_value
	$(Q)$(MAKE) clean -C rich_value_rel
	$(Q)$(MAKE) clean -C rich_value_types
	$(Q)$(MAKE) clean -C rich_value_structure
# END make clean


###############################################################################
#
# Builds the tests.
#
test_all : test_all.o $(OBJS) $(LIBS_A)
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) -I$(INC_DIR) -o $@ $^ $(LIBS_O)


###############################################################################
#
# Run the tests.
#
test : all test_all
	$(Q)./test_all
