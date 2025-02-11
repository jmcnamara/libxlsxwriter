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

    # Requires autofilter and fit_to_pages.

    def test_defined_name01(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins']}
        self.run_exe_test('test_defined_name01')

    def test_defined_name02(self):
        self.run_exe_test('test_defined_name02')

    def test_defined_name03(self):
        self.run_exe_test('test_defined_name03')

    def test_defined_name04(self):
        self.run_exe_test('test_defined_name04')

