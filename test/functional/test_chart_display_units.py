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

    def test_chart_display_units01(self):
        self.run_exe_test('test_chart_display_units01')

    def test_chart_display_units02(self):
        self.run_exe_test('test_chart_display_units02')

    def test_chart_display_units03(self):
        self.run_exe_test('test_chart_display_units03')

    def test_chart_display_units04(self):
        self.run_exe_test('test_chart_display_units04')

    def test_chart_display_units05(self):
        self.run_exe_test('test_chart_display_units05')

    def test_chart_display_units06(self):
        self.run_exe_test('test_chart_display_units06')

    def test_chart_display_units07(self):
        self.run_exe_test('test_chart_display_units07')

    def test_chart_display_units08(self):
        self.run_exe_test('test_chart_display_units08')

    def test_chart_display_units09(self):
        self.run_exe_test('test_chart_display_units09')

    def test_chart_display_units10(self):
        self.run_exe_test('test_chart_display_units10')

    def test_chart_display_units11(self):
        self.run_exe_test('test_chart_display_units11')

    def test_chart_display_units12(self):
        self.run_exe_test('test_chart_display_units12')
