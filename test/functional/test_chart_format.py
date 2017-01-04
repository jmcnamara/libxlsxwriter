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

    def test_chart_format01(self):
        self.run_exe_test('test_chart_format01')

    def test_chart_format02(self):
        self.run_exe_test('test_chart_format02')

    def test_chart_format03(self):
        self.run_exe_test('test_chart_format03')

    def test_chart_format04(self):
        self.run_exe_test('test_chart_format04')

    def test_chart_format05(self):
        self.run_exe_test('test_chart_format05')

    def test_chart_format06(self):
        self.run_exe_test('test_chart_format06')

    def test_chart_format07(self):
        self.run_exe_test('test_chart_format07')



    def test_chart_format09(self):
        self.run_exe_test('test_chart_format09')



    def test_chart_format17(self):
        self.run_exe_test('test_chart_format17')

    def test_chart_format18(self):
        self.run_exe_test('test_chart_format18')

    def test_chart_format21(self):
        self.run_exe_test('test_chart_format21')

    def test_chart_format22(self):
        self.run_exe_test('test_chart_format22')

    def test_chart_format23(self):
        self.run_exe_test('test_chart_format23')

    def test_chart_format24(self):
        self.run_exe_test('test_chart_format24')

    def test_chart_format25(self):
        self.run_exe_test('test_chart_format25')
