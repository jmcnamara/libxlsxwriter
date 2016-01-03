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

    def test_hyperlink01(self):
        self.run_exe_test('test_hyperlink01')

    def test_hyperlink02(self):
        self.run_exe_test('test_hyperlink02')

    def test_hyperlink03(self):
        self.run_exe_test('test_hyperlink03')

    def test_hyperlink04(self):
        self.run_exe_test('test_hyperlink04')

    def test_hyperlink05(self):
        self.run_exe_test('test_hyperlink05')

    def test_hyperlink06(self):
        self.run_exe_test('test_hyperlink06')

    def test_hyperlink07(self):
        self.run_exe_test('test_hyperlink07')

    def test_hyperlink08(self):
        self.run_exe_test('test_hyperlink08')

    def test_hyperlink09(self):
        self.run_exe_test('test_hyperlink09')

    def test_hyperlink10(self):
        self.run_exe_test('test_hyperlink10')

    def test_hyperlink11(self):
        self.run_exe_test('test_hyperlink11')

    def test_hyperlink12(self):
        self.run_exe_test('test_hyperlink12')

    def test_hyperlink13(self):
        # Ignore the count of duplicate strings.
        self.ignore_elements = {'xl/sharedStrings.xml': ['<sst']}
        self.run_exe_test('test_hyperlink13')

    def test_hyperlink14(self):
        # Ignore unused/overwritten strings.
        self.ignore_files = ['xl/sharedStrings.xml']
        self.run_exe_test('test_hyperlink14')

    def test_hyperlink15(self):
        self.run_exe_test('test_hyperlink15')

    def test_hyperlink16(self):
        self.run_exe_test('test_hyperlink16')

    def test_hyperlink17(self):
        self.run_exe_test('test_hyperlink17')

    def test_hyperlink18(self):
        self.run_exe_test('test_hyperlink18')

    # This test requires that we check if the cell that the hyperlink refers
    # to is a string. If it isn't be have to add a display attribute. However,
    # looking up the cell is currently too expensive.
    # 
    # def test_hyperlink19(self):
    #     self.ignore_files = ['xl/calcChain.xml',
    #                          '[Content_Types].xml',
    #                          'xl/_rels/workbook.xml.rels']
    #     self.run_exe_test('test_hyperlink19')

    def test_hyperlink20(self):
        # Ignore custom colours.
        self.ignore_files = ['xl/styles.xml']
        self.run_exe_test('test_hyperlink20')
