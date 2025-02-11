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

    def test_quote_name01(self):
        self.run_exe_test('test_quote_name01')

    def test_quote_name02(self):
        self.run_exe_test('test_quote_name02')

    def test_quote_name03(self):
        self.run_exe_test('test_quote_name03')

    def test_quote_name04(self):
        self.run_exe_test('test_quote_name04')

    def test_quote_name05(self):
        self.run_exe_test('test_quote_name05')

    def test_quote_name06(self):
        self.run_exe_test('test_quote_name06')

    def test_quote_name07(self):
        self.run_exe_test('test_quote_name07')
