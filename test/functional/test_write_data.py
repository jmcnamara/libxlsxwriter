###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_data01(self):
        self.run_exe_test('test_data01')

    def test_data02(self):
        self.run_exe_test('test_data02')

    def test_data03(self):
        self.run_exe_test('test_data03')

    def test_data04(self):
        self.run_exe_test('test_data04')

    def test_data05(self):
        self.run_exe_test('test_data05')

    def test_data06(self):
        self.run_exe_test('test_data06')

    def test_data07(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_data07')
