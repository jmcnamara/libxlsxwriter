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

    def test_chart_data_labels01(self):
        self.run_exe_test('test_chart_data_labels01')

    def test_chart_data_labels02(self):
        self.run_exe_test('test_chart_data_labels02')

    def test_chart_data_labels03(self):
        self.run_exe_test('test_chart_data_labels03')

    def test_chart_data_labels04(self):
        self.run_exe_test('test_chart_data_labels04')

    def test_chart_data_labels05(self):
        self.run_exe_test('test_chart_data_labels05')

    def test_chart_data_labels06(self):
        self.run_exe_test('test_chart_data_labels06')

    def test_chart_data_labels07(self):
        self.run_exe_test('test_chart_data_labels07')

    def test_chart_data_labels08(self):
        self.run_exe_test('test_chart_data_labels08')

    def test_chart_data_labels09(self):
        self.run_exe_test('test_chart_data_labels09')

    def test_chart_data_labels10(self):
        self.run_exe_test('test_chart_data_labels10')
