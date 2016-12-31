###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_chart_font01(self):
        self.run_exe_test('test_chart_font01')

    def test_chart_font02(self):
        self.run_exe_test('test_chart_font02')

    def test_chart_font03(self):
        self.run_exe_test('test_chart_font03')

    def test_chart_font04(self):
        self.run_exe_test('test_chart_font04')

    def test_chart_font05(self):
        self.run_exe_test('test_chart_font05')

    def test_chart_font06(self):
        self.run_exe_test('test_chart_font06')

    def test_chart_font07(self):
        self.run_exe_test('test_chart_font07')

    def test_chart_font08(self):
        self.run_exe_test('test_chart_font08')

    def test_chart_font09(self):
        self.run_exe_test('test_chart_font09')
