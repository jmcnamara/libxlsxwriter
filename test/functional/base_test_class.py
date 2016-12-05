###############################################################################
#
# Base test class for libxlsxwriter functional tests.
#
# Copyright 2014-2016, John McNamara, jmcnamara@cpan.org
#

import pytest
import unittest
import os, sys
from helper_functions import _compare_xlsx_files

class XLSXBaseTest(unittest.TestCase):
    """
    Test file created with libxlsxwriter against a file created by Excel.

    """

    def setUp(self):
        self.maxDiff = None

        self.no_system_error = 0
        self.got_filename = ''
        self.exp_filename = ''
        self.ignore_files = []
        self.ignore_elements = {}

    def run_exe_test(self, exe_name, exp_filename=None):
        """Run C exe and compare output xlsx file with the Excel file."""

        data_dir = pytest.config.getoption('data_dir')
        exec_dir = pytest.config.getoption('exec_dir')

        # Run the C executable to generate the "got" xlsx file.
        got = os.system("cd %s; ./%s" % (exec_dir, exe_name))
        self.assertEqual(got, self.no_system_error)

        # Create the path/file names for the xlsx files to compare.
        got_filename = exe_name.replace('test_', '') + '.xlsx'

        if not exp_filename:
            exp_filename = got_filename

        self.got_filename = os.path.join(exec_dir, 'test_' + got_filename)
        self.exp_filename = os.path.join(data_dir, exp_filename)

        # Do the comparison between the files.
        got, exp = _compare_xlsx_files(self.got_filename,
                                       self.exp_filename,
                                       self.ignore_files,
                                       self.ignore_elements)

        self.assertEqual(got, exp)

    def tearDown(self):
        # Cleanup.
        if os.path.exists(self.got_filename):
            os.remove(self.got_filename)

        self.ignore_files = []
        self.ignore_elements = {}
