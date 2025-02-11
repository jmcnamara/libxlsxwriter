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

    def test_comment01(self):
        self.run_exe_test('test_comment01')

    def test_comment02(self):
        self.run_exe_test('test_comment02')

    def test_comment03(self):
        self.run_exe_test('test_comment03')

    def test_comment04(self):
        self.run_exe_test('test_comment04')

    def test_comment05(self):
        self.run_exe_test('test_comment05')

    def test_comment06(self):
        self.run_exe_test('test_comment06')

    def test_comment07(self):
        self.run_exe_test('test_comment07')

    def test_comment08(self):
        self.run_exe_test('test_comment08')

    def test_comment09(self):
        self.run_exe_test('test_comment09')

    def test_comment10(self):
        self.run_exe_test('test_comment10')

    def test_comment11(self):
        self.run_exe_test('test_comment11')

    def test_comment12(self):
        self.run_exe_test('test_comment12')

    def test_comment13(self):
        self.ignore_files = ['xl/styles.xml']
        self.run_exe_test('test_comment13')

    def test_comment14(self):
        self.run_exe_test('test_comment14')

    def test_comment15(self):
        self.run_exe_test('test_comment15')

    def test_comment16(self):
        self.run_exe_test('test_comment16')

    # Memory leak test.
    def test_comment56(self):
        self.run_exe_test('test_comment56', 'comment16.xlsx')
