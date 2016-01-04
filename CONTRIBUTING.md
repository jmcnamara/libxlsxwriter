# libxlsxwriter: Reporting Bugs and submitting Pull Requests


## Reporting Bugs

Here are some tips on reporting bugs in `libxlsxwriter`.

### Upgrade to the latest version of the library

Upgrade to the latest version of the library since the bug you are reporting
may already be fixed.

Check the [Changes][changes] section of the documentation to see what has
changed in the latest versions.

[changes]: http://libxlsxwriter.github.io/changes.html

You can check which version of `libxlsxwriter` that you are using by checking
the `xlsxwriter.h` header file or by adding the following to your program:

```C
#include <stdio.h>
#include "xlsxwriter.h"

int main() {

    printf("Libxlsxwriter version = %s\n", LXW_VERSION);

    return 0;
}
```


### Read the documentation

Read or search the `libxlsxwriter` [documentation][docs] to see if the issue
you are encountering is already explained.

[docs]: http://libxlsxwriter.github.io/index.html

### Look at the example programs

There are many [examples programs][examples] in the distribution. Try to
identify an example program that corresponds to your query and adapt it to use
as a bug report.

[examples]: http://libxlsxwriter.github.io/examples.html


### Use the xlsxwriter Issue Tracker

The [libxlsxwriter issue tracker][issues] is on GitHub.

[issues]: https://github.com/jmcnamara/libxlsxwriter/issues


### Tips for submitting a bug report

1.  Describe the problem as clearly and as concisely as possible.
2.  Include a sample program. This is probably the most important step.
    It is generally easier to describe a problem in code than in written
    prose.
3.  The sample program should be as small as possible to demonstrate the
    problem. Don't copy and paste large non-relevant sections of your
    program.

A sample bug report is shown below. This format helps analyze and respond to
the bug report more quickly.


> Subject: Issue with SOMETHING
>
> Greetings,
>
> I am using libxlsxwriter to do SOMETHING but it appears to do SOMETHING ELSE.
>
> I am using CC version X.Y.Z, OS = uname and libxlsxwriter x.y.z.
>
> Here is some code that demonstrates the problem:
>
>
>```C
>#include "xlsxwriter.h"
>
>int main() {
>
>    lxw_workbook  *workbook  = workbook_new("bug_report.xlsx");
>    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
>
>    worksheet_write_string(worksheet, 0, 0, "Hello", NULL);
>    worksheet_write_number(worksheet, 1, 0, 123, NULL);
>
>    return workbook_close(workbook);
>}
>```
>


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
[Travis CI][travis].

[Travis]: https://travis-ci.org/jmcnamara/libxlsxwriter/
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
