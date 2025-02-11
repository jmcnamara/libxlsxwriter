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

    def test_chart_gridlines01(self):
        self.run_exe_test('test_chart_gridlines01')

    def test_chart_gridlines02(self):
        self.run_exe_test('test_chart_gridlines02')

    # test_chart_gridlines03 is a stock chart.

    def test_chart_gridlines04(self):
        self.run_exe_test('test_chart_gridlines04')

    def test_chart_gridlines05(self):
        self.run_exe_test('test_chart_gridlines05')

    def test_chart_gridlines06(self):
        self.run_exe_test('test_chart_gridlines06')

    # test_chart_gridlines07 is a stock chart.

    def test_chart_gridlines08(self):
        self.run_exe_test('test_chart_gridlines08')

    def test_chart_gridlines09(self):
        self.run_exe_test('test_chart_gridlines09')
