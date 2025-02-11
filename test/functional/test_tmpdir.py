###############################################################################
#
# Tests for libxlsxwriter.
#
# SPDX-License-Identifier: BSD-2-Clause
# Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
#

import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_tmpdir01(self):
        self.run_exe_test('test_tmpdir01', 'simple01.xlsx')

    def test_tmpdir02(self):
        self.run_exe_test('test_tmpdir02', 'optimize01.xlsx')
