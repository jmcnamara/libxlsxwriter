# libxlsxwriter: Submitting Pull Requests

# Pull Requests and Contributing to Libxlsxwriter

All patches and pull requests are welcome but in general you should start with
an issue tracker to describe what you intend to do before you do it.


### Getting Started

1. Pull requests and new feature proposals must start with an [issue
   tracker][issues]. This serves as the focal point for the design discussion.
2. Describe what you plan to do. If there are API changes add some code
   example to demonstrate them.
3. Fork the repository.
4. Run all the tests to make sure the current code works on your system using
   `make test`.  See the [Running the Test Suite][tests] section of the docs
   for instructions.
5. Create a feature branch for your new feature.


[tests]: http://libxlsxwriter.github.io/running_the_tests.html

### Code Style

The code style is mainly K&R style with 4 space indents.

The author uses GNU indent (`gindent`) 2.2.10 with the following options:

```
    --braces-on-if-line
    --braces-on-struct-decl-line
    --case-indentation 4
    --continue-at-parentheses
    --declaration-comment-column 0
    --format-first-column-comments
    --honour-newlines
    --ignore-profile
    --indent-label 0
    --indent-level 4
    --no-space-after-function-call-names
    --no-tabs
    --swallow-optional-blank-lines
```

The [indent configuration file][indentpro] is available in the repo. The code
can be indented automatically if the same version of `gindent` is used with
the following make command:

```shell
    make indent
```

Note, make sure you have backed up your files or added them to the index
before running this command.

In general follow the existing style in the code.

[indentpro]: https://github.com/jmcnamara/libxlsxwriter/blob/master/.indent.pro

### Writing and Running Tests

Any significant features should be accompanied by a test. See the `test`
directory and the [Running the Test Suite][tests] section of the docs for
details of the test setup.

The tests can be run as follows:

```shell
    make test
```
Same as:

```shell
    make test_unit
    make test_functional
```

The functional tests require the Python module [pytest][pytest] as a test runner.

If you have `valgrind` installed you can use the test suite to check for memory leaks:

```shell
    make test_valgrind
```

When you push your changes they will also be tested automatically using
[GitHub Actions][actions].

[actions]: https://github.com/jmcnamara/libxlsxwriter/actions
[pytest]: http://pytest.org/


### Documentation

The `libxlsxwriter` documentation is written in Doxygen format in the header
files and in additional `.dox` files in the `docs/src` directory of the
repo. The documentation can be built as follows:

```shell
    make docs
    open docs/html/index.html
```


### Example programs

If applicable add an example program to the `examples` directory. Example
files can be built using:

```shell
   make docs
```

### Copyright and License

Copyright remains with the original author. Do not include additional
copyright claims or Licensing requirements. GitHub and the `git` repository
will record your contribution and it will be acknowledged it in the Changes
file.


### Submitting the Pull Request

If your change involves several incremental `git` commits then `rebase` or
`squash` them onto another branch so that the Pull Request is a single commit
or a small number of logical commits.

Push your changes to GitHub and submit the Pull Request with a hash link to
the to the Issue tracker that was opened above.
