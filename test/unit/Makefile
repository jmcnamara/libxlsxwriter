###############################################################################
#
# Makefile for libxlsxwriter library.
#
# Copyright 2014, John McNamara, jmcnamara@cpan.org
#

# Keep the output quiet by default.
Q=@
ifdef V
Q=
endif

# Directory variables.
INC_DIR        = ../../include
LIB_DIR        = ../../src$
GTEST_DIR      = /usr/lib

# Conditional includes for testing.
TESTING = -DTESTING

# Flags passed to the preprocessor.
CPPFLAGS += $(TESTING)

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra


# All tests produced by this Makefile.
TESTS = test_all

# Objects to link for test_all executable.
SRCS  = $(wildcard utility/test*.cpp)
SRCS += $(wildcard xmlwriter/test*.cpp)
SRCS += $(wildcard worksheet/test*.cpp)
SRCS += $(wildcard sst/test*.cpp)
SRCS += $(wildcard workbook/test*.cpp)
SRCS += $(wildcard app/test*.cpp)
SRCS += $(wildcard content_types/test*.cpp)
SRCS += $(wildcard core/test*.cpp)
SRCS += $(wildcard relationships/test*.cpp)
SRCS += $(wildcard format/test*.cpp)
SRCS += $(wildcard styles/test*.cpp)
# End of SRCS

OBJS  = $(patsubst %.cpp,%.o,$(SRCS))

# Libs to link.
LIBS_A = $(LIB_DIR)/libxlsxwriter_test.a $(GTEST_DIR)/libgtest.a $(GTEST_DIR)/libgtest_main.a
LIBS_O = -lpthread -lz
# End of LIBS

# House-keeping build targets.
all :
	$(Q)make -C utility
	$(Q)make -C xmlwriter
	$(Q)make -C worksheet
	$(Q)make -C sst
	$(Q)make -C workbook
	$(Q)make -C app
	$(Q)make -C content_types
	$(Q)make -C core
	$(Q)make -C relationships
	$(Q)make -C styles
# END make all

clean :
	$(Q)rm -f $(TESTS) test_all
	$(Q)make clean -C utility
	$(Q)make clean -C xmlwriter
	$(Q)make clean -C worksheet
	$(Q)make clean -C sst
	$(Q)make clean -C workbook
	$(Q)make clean -C app
	$(Q)make clean -C content_types
	$(Q)make clean -C core
	$(Q)make clean -C relationships
	$(Q)make clean -C styles
# END make clean


###############################################################################
#
# Builds the tests.
#
test_all : $(OBJS) $(LIBS_A)
	$(Q)$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INC_DIR) -o $@ $^ $(LIBS_O)


###############################################################################
#
# Run the tests.
#
test : all test_all
	$(Q)./test_all

