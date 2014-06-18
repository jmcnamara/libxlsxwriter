###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright (c), 2014, John McNamara, jmcnamara@cpan.org
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_row_colformat01(self):
        self.run_exe_test('test_row_col_format01')

    def test_row_colformat02(self):
        self.run_exe_test('test_row_col_format02')

    def test_row_colformat03(self):
        self.run_exe_test('test_row_col_format03')

    def test_row_colformat04(self):
        self.run_exe_test('test_row_col_format04')

    def test_row_colformat05(self):
        self.run_exe_test('test_row_col_format05')

    def test_row_colformat06(self):
        self.run_exe_test('test_row_col_format06')

    def test_row_colformat07(self):
        self.run_exe_test('test_row_col_format07')

    def test_row_colformat08(self):
        self.run_exe_test('test_row_col_format08')

    def test_row_colformat09(self):
        self.run_exe_test('test_row_col_format09')

    def test_row_colformat10(self):
        self.run_exe_test('test_row_col_format10')

    def test_row_colformat11(self):
        self.run_exe_test('test_row_col_format11')

