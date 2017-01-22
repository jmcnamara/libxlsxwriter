###############################################################################
#
# Tests for libxlsxwriter.
#
# Copyright 2014-2017, John McNamara, jmcnamara@cpan.org
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_chart_up_down_bars01(self):
        self.run_exe_test('test_chart_up_down_bars01')

    def test_chart_up_down_bars02(self):
        self.run_exe_test('test_chart_up_down_bars02')
