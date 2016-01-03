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

    # def test_escapes01(self):
    #     self.run_exe_test('test_escapes01')

    # def test_escapes02(self):
    #     self.run_exe_test('test_escapes02')

    # def test_escapes03(self):
    #     self.run_exe_test('test_escapes03')

    def test_escapes04(self):
        self.run_exe_test('test_escapes04')

    def test_escapes05(self):
        self.run_exe_test('test_escapes05')

    def test_escapes06(self):
        self.run_exe_test('test_escapes06')

    def test_escapes07(self):
        self.run_exe_test('test_escapes07')

    def test_escapes08(self):
        self.run_exe_test('test_escapes08')
