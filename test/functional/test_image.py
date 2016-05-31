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
    # self.ignore_files = ['xl/drawings/drawing1.xml']

    def test_image01(self):
        self.run_exe_test('test_image01')

    def test_image02(self):
        self.run_exe_test('test_image02')

    def test_image03(self):
        self.run_exe_test('test_image03')

    def test_image04(self):
        self.run_exe_test('test_image04')

    def test_image05(self):
        self.run_exe_test('test_image05')

    def test_image07(self):
        self.run_exe_test('test_image07')

    def test_image08(self):
        self.run_exe_test('test_image08')

    def test_image09(self):
        self.run_exe_test('test_image09')

    def test_image10(self):
        self.run_exe_test('test_image10')

    def test_image11(self):
        self.run_exe_test('test_image11')

    def test_image12(self):
        self.run_exe_test('test_image12')

    def test_image13(self):
        self.run_exe_test('test_image13')

    def test_image14(self):
        self.run_exe_test('test_image14')

    def test_image15(self):
        self.run_exe_test('test_image15')

    def test_image16(self):
        self.run_exe_test('test_image16')

    def test_image17(self):
        self.run_exe_test('test_image17')

    def test_image18(self):
        self.run_exe_test('test_image18')

    def test_image19(self):
        self.run_exe_test('test_image19')

    def test_image22(self):
        self.run_exe_test('test_image22')

    def test_image23(self):
        self.run_exe_test('test_image23')

    def test_image24(self):
        self.run_exe_test('test_image24')

    def test_image25(self):
        self.run_exe_test('test_image25')

    def test_image26(self):
        self.run_exe_test('test_image26')

    def test_image27(self):
        self.run_exe_test('test_image27')

    def test_image28(self):
        self.run_exe_test('test_image28')

    def test_image29(self):
        self.run_exe_test('test_image29')

    def test_image30(self):
        self.run_exe_test('test_image30')

    def test_image31(self):
        self.run_exe_test('test_image31')

    def test_image32(self):
        self.run_exe_test('test_image32')

    def test_image33(self):
        self.run_exe_test('test_image33')

    def test_image34(self):
        self.run_exe_test('test_image34')

    def test_image35(self):
        self.run_exe_test('test_image35')
