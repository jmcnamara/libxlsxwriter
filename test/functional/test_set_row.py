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

    def test_set_row01(self):
        self.run_exe_test('test_set_row01')


    def test_set_row02(self):
        self.run_exe_test('test_set_row02', 'set_row01.xlsx')

    def test_set_row03(self):
        self.run_exe_test('test_set_row03')


    def test_set_row04(self):
        self.run_exe_test('test_set_row04', 'set_row03.xlsx')
