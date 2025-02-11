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

    def test_chart_blank01(self):
        self.run_exe_test('test_chart_blank01')

    def test_chart_blank02(self):
        self.run_exe_test('test_chart_blank02')

    def test_chart_blank03(self):
        self.run_exe_test('test_chart_blank03')

    def test_chart_blank04(self):
        self.run_exe_test('test_chart_blank04')

    def test_chart_blank05(self):
        self.ignore_elements = {'xl/drawings/drawing1.xml': ['<xdr:ext']}
        self.run_exe_test('test_chart_blank05')

    def test_chart_blank06(self):
        self.run_exe_test('test_chart_blank06')
