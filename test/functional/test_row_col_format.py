###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright (c), 2014, John McNamara, jmcnamara@cpan.org
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_row_colformat01(self):
        self.run_exe_test('test_row_col_format01')

    def test_row_colformat02(self):
        self.run_exe_test('test_row_col_format02')

    # def test_row_colformat03(self):
    #     self.run_exe_test('test_row_col_format03')

