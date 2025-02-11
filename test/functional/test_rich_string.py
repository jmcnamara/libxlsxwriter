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

    def test_rich_string01(self):
        self.run_exe_test('test_rich_string01')

    def test_rich_string02(self):
        self.run_exe_test('test_rich_string02')

    def test_rich_string03(self):
        self.run_exe_test('test_rich_string03')

    def test_rich_string04(self):
        self.run_exe_test('test_rich_string04')

    def test_rich_string05(self):
        self.run_exe_test('test_rich_string05')

    def test_rich_string06(self):
        self.run_exe_test('test_rich_string06')

    def test_rich_string07(self):
        self.run_exe_test('test_rich_string07')

    def test_rich_string08(self):
        self.run_exe_test('test_rich_string08')

    def test_rich_string09(self):
        self.run_exe_test('test_rich_string09')

    def test_rich_string10(self):
        self.run_exe_test('test_rich_string10')

    def test_rich_string11(self):
        self.run_exe_test('test_rich_string11')

    def test_rich_string12(self):
        self.run_exe_test('test_rich_string12')
