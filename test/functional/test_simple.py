###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_simple01(self):
        self.run_exe_test('test_simple01')

    def test_simple02(self):
        self.run_exe_test('test_simple02')

    def test_simple03(self):
        self.run_exe_test('test_simple03')

    def test_simple04(self):
        self.run_exe_test('test_simple04')

   
