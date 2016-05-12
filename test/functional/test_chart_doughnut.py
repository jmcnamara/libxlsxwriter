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

    def test_chart_doughnut01(self):
        self.run_exe_test('test_chart_doughnut01')

    def test_chart_doughnut02(self):
        self.run_exe_test('test_chart_doughnut02')

    def test_chart_doughnut03(self):
        self.run_exe_test('test_chart_doughnut03')

    def test_chart_doughnut04(self):
        self.run_exe_test('test_chart_doughnut04')

    def test_chart_doughnut05(self):
        self.run_exe_test('test_chart_doughnut05')

    def test_chart_doughnut06(self):
        self.run_exe_test('test_chart_doughnut06')
