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
    def test_image01(self):
        self.run_exe_test('test_image01')

    def test_image02(self):
        self.run_exe_test('test_image02')

    def test_image03(self):
        self.run_exe_test('test_image03')

    def test_image04(self):
        self.run_exe_test('test_image04')

    def test_image05(self):
        self.run_exe_test('test_image05')

    def test_image07(self):
        self.run_exe_test('test_image07')

    def test_image08(self):
        self.run_exe_test('test_image08')

    def test_image09(self):
        self.run_exe_test('test_image09')

    def test_image10(self):
        self.run_exe_test('test_image10')

    def test_image11(self):
        self.run_exe_test('test_image11')

    def test_image12(self):
        self.run_exe_test('test_image12')

    def test_image13(self):
        self.run_exe_test('test_image13')

    def test_image14(self):
        self.run_exe_test('test_image14')

    def test_image15(self):
        self.run_exe_test('test_image15')

    def test_image16(self):
        self.run_exe_test('test_image16')

    def test_image17(self):
        self.run_exe_test('test_image17')

    def test_image18(self):
        self.run_exe_test('test_image18')

    def test_image19(self):
        self.run_exe_test('test_image19')

    def test_image22(self):
        self.run_exe_test('test_image22')

    def test_image23(self):
        self.run_exe_test('test_image23')

    def test_image24(self):
        self.run_exe_test('test_image24')

    def test_image25(self):
        self.run_exe_test('test_image25')

    def test_image26(self):
        self.run_exe_test('test_image26')

    def test_image27(self):
        self.run_exe_test('test_image27')

    def test_image28(self):
        self.run_exe_test('test_image28')

    def test_image29(self):
        self.run_exe_test('test_image29')

    def test_image30(self):
        self.run_exe_test('test_image30')

    def test_image31(self):
        self.run_exe_test('test_image31')

    def test_image32(self):
        self.run_exe_test('test_image32')

    def test_image33(self):
        self.run_exe_test('test_image33')

    def test_image34(self):
        self.run_exe_test('test_image34')

    def test_image35(self):
        self.run_exe_test('test_image35')

    def test_image36(self):
        self.run_exe_test('test_image36')

    def test_image44(self):
        self.run_exe_test('test_image44')

    def test_image45(self):
        self.run_exe_test('test_image45')

    def test_image46(self):
        self.run_exe_test('test_image46')

    def test_image47(self):
        self.run_exe_test('test_image47')

    # Some of the following tests require MD5 hash support to remove duplicate images.
    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_image48(self):
        self.run_exe_test('test_image48')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_image49(self):
        self.run_exe_test('test_image49')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_image50(self):
        self.run_exe_test('test_image50')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_image51(self):
        self.run_exe_test('test_image51')

    def test_image52(self):
        self.run_exe_test('test_image52')

    def test_image53(self):
        self.run_exe_test('test_image53')

    def test_image54(self):
        self.run_exe_test('test_image54')

    def test_image55(self):
        self.run_exe_test('test_image55')

    def test_image56(self):
        self.run_exe_test('test_image56')

    def test_image57(self):
        self.run_exe_test('test_image57')

    def test_image58(self):
        self.run_exe_test('test_image58')

    # Test in-memory image handling.
    def test_image81(self):
        self.run_exe_test('test_image81', 'image01.xlsx')

    def test_image82(self):
        self.run_exe_test('test_image82')

    def test_image83(self):
        self.run_exe_test('test_image83', 'image02.xlsx')

    def test_image89(self):
        self.run_exe_test('test_image89', 'image03.xlsx')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_image86(self):
        self.run_exe_test('test_image86', 'image48.xlsx')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_image87(self):
        self.run_exe_test('test_image87', 'image50.xlsx')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_image88(self):
        self.run_exe_test('test_image88', 'image54.xlsx')

    # Test image description fields.
    def test_image84(self):
        self.run_exe_test('test_image84')

    def test_image85(self):
        self.run_exe_test('test_image85')
