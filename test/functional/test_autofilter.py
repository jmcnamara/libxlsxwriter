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

    def test_autofilter00(self):
        self.run_exe_test('test_autofilter00')

    def test_autofilter01(self):
        self.run_exe_test('test_autofilter01')

    def test_autofilter02(self):
        self.run_exe_test('test_autofilter02')

    def test_autofilter03(self):
        self.run_exe_test('test_autofilter03')

    def test_autofilter04(self):
        self.run_exe_test('test_autofilter04')

    def test_autofilter05(self):
        self.run_exe_test('test_autofilter05')

    def test_autofilter06(self):
        self.run_exe_test('test_autofilter06')

    def test_autofilter07(self):
        self.run_exe_test('test_autofilter07')

    def test_autofilter08(self):
        self.run_exe_test('test_autofilter08')

    def test_autofilter09(self):
        self.run_exe_test('test_autofilter09')

    def test_autofilter10(self):
        self.run_exe_test('test_autofilter10')

    def test_autofilter11(self):
        self.run_exe_test('test_autofilter11')
