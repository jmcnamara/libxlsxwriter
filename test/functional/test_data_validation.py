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

    def test_data_validation01(self):
        self.run_exe_test('test_data_validation01')

    def test_data_validation02(self):
        self.run_exe_test('test_data_validation02')

    def test_data_validation03(self):
        self.run_exe_test('test_data_validation03')

    def test_data_validation04(self):
        self.run_exe_test('test_data_validation04', 'data_validation02.xlsx')

    def test_data_validation05(self):
        self.run_exe_test('test_data_validation05', 'data_validation02.xlsx')

    def test_data_validation06(self):
        self.run_exe_test('test_data_validation06', 'data_validation02.xlsx')

    def test_data_validation07(self):
        self.run_exe_test('test_data_validation07')

    def test_data_validation08(self):
        self.run_exe_test('test_data_validation08')
