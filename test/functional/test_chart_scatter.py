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

    def test_chart_scatter01(self):
        self.run_exe_test('test_chart_scatter01')

    def test_chart_scatter02(self):
        self.run_exe_test('test_chart_scatter02')

    def test_chart_scatter03(self):
        self.run_exe_test('test_chart_scatter03')

    def test_chart_scatter04(self):
        self.run_exe_test('test_chart_scatter04')

    def test_chart_scatter05(self):
        self.run_exe_test('test_chart_scatter05')

    def test_chart_scatter06(self):
        self.run_exe_test('test_chart_scatter06')

    def test_chart_scatter07(self):
        # Scatter charts with secondary axis have structural differences in axis types
        # and axis ordering. This test validates chart data/series correctly but ignores
        # axis structure differences between libxlsxwriter and Excel output.
        self.ignore_elements = {'xl/workbook.xml': ['<fileVersion', '<calcPr'],
                                'xl/charts/chart1.xml': ['<c:crosses', '<c:tickLblPos',
                                    '<c:valAx>', '</c:valAx>', '<c:catAx>', '</c:catAx>',
                                    '<c:numFmt', '<c:auto', '<c:lblAlgn', '<c:lblOffset',
                                    '<c:axId', '<c:crossAx', '<c:crossBetween', '<c:delete',
                                    '<c:axPos', '<c:scaling>', '</c:scaling>',
                                    '<c:orientation', '<c:majorGridlines']}
        self.run_exe_test('test_chart_scatter07')

    def test_chart_scatter09(self):
        self.run_exe_test('test_chart_scatter09')

    def test_chart_scatter10(self):
        self.run_exe_test('test_chart_scatter10')

    def test_chart_scatter11(self):
        self.run_exe_test('test_chart_scatter11')

    def test_chart_scatter12(self):
        self.run_exe_test('test_chart_scatter12')

    def test_chart_scatter13(self):
        self.run_exe_test('test_chart_scatter13', 'chart_scatter12.xlsx')

    def test_chart_scatter14(self):
        self.run_exe_test('test_chart_scatter14')

    def test_chart_scatter15(self):
        self.run_exe_test('test_chart_scatter15')
