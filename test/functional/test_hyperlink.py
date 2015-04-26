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

    def test_hyperlink01(self):
        self.run_exe_test('test_hyperlink01')

    def test_hyperlink02(self):
        self.run_exe_test('test_hyperlink02')

    def test_hyperlink03(self):
        self.run_exe_test('test_hyperlink03')

    # def test_hyperlink04(self):
    #     self.run_exe_test('test_hyperlink04')

   
