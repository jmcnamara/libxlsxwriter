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

    def test_chart_pattern01(self):
        self.run_exe_test('test_chart_pattern01')

    def test_chart_pattern02(self):
        self.run_exe_test('test_chart_pattern02')

    def test_chart_pattern03(self):
        self.run_exe_test('test_chart_pattern03')

    def test_chart_pattern04(self):
        self.run_exe_test('test_chart_pattern04')

    def test_chart_pattern05(self):
        self.run_exe_test('test_chart_pattern05')

    def test_chart_pattern06(self):
        self.run_exe_test('test_chart_pattern06')

    def test_chart_pattern07(self):
        self.run_exe_test('test_chart_pattern07')

    def test_chart_pattern08(self):
        self.run_exe_test('test_chart_pattern08')

    def test_chart_pattern10(self):
        self.run_exe_test('test_chart_pattern10')
