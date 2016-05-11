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

    def test_chart_scatter01(self):
        self.run_exe_test('test_chart_scatter01')

    def test_chart_scatter02(self):
        self.run_exe_test('test_chart_scatter02')

    def test_chart_scatter03(self):
        self.run_exe_test('test_chart_scatter03')

    def test_chart_scatter04(self):
        self.run_exe_test('test_chart_scatter04')

    def test_chart_scatter05(self):
        self.run_exe_test('test_chart_scatter05')
