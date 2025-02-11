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

    def test_header_image01(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image01')

    def test_header_image02(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image02')

    def test_header_image03(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image03')

    def test_header_image04(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image04')

    def test_header_image05(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image05')

    def test_header_image06(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup'],
                                'xl/worksheets/sheet2.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image06')

    def test_header_image07(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image07')

    def test_header_image08(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image08')

    def test_header_image09(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup'],
                                'xl/worksheets/sheet2.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image09')

    def test_header_image10(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup'],
                                'xl/worksheets/sheet2.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image10')

    def test_header_image11(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image11')

    def test_header_image12(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image12')

    def test_header_image13(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image13')

    def test_header_image14(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image14')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_header_image15(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup'],
                                'xl/worksheets/sheet2.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image15')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_header_image16(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup'],
                                'xl/worksheets/sheet2.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image16')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_header_image17(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image17')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_header_image18(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image18')

    def test_header_image19(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image19')

    def test_header_image20(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image20')

    # Test format strings with &[Picture] instead of &G.
    def test_header_image51(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image51', 'header_image01.xlsx')

    def test_header_image52(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image52', 'header_image02.xlsx')

    def test_header_image53(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins', '<pageSetup']}
        self.run_exe_test('test_header_image53', 'header_image03.xlsx')

    # Manually modified version of test_header_image05 so margins can be compared.
    def test_header_image55(self):
        self.run_exe_test('test_header_image55')
