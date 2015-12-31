###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright 2014-2015, John McNamara, jmcnamara@cpan.org
#

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
