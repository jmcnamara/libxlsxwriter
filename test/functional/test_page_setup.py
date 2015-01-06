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

    def test_page_view01(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageSetup']}
        self.run_exe_test('test_page_view01')

