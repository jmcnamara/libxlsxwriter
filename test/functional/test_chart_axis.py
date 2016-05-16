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

    def test_chart_axis01(self):
        self.run_exe_test('test_chart_axis01')

    def test_chart_axis02(self):
        self.run_exe_test('test_chart_axis02')

    def test_chart_axis04(self):
        self.run_exe_test('test_chart_axis04')

    def test_chart_axis05(self):
        self.run_exe_test('test_chart_axis05')

    def test_chart_axis06(self):
        self.run_exe_test('test_chart_axis06')

    def test_chart_axis07(self):
        self.run_exe_test('test_chart_axis07')
