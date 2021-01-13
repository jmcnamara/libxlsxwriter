###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_chart_line01(self):
        self.run_exe_test('test_chart_line01')

    def test_chart_line03(self):
        self.run_exe_test('test_chart_line03')

    def test_chart_line04(self):
        self.run_exe_test('test_chart_line04')

    def test_chart_line05(self):
        self.run_exe_test('test_chart_line05')

    def test_chart_line06(self):
        self.run_exe_test('test_chart_line06')
