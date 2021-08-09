# libxlsxwriter: Reporting Bugs

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

    printf("Libxlsxwriter version = %s\n", lxw_version());

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
