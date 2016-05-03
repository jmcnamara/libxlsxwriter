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

    def test_chart_bar01(self):
        self.run_exe_test('test_chart_bar01')

    def test_chart_bar05(self):
        self.run_exe_test('test_chart_bar05')

    def test_chart_bar51(self):
        self.run_exe_test('test_chart_bar51')


   
