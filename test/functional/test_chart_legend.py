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

    def test_chart_legend01(self):
        self.run_exe_test('test_chart_legend01')

    # def test_chart_legend02(self):
    #     self.run_exe_test('test_chart_legend02')
    # This is just a duplicate of test_chart_legend01.

    def test_chart_legend03(self):
        self.run_exe_test('test_chart_legend03')

    def test_chart_legend04(self):
        self.run_exe_test('test_chart_legend04')


