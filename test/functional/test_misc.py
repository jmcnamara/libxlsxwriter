###############################################################################
#
# Tests for libxlsxwriter.
#
# SPDX-License-Identifier: BSD-2-Clause
# Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
#

import base_test_class

# Tests for individual testcases that don't merit their own test suite.

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    # Tests for cases that don't deserve a full test_*.py of their own.

    def test_tab_color01(self):
        self.run_exe_test('test_tab_color01')

    def test_firstsheet01(self):
        self.run_exe_test('test_firstsheet01')

    def test_hide01(self):
        self.run_exe_test('test_hide01')

    def test_shared_strings01(self):
        self.run_exe_test('test_shared_strings01')

    def test_gh42_01(self):
        self.run_exe_test('test_gh42_01')

    def test_gh42_02(self):
        self.run_exe_test('test_gh42_02')

    def test_chart_legend01(self):
        self.run_exe_test('test_chart_legend01')

    def test_header04(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageSetup']}
        self.run_exe_test('test_header04')
