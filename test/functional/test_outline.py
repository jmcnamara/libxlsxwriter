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

    def test_outline01(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_outline01')

    def test_outline02(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_outline02')

    def test_outline03(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_outline03')

    def test_outline04(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_outline04')

    def test_outline05(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_outline05')

    def test_outline06(self):
        self.ignore_files = ['xl/calcChain.xml',
                             '[Content_Types].xml',
                             'xl/_rels/workbook.xml.rels']
        self.run_exe_test('test_outline06')
