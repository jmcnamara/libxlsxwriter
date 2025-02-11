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

    def test_object_position01(self):
        self.run_exe_test('test_object_position01')

    def test_object_position02(self):
        self.run_exe_test('test_object_position02')

    def test_object_position03(self):
        self.run_exe_test('test_object_position03')

    def test_object_position04(self):
        self.run_exe_test('test_object_position04')

    def test_object_position06(self):
        self.run_exe_test('test_object_position06')

    def test_object_position07(self):
        self.run_exe_test('test_object_position07')

    def test_object_position08(self):
        self.ignore_files = ['xl/worksheets/sheet1.xml']
        self.run_exe_test('test_object_position08')

    def test_object_position09(self):
        self.ignore_files = ['xl/worksheets/sheet1.xml']
        self.run_exe_test('test_object_position09')

    def test_object_position10(self):
        self.run_exe_test('test_object_position10')

    def test_object_position12(self):
        self.run_exe_test('test_object_position12')

    def test_object_position13(self):
        self.run_exe_test('test_object_position13')

    def test_object_position14(self):
        self.run_exe_test('test_object_position14')

    def test_object_position15(self):
        self.run_exe_test('test_object_position15')

    def test_object_position16(self):
        self.run_exe_test('test_object_position16')

    def test_object_position17(self):
        self.run_exe_test('test_object_position17')

    def test_object_position18(self):
        self.run_exe_test('test_object_position18')

    def test_object_position19(self):
        self.run_exe_test('test_object_position19')

    def test_object_position20(self):
        self.run_exe_test('test_object_position20')

    # Test in-memory image handling.
    def test_object_position51(self):
        self.run_exe_test('test_object_position51', 'object_position01.xlsx')
