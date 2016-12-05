import pytest

def pytest_addoption(parser):
    parser.addoption("--data_dir", action="store",
            default='test/functional/xlsx_files/',
            help="the path where baseline images are found")

    parser.addoption("--exec_dir", action="store",
            default='test/functional/src/',
            help="the path where test executables are found")

