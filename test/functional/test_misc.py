###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
#

import base_test_class

# Tests for individual testcases that don't merit their own test suite.

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_tab_color01(self):
        self.run_exe_test('test_tab_color01')

    def test_firstsheet01(self):
        self.run_exe_test('test_firstsheet01')

    def test_hide01(self):
        self.run_exe_test('test_hide01')
