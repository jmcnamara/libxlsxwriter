###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_format01(self):
        self.run_exe_test('test_format01')

    def test_format02(self):
        self.run_exe_test('test_format02')

    def test_format06(self):
        self.run_exe_test('test_format06')

    def test_format07(self):
        self.run_exe_test('test_format07')

    def test_format08(self):
        self.run_exe_test('test_format08')

    def test_format09(self):
        self.run_exe_test('test_format09')

    def test_format10(self):
        self.run_exe_test('test_format10')

    def test_format12(self):
        self.run_exe_test('test_format12')
