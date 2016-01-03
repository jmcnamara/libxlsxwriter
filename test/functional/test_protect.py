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

    def test_protect01(self):
        self.run_exe_test('test_protect01')

    def test_protect02(self):
        self.run_exe_test('test_protect02')

    def test_protect03(self):
        self.run_exe_test('test_protect03')
