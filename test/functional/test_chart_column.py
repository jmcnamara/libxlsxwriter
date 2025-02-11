###############################################################################
#
# Tests for libxlsxwriter.
#
# SPDX-License-Identifier: BSD-2-Clause
# Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_chart_column01(self):
        self.run_exe_test('test_chart_column01')

    def test_chart_column02(self):
        self.run_exe_test('test_chart_column02')

    def test_chart_column03(self):
        self.run_exe_test('test_chart_column03')

    def test_chart_column05(self):
        self.run_exe_test('test_chart_column05')

    def test_chart_column06(self):
        self.run_exe_test('test_chart_column06')

    def test_chart_column07(self):
        self.run_exe_test('test_chart_column07')

    def test_chart_column08(self):
        self.run_exe_test('test_chart_column08')

    def test_chart_column09(self):
        self.run_exe_test('test_chart_column09')

    def test_chart_column10(self):
        self.run_exe_test('test_chart_column10')

    def test_chart_column11(self):
        self.run_exe_test('test_chart_column11')

    def test_chart_column12(self):
        self.run_exe_test('test_chart_column12')

    def test_chart_column13(self):
        self.run_exe_test('test_chart_column13')
