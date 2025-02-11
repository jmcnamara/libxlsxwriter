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

    def test_chart_pie01(self):
        self.run_exe_test('test_chart_pie01')

    def test_chart_pie02(self):
        self.run_exe_test('test_chart_pie02')

    def test_chart_pie03(self):
        self.run_exe_test('test_chart_pie03')

    def test_chart_pie04(self):
        self.run_exe_test('test_chart_pie04')

    def test_chart_pie05(self):
        self.run_exe_test('test_chart_pie05')
