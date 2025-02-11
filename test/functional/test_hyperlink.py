###############################################################################
#
# Tests for libxlsxwriter.
#
# SPDX-License-Identifier: BSD-2-Clause
# Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
#

import os
import pytest
import base_test_class

class TestCompareXLSXFiles(base_test_class.XLSXBaseTest):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def test_hyperlink01(self):
        self.run_exe_test('test_hyperlink01')

    def test_hyperlink02(self):
        self.run_exe_test('test_hyperlink02')

    def test_hyperlink03(self):
        self.run_exe_test('test_hyperlink03')

    def test_hyperlink04(self):
        self.run_exe_test('test_hyperlink04')

    def test_hyperlink05(self):
        self.run_exe_test('test_hyperlink05')

    def test_hyperlink06(self):
        self.run_exe_test('test_hyperlink06')

    def test_hyperlink07(self):
        self.run_exe_test('test_hyperlink07')

    def test_hyperlink08(self):
        self.run_exe_test('test_hyperlink08')

    def test_hyperlink09(self):
        self.run_exe_test('test_hyperlink09')

    def test_hyperlink10(self):
        self.run_exe_test('test_hyperlink10')

    def test_hyperlink11(self):
        self.run_exe_test('test_hyperlink11')

    def test_hyperlink12(self):
        self.run_exe_test('test_hyperlink12')

    def test_hyperlink13(self):
        # Ignore the count of duplicate strings.
        self.ignore_elements = {'xl/sharedStrings.xml': ['<sst']}
        self.run_exe_test('test_hyperlink13')

    def test_hyperlink14(self):
        # Ignore unused/overwritten strings.
        self.ignore_files = ['xl/sharedStrings.xml']
        self.run_exe_test('test_hyperlink14')

    def test_hyperlink15(self):
        self.run_exe_test('test_hyperlink15')

    def test_hyperlink16(self):
        self.run_exe_test('test_hyperlink16')

    def test_hyperlink17(self):
        self.run_exe_test('test_hyperlink17')

    def test_hyperlink18(self):
        self.run_exe_test('test_hyperlink18')

    # This test requires that we check if the cell that the hyperlink refers
    # to is a string. If it isn't we have to add a display attribute. However,
    # looking up the cell is currently too expensive.
    #
    # def test_hyperlink19(self):
    #     self.ignore_files = ['xl/calcChain.xml',
    #                          '[Content_Types].xml',
    #                          'xl/_rels/workbook.xml.rels']
    #     self.run_exe_test('test_hyperlink19')

    def test_hyperlink20(self):
        # Ignore custom colours.
        self.ignore_files = ['xl/styles.xml']
        self.run_exe_test('test_hyperlink20')

    def test_hyperlink21(self):
        self.run_exe_test('test_hyperlink21')

    def test_hyperlink22(self):
        self.run_exe_test('test_hyperlink22')

    def test_hyperlink23(self):
        self.run_exe_test('test_hyperlink23')

    def test_hyperlink24(self):
        self.run_exe_test('test_hyperlink24')

    def test_hyperlink25(self):
        self.run_exe_test('test_hyperlink25')

    def test_hyperlink26(self):
        self.run_exe_test('test_hyperlink26')

    def test_hyperlink27(self):
        self.run_exe_test('test_hyperlink27')

    def test_hyperlink28(self):
        self.run_exe_test('test_hyperlink28')

    def test_hyperlink29(self):
        self.run_exe_test('test_hyperlink29')

    def test_hyperlink30(self):
        # Ignore custom colors in styles.xml file.
        self.ignore_elements = {'xl/styles.xml': ['<color',
                                                  '<mruColors>',
                                                  '</mruColors>',
                                                  '</colors>']}
        self.run_exe_test('test_hyperlink30')

    def test_hyperlink31(self):
        self.run_exe_test('test_hyperlink31')

    # Copy of hyperlink28 with explicit formatting.
    def test_hyperlink78(self):
        self.run_exe_test('test_hyperlink78', 'hyperlink28.xlsx')

    def test_hyperlink32(self):
        self.run_exe_test('test_hyperlink32')

    def test_hyperlink33(self):
        self.run_exe_test('test_hyperlink33')

    def test_hyperlink34(self):
        self.run_exe_test('test_hyperlink34')

    def test_hyperlink35(self):
        self.run_exe_test('test_hyperlink35')

    def test_hyperlink36(self):
        self.run_exe_test('test_hyperlink36')

    # Skip 37 since it contains a textbox.

    def test_hyperlink38(self):
        self.run_exe_test('test_hyperlink38')

    def test_hyperlink39(self):
        self.run_exe_test('test_hyperlink39')

    def test_hyperlink40(self):
        self.run_exe_test('test_hyperlink40')

    def test_hyperlink41(self):
        self.run_exe_test('test_hyperlink41')

    def test_hyperlink42(self):
        self.run_exe_test('test_hyperlink42')

    def test_hyperlink43(self):
        self.run_exe_test('test_hyperlink43')

    def test_hyperlink44(self):
        self.run_exe_test('test_hyperlink44')

    def test_hyperlink45(self):
        self.run_exe_test('test_hyperlink45')

    def test_hyperlink46(self):
        self.run_exe_test('test_hyperlink46')

    def test_hyperlink47(self):
        self.run_exe_test('test_hyperlink47')

    @pytest.mark.skipif(os.environ.get('USE_NO_MD5'), reason="compiled without MD5 support")
    def test_hyperlink48(self):
        self.run_exe_test('test_hyperlink48')

    def test_hyperlink49(self):
        self.run_exe_test('test_hyperlink49')

    def test_hyperlink50(self):
        self.run_exe_test('test_hyperlink50')

    def test_hyperlink51(self):
        self.run_exe_test('test_hyperlink51')

    # Test in-memory image handling.
    def test_hyperlink81(self):
        self.run_exe_test('test_hyperlink81', 'hyperlink49.xlsx')

    def test_hyperlink81(self):
        self.run_exe_test('test_hyperlink82', 'hyperlink33.xlsx')
