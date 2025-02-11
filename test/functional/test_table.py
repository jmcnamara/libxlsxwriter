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

    def test_table01(self):
        self.run_exe_test('test_table01')

    def test_table02(self):
        self.run_exe_test('test_table02')

    def test_table03(self):
        self.run_exe_test('test_table03')

    def test_table04(self):
        self.run_exe_test('test_table04')

    def test_table05(self):
        self.run_exe_test('test_table05')

    def test_table06(self):
        self.run_exe_test('test_table06')

    def test_table07(self):
        self.run_exe_test('test_table07')

    def test_table08(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_table08')

    def test_table09(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_table09')

    def test_table10(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_table10')

    def test_table11(self):
        self.run_exe_test('test_table11')

    def test_table12(self):
        self.run_exe_test('test_table12')

    # Skip test_table13 since we can't set the dxf index for
    # format_set_num_format_index() formats with C hash struct.

    def test_table14(self):
        self.run_exe_test('test_table14')

    def test_table15(self):
        self.run_exe_test('test_table15')

    def test_table16(self):
        self.run_exe_test('test_table16', 'table02.xlsx')

    def test_table17(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_table17')

    def test_table18(self):
        self.run_exe_test('test_table18')

    def test_table19(self):
        self.run_exe_test('test_table19')

    # TODO. Duplicate names.

    def test_table21(self):
        self.run_exe_test('test_table21')

    def test_table22(self):
        self.run_exe_test('test_table22')

    def test_table23(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_table23')

    def test_table24(self):
        self.run_exe_test('test_table24')

    def test_table25(self):
        self.run_exe_test('test_table25')

    def test_table26(self):
        self.run_exe_test('test_table26')

    def test_table29(self):
        self.run_exe_test('test_table29')

    def test_table30(self):
        self.run_exe_test('test_table30')
