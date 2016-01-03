###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test writing a workbook in optimization/constant memory mode.

    """

    def test_optimize01(self):
        self.run_exe_test('test_optimize01')

    def test_optimize02(self):
        self.run_exe_test('test_optimize02')

    def test_optimize06(self):
        self.run_exe_test('test_optimize06')

    # Skip some of the XlsxWriter tests until the required functionality is ported.

    def test_optimize21(self):
        self.run_exe_test('test_optimize21')

    def test_optimize22(self):
        self.run_exe_test('test_optimize22')

    def test_optimize23(self):
        self.run_exe_test('test_optimize23')

    def test_optimize24(self):
        self.run_exe_test('test_optimize24')

    def test_optimize25(self):
        self.run_exe_test('test_optimize25')

    def test_optimize26(self):
        self.run_exe_test('test_optimize26')
