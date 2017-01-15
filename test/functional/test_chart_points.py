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

    def test_chart_points01(self):
        self.run_exe_test('test_chart_points01')

    def test_chart_points02(self):
        self.run_exe_test('test_chart_points02')

    def test_chart_points03(self):
        # Ignore custom colors in styles.xml file.
        self.ignore_files = ['xl/styles.xml']
        self.run_exe_test('test_chart_points03')

    def test_chart_points04(self):
        self.run_exe_test('test_chart_points04')

    def test_chart_points05(self):
        self.run_exe_test('test_chart_points05')

    def test_chart_points06(self):
        self.run_exe_test('test_chart_points06')
