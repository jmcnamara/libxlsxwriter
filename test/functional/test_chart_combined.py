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

    def test_chart_combined01(self):
        self.run_exe_test('test_chart_combined01')

    def test_chart_combined02(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:dispBlanksAs']}
        self.run_exe_test('test_chart_combined02')

    def test_chart_combined03(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:dispBlanksAs']}
        self.run_exe_test('test_chart_combined03')

    def test_chart_combined04(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:dispBlanksAs', '<c:tickLblPos', '<c:crosses']}
        self.run_exe_test('test_chart_combined04')

    def test_chart_combined05(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:dispBlanksAs', '<c:tickLblPos', '<c:crosses', '<c:axPos']}
        self.run_exe_test('test_chart_combined05')

    def test_chart_combined06(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:dispBlanksAs']}
        self.run_exe_test('test_chart_combined06')

    def test_chart_combined07(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:dispBlanksAs']}
        self.run_exe_test('test_chart_combined07')

    def test_chart_combined08(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:dispBlanksAs', '<c:crossBetween', '<c:tickLblPos', '<c:auto', '<c:valAx>', '<c:catAx>', '</c:valAx>', '</c:catAx>', '<c:crosses', '<c:lblOffset', '<c:lblAlgn']}
        self.run_exe_test('test_chart_combined08')

    def test_chart_combined09(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:dispBlanksAs', '<c:tickLblPos', '<c:crosses']}
        self.run_exe_test('test_chart_combined09')

    def test_chart_combined10(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:dispBlanksAs']}
        self.run_exe_test('test_chart_combined10')

    def test_chart_combined11(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:dispBlanksAs']}
        self.run_exe_test('test_chart_combined11')
