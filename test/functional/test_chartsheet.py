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

    def test_chartsheet01(self):
        self.run_exe_test('test_chartsheet01')

    def test_chartsheet02(self):
        self.run_exe_test('test_chartsheet02')

    def test_chartsheet03(self):
        self.run_exe_test('test_chartsheet03')

    def test_chartsheet04(self):
        self.run_exe_test('test_chartsheet04')

    def test_chartsheet05(self):
        self.run_exe_test('test_chartsheet05')

    def test_chartsheet06(self):
        self.run_exe_test('test_chartsheet06')

    def test_chartsheet07(self):
        self.run_exe_test('test_chartsheet07')

    def test_chartsheet08(self):
        self.ignore_files = ['xl/drawings/drawing1.xml']
        self.run_exe_test('test_chartsheet08')

    def test_chartsheet09(self):
        self.run_exe_test('test_chartsheet09')
