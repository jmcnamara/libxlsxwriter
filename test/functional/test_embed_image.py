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

    def test_embed_image01(self):
        self.run_exe_test('test_embed_image01')

    # Some of the following tests require MD5 hash support to remove duplicate images.
    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_embed_image02(self):
        self.run_exe_test('test_embed_image02')

    def test_embed_image03(self):
        self.run_exe_test('test_embed_image03')

    def test_embed_image04(self):
        self.run_exe_test('test_embed_image04')

    def test_embed_image05(self):
        self.run_exe_test('test_embed_image05')

    def test_embed_image06(self):
        self.run_exe_test('test_embed_image06')

    def test_embed_image07(self):
        self.run_exe_test('test_embed_image07')

    def test_embed_image08(self):
        self.run_exe_test('test_embed_image08')

    def test_embed_image09(self):
        self.run_exe_test('test_embed_image09')

    def test_embed_image10(self):
        # Ignore the missing "display" parameter.
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<hyperlink']}
        self.run_exe_test('test_embed_image10')

    def test_embed_image13(self):
        self.run_exe_test('test_embed_image11')
    def test_embed_image12(self):
        self.run_exe_test('test_embed_image12')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_embed_image13(self):
        self.run_exe_test('test_embed_image13')

    # Test in-memory image handling.
    def test_embed_image51(self):
        self.run_exe_test('test_embed_image51', 'embed_image01.xlsx')

    def test_embed_image52(self):
        self.run_exe_test('test_embed_image52', 'embed_image08.xlsx')

