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

    def test_set_column01(self):
        self.run_exe_test('test_set_column01')

    def test_set_column02(self):
        self.run_exe_test('test_set_column02', 'set_column01.xlsx')

    def test_set_column03(self):
        self.run_exe_test('test_set_column03')

    def test_set_column04(self):
        self.run_exe_test('test_set_column04')

    def test_set_column05(self):
        self.run_exe_test('test_set_column05')

    def test_set_column06(self):
        # TODO. Fix span calculation for this condition.
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<row r="13"']}
        self.run_exe_test('test_set_column06')

    def test_set_column07(self):
        self.run_exe_test('test_set_column07')

    def test_set_column08(self):
        # TODO. Fix span calculation for this condition.
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<row r="13"']}
        self.run_exe_test('test_set_column08')

    def test_set_column09(self):
        self.run_exe_test('test_set_column09')

    def test_set_column10(self):
        self.run_exe_test('test_set_column10', 'set_column01.xlsx')

    def test_set_column11(self):
        # TODO. Fix span calculation for this condition.
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<row r="13"']}
        self.run_exe_test('test_set_column11', 'set_column06.xlsx')
