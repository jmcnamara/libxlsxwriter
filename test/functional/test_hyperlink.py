###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
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

