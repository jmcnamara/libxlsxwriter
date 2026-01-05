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

    def test_chart_stock01(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:formatCode']}
        self.run_exe_test('test_chart_stock01')

    def test_chart_stock02(self):
        self.ignore_elements = {
            'xl/charts/chart1.xml': ['<c:formatCode'],
            'xl/workbook.xml': ['<fileVersion', '<calcPr'],
            'xl/worksheets/sheet1.xml': [
                '<sheetView',
                '<selection activeCell',
                '</sheetView',
            ],
        }
        self.run_exe_test('test_chart_stock02')

    def test_chart_stock03(self):
        self.ignore_elements = {'xl/charts/chart1.xml': ['<c:formatCode']}
        self.run_exe_test('test_chart_stock03')
