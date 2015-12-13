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

    def test_properties01(self):
        self.run_exe_test('test_properties01')

    def test_properties02(self):
        self.run_exe_test('test_properties02')
