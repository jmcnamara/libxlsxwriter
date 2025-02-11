###############################################################################
#
# Tests for libxlsxwriter.
#
# SPDX-License-Identifier: BSD-2-Clause
# Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
#
import os
import pytest
import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_background01(self):
        self.run_exe_test('test_background01')

    def test_background02(self):
        self.run_exe_test('test_background02')

    def test_background03(self):
        self.run_exe_test('test_background03')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_background04(self):
        self.run_exe_test('test_background04')

    def test_background05(self):
        self.run_exe_test('test_background05')

    def test_background06(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageSetup']}
        self.run_exe_test('test_background06')

    def test_background07(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageSetup']}
        self.run_exe_test('test_background07')

    # Same as background02 except with an image buffer.
    def test_background52(self):
        self.run_exe_test('test_background52', 'background02.xlsx')

