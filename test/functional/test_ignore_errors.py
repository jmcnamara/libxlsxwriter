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

    def test_ignore_error01(self):
        self.run_exe_test('test_ignore_error01')

    def test_ignore_error02(self):
        self.run_exe_test('test_ignore_error02')

    def test_ignore_error03(self):
        self.run_exe_test('test_ignore_error03')

    def test_ignore_error04(self):
        self.run_exe_test('test_ignore_error04')

    def test_ignore_error05(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<c', '<v']}

        self.run_exe_test('test_ignore_error05')

    def test_ignore_error06(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']

        self.run_exe_test('test_ignore_error06')
