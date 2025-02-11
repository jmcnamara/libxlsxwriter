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

    def test_dynamic_array01(self):
        self.run_exe_test('test_dynamic_array01')

    def test_dynamic_array02(self):
        self.run_exe_test('test_dynamic_array02')

    def test_dynamic_array03(self):
        self.run_exe_test('test_dynamic_array03')

    # Some variant of the default test case.
    def test_dynamic_array51(self):
        self.run_exe_test('test_dynamic_array51', 'dynamic_array01.xlsx')

    def test_dynamic_array52(self):
        self.run_exe_test('test_dynamic_array52', 'dynamic_array01.xlsx')

    def test_dynamic_array53(self):
        self.run_exe_test('test_dynamic_array53', 'dynamic_array01.xlsx')
