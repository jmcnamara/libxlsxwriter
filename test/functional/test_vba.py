###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_macro01(self):
        self.run_exe_test('test_macro01', 'macro01.xlsm')

    def test_macro02(self):
        self.run_exe_test('test_macro02', 'macro02.xlsm')

    def test_macro03(self):
        self.run_exe_test('test_macro03', 'macro03.xlsm')
