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

    def test_button01(self):
        self.run_exe_test('test_button01')

    def test_button02(self):
        self.run_exe_test('test_button02')

    def test_button03(self):
        self.run_exe_test('test_button03')

    def test_button04(self):
        self.run_exe_test('test_button04')

    def test_button05(self):
        self.run_exe_test('test_button05')

    def test_button06(self):
        self.run_exe_test('test_button06', 'button05.xlsx')

    def test_button07(self):
        self.run_exe_test('test_button07', 'button07.xlsm')

    def test_button08(self):
        self.run_exe_test('test_button08')

    def test_button09(self):
        self.run_exe_test('test_button09')

    def test_button10(self):
        self.run_exe_test('test_button10')

    def test_button11(self):
        self.run_exe_test('test_button11')

    def test_button12(self):
        self.run_exe_test('test_button12')

    def test_button13(self):
        self.run_exe_test('test_button13', 'button07.xlsm')

    def test_button14(self):
        self.run_exe_test('test_button14', 'button07.xlsm')

    def test_button15(self):
        self.run_exe_test('test_button15')
