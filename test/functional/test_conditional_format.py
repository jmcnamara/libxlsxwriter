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

    def test_cond_format01(self):
        self.run_exe_test('test_cond_format01')

    def test_cond_format02(self):
        self.run_exe_test('test_cond_format02')

    def test_cond_format03(self):
        self.run_exe_test('test_cond_format03')

    def test_cond_format04(self):
        self.run_exe_test('test_cond_format04')

    def test_cond_format05(self):
        self.run_exe_test('test_cond_format05')

    def test_cond_format06(self):
        self.run_exe_test('test_cond_format06')

    def test_cond_format07(self):
        self.run_exe_test('test_cond_format07')

    def test_cond_format08(self):
        self.run_exe_test('test_cond_format08')

    def test_cond_format09(self):
        self.run_exe_test('test_cond_format09', 'cond_format08.xlsx')

    def test_cond_format10(self):
        self.run_exe_test('test_cond_format10')

    def test_cond_format11(self):
        self.run_exe_test('test_cond_format11')

    def test_cond_format12(self):
        self.run_exe_test('test_cond_format12')

    def test_cond_format13(self):
        self.run_exe_test('test_cond_format13', 'cond_format04.xlsx')

    def test_cond_format14(self):
        self.run_exe_test('test_cond_format14')

    def test_cond_format15(self):
        self.run_exe_test('test_cond_format15')

    def test_cond_format16(self):
        self.run_exe_test('test_cond_format16')

    def test_cond_format17(self):
        self.run_exe_test('test_cond_format17')
