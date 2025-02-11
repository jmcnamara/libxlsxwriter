/*
 * An example of using the libxlsxwriter library to write some "rich strings",
 * i.e., strings with multiple formats.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("rich_strings.xlsx");
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    /* Set up some formats to use. */
    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    lxw_format *italic = workbook_add_format(workbook);
    format_set_italic(italic);

    lxw_format *red = workbook_add_format(workbook);
    format_set_font_color(red, LXW_COLOR_RED);

    lxw_format *blue = workbook_add_format(workbook);
    format_set_font_color(blue, LXW_COLOR_BLUE);

    lxw_format *center = workbook_add_format(workbook);
    format_set_align(center, LXW_ALIGN_CENTER);

    lxw_format *superscript = workbook_add_format(workbook);
    format_set_font_script(superscript, LXW_FONT_SUPERSCRIPT);

    /* Make the first column wider for clarity. */
    worksheet_set_column(worksheet, 0, 0, 30, NULL);


    /*
     * Create and write some rich strings with multiple formats.
     */

    /* Example 1. Some bold and italic in the same string. */
    lxw_rich_string_tuple fragment11 = {.format = NULL,   .string = "This is "     };
    lxw_rich_string_tuple fragment12 = {.format = bold,   .string = "bold"         };
    lxw_rich_string_tuple fragment13 = {.format = NULL,   .string = " and this is "};
    lxw_rich_string_tuple fragment14 = {.format = italic, .string = "italic"       };

    lxw_rich_string_tuple *rich_string1[] = {&fragment11, &fragment12,
                                             &fragment13, &fragment14, NULL};

    worksheet_write_rich_string(worksheet, CELL("A1"), rich_string1, NULL);


    /* Example 2. Some red and blue coloring in the same string. */
    lxw_rich_string_tuple fragment21 = {.format = NULL, .string = "This is "     };
    lxw_rich_string_tuple fragment22 = {.format = red,  .string = "red"          };
    lxw_rich_string_tuple fragment23 = {.format = NULL, .string = " and this is "};
    lxw_rich_string_tuple fragment24 = {.format = blue, .string = "blue"         };

    lxw_rich_string_tuple *rich_string2[] = {&fragment21, &fragment22,
                                             &fragment23, &fragment24, NULL};

    worksheet_write_rich_string(worksheet, CELL("A3"), rich_string2, NULL);


    /* Example 3. A rich string plus cell formatting. */
    lxw_rich_string_tuple fragment31 = {.format = NULL, .string = "Some "    };
    lxw_rich_string_tuple fragment32 = {.format = bold, .string = "bold text"};
    lxw_rich_string_tuple fragment33 = {.format = NULL, .string = " centered"};

    lxw_rich_string_tuple *rich_string3[] = {&fragment31, &fragment32,
                                             &fragment33, NULL};

    /* Note that this example also has a "center" cell format. */
    worksheet_write_rich_string(worksheet, CELL("A5"), rich_string3, center);


    /* Example 4. A math example with a superscript. */
    lxw_rich_string_tuple fragment41 = {.format = italic,      .string = "j =k" };
    lxw_rich_string_tuple fragment42 = {.format = superscript, .string = "(n-1)"};

    lxw_rich_string_tuple *rich_string4[] = {&fragment41, &fragment42, NULL};

    worksheet_write_rich_string(worksheet, CELL("A7"), rich_string4, center);


    workbook_close(workbook);

    return 0;
}
