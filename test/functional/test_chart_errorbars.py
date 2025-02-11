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

    def test_chart_errorbars01(self):
        self.run_exe_test('test_chart_errorbars01')

    def test_chart_errorbars02(self):
        self.run_exe_test('test_chart_errorbars02')

    def test_chart_errorbars03(self):
        self.run_exe_test('test_chart_errorbars03')

    def test_chart_errorbars04(self):
        self.run_exe_test('test_chart_errorbars04')

    def test_chart_errorbars05(self):
        self.run_exe_test('test_chart_errorbars05')

    def test_chart_errorbars06(self):
        self.run_exe_test('test_chart_errorbars06')
