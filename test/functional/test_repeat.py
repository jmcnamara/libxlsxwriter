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

    def test_repeat01(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins']}
        self.run_exe_test('test_repeat01')

    def test_repeat02(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins']}
        self.run_exe_test('test_repeat02')

    def test_repeat03(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins']}
        self.run_exe_test('test_repeat03')

    def test_repeat04(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins']}
        self.run_exe_test('test_repeat04')

    def test_repeat05(self):
        self.ignore_files = ['docProps/app.xml']
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins'],
                                'xl/worksheets/sheet3.xml': ['<pageMargins']}
        self.run_exe_test('test_repeat05')

    def test_repeat06(self):
        self.ignore_elements = {'xl/worksheets/sheet1.xml': ['<pageMargins']}
        self.run_exe_test('test_repeat06')
