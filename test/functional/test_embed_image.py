###############################################################################
#
# Tests for libxlsxwriter.
#
# SPDX-License-Identifier: BSD-2-Clause
# Copyright 2014-2024, John McNamara, jmcnamara@cpan.org.
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

    def test_embed_image11(self):
        self.run_exe_test('test_embed_image11')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_embed_image13(self):
        self.run_exe_test('test_embed_image13')
