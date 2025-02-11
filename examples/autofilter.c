/*
 * Example of adding an autofilter to a worksheet in Excel using
 * libxlsxwriter.
 *
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org
 *
 */

#include "xlsxwriter.h"


void write_worksheet_header(lxw_worksheet *worksheet, lxw_format *header);


int main() {

    lxw_workbook  *workbook   = workbook_new("autofilter.xlsx");
    lxw_worksheet *worksheet1 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet2 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet3 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet4 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet5 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet6 = workbook_add_worksheet(workbook, NULL);
    lxw_worksheet *worksheet7 = workbook_add_worksheet(workbook, NULL);

    struct row {
        char region[16];
        char item[16];
        int  volume;
        char month[16];
    };

    struct row data[] = {
        {"East",  "Apple",   9000, "July"      },
        {"East",  "Apple",   5000, "July"      },
        {"South", "Orange",  9000, "September" },
        {"North", "Apple",   2000, "November"  },
        {"West",  "Apple",   9000, "November"  },
        {"South", "Pear",    7000, "October"   },
        {"North", "Pear",    9000, "August"    },
        {"West",  "Orange",  1000, "December"  },
        {"West",  "Grape",   1000, "November"  },
        {"South", "Pear",   10000, "April"     },
        {"West",  "Grape",   6000, "January"   },
        {"South", "Orange",  3000, "May"       },
        {"North", "Apple",   3000, "December"  },
        {"South", "Apple",   7000, "February"  },
        {"West",  "Grape",   1000, "December"  },
        {"East",  "Grape",   8000, "February"  },
        {"South", "Grape",  10000, "June"      },
        {"West",  "Pear",    7000, "December"  },
        {"South", "Apple",   2000, "October"   },
        {"East",  "Grape",   7000, "December"  },
        {"North", "Grape",   6000, "April"     },
        {"East",  "Pear",    8000, "February"  },
        {"North", "Apple",   7000, "August"    },
        {"North", "Orange",  7000, "July"      },
        {"North", "Apple",   6000, "June"      },
        {"South", "Grape",   8000, "September" },
        {"West",  "Apple",   3000, "October"   },
        {"South", "Orange", 10000, "November"  },
        {"West",  "Grape",   4000, "July"      },
        {"North", "Orange",  5000, "August"    },
        {"East",  "Orange",  1000, "November"  },
        {"East",  "Orange",  4000, "October"   },
        {"North", "Grape",   5000, "August"    },
        {"East",  "Apple",   1000, "December"  },
        {"South", "Apple",   10000, "March"    },
        {"East",  "Grape",   7000, "October"   },
        {"West",  "Grape",   1000, "September" },
        {"East",  "Grape",  10000, "October"   },
        {"South", "Orange",  8000, "March"     },
        {"North", "Apple",   4000, "July"      },
        {"South", "Orange",  5000, "July"      },
        {"West",  "Apple",   4000, "June"      },
        {"East",  "Apple",   5000, "April"     },
        {"North", "Pear",    3000, "August"    },
        {"East",  "Grape",   9000, "November"  },
        {"North", "Orange",  8000, "October"   },
        {"East",  "Apple",  10000, "June"      },
        {"South", "Pear",    1000, "December"  },
        {"North", "Grape",   10000, "July"     },
        {"East",  "Grape",   6000, "February"  }
    };

    uint16_t i;
    lxw_row_col_options hidden = {.hidden = LXW_TRUE};

    lxw_format *header = workbook_add_format(workbook);
    format_set_bold(header);



    /*
     * Example 1. Autofilter without conditions.
     */

    /* Set up the worksheet data. */
    write_worksheet_header(worksheet1, header);

    /* Write the row data. */
    for (i = 0; i < sizeof(data)/sizeof(struct row); i++) {
        worksheet_write_string(worksheet1, i + 1, 0, data[i].region, NULL);
        worksheet_write_string(worksheet1, i + 1, 1, data[i].item,   NULL);
        worksheet_write_number(worksheet1, i + 1, 2, data[i].volume, NULL);
        worksheet_write_string(worksheet1, i + 1, 3, data[i].month,  NULL);
    }


    /* Add the autofilter. */
    worksheet_autofilter(worksheet1, 0, 0, 50, 3);


    /*
     * Example 2. Autofilter with a filter condition in the first column.
     */

    /* Set up the worksheet data. */
    write_worksheet_header(worksheet2, header);

    /* Write the row data. */
    for (i = 0; i < sizeof(data)/sizeof(struct row); i++) {
        worksheet_write_string(worksheet2, i + 1, 0, data[i].region, NULL);
        worksheet_write_string(worksheet2, i + 1, 1, data[i].item,   NULL);
        worksheet_write_number(worksheet2, i + 1, 2, data[i].volume, NULL);
        worksheet_write_string(worksheet2, i + 1, 3, data[i].month,  NULL);

        /* It isn't sufficient to just apply the filter condition below. We
         * must also hide the rows that don't match the criteria since Excel
         * doesn't do that automatically. */
        if (strcmp(data[i].region, "East") == 0) {
            /* Row matches the filter, no further action required. */
        }
        else {
            /* Hide rows that don't match the filter. */
            worksheet_set_row_opt(worksheet2, i + 1, LXW_DEF_ROW_HEIGHT, NULL, &hidden);
        }

        /* Note, the if() statement above is written to match the logic of the
         * criteria in worksheet_filter_column() below. However you could get
         * the same results with the following simpler, but reversed, code:
         *
         *     if (strcmp(data[i].region, "East") != 0) {
         *         worksheet_set_row_opt(worksheet2, i + 1, LXW_DEF_ROW_HEIGHT, NULL, &hidden);
         *     }
         *
         * The same applies to the Examples 3-6 as well.
         */
    }


    /* Add the autofilter. */
    worksheet_autofilter(worksheet2, 0, 0, 50, 3);

    /* Add the filter criteria. */
    lxw_filter_rule filter_rule2 = {.criteria     = LXW_FILTER_CRITERIA_EQUAL_TO,
                                    .value_string = "East"};

    worksheet_filter_column(worksheet2, 0, &filter_rule2);


    /*
     * Example 3. Autofilter with a dual filter condition in one of the columns.
     */

    /* Set up the worksheet data. */
    write_worksheet_header(worksheet3, header);

    /* Write the row data. */
    for (i = 0; i < sizeof(data)/sizeof(struct row); i++) {
        worksheet_write_string(worksheet3, i + 1, 0, data[i].region, NULL);
        worksheet_write_string(worksheet3, i + 1, 1, data[i].item,   NULL);
        worksheet_write_number(worksheet3, i + 1, 2, data[i].volume, NULL);
        worksheet_write_string(worksheet3, i + 1, 3, data[i].month,  NULL);

        if (strcmp(data[i].region, "East") == 0 || strcmp(data[i].region, "South") == 0) {
            /* Row matches the filter, no further action required. */
        }
        else {
            /* We need to hide rows that don't match the filter. */
            worksheet_set_row_opt(worksheet3, i + 1, LXW_DEF_ROW_HEIGHT, NULL, &hidden);
        }
    }

    /* Add the autofilter. */
    worksheet_autofilter(worksheet3, 0, 0, 50, 3);

    /* Add the filter criteria. */
    lxw_filter_rule filter_rule3a = {.criteria     = LXW_FILTER_CRITERIA_EQUAL_TO,
                                     .value_string = "East"};

    lxw_filter_rule filter_rule3b = {.criteria     = LXW_FILTER_CRITERIA_EQUAL_TO,
                                     .value_string = "South"};

    worksheet_filter_column2(worksheet3, 0, &filter_rule3a, &filter_rule3b, LXW_FILTER_OR);



    /*
     * Example 4. Autofilter with filter conditions in two columns.
     */

    /* Set up the worksheet data. */
    write_worksheet_header(worksheet4, header);

    /* Write the row data. */
    for (i = 0; i < sizeof(data)/sizeof(struct row); i++) {
        worksheet_write_string(worksheet4, i + 1, 0, data[i].region, NULL);
        worksheet_write_string(worksheet4, i + 1, 1, data[i].item,   NULL);
        worksheet_write_number(worksheet4, i + 1, 2, data[i].volume, NULL);
        worksheet_write_string(worksheet4, i + 1, 3, data[i].month,  NULL);

        if (strcmp(data[i].region, "East") == 0 &&
            data[i].volume > 3000 && data[i].volume < 8000)
        {
            /* Row matches the filter, no further action required. */
        }
        else {
            /* We need to hide rows that don't match the filter. */
            worksheet_set_row_opt(worksheet4, i + 1, LXW_DEF_ROW_HEIGHT, NULL, &hidden);
        }
    }

    /* Add the autofilter. */
    worksheet_autofilter(worksheet4, 0, 0, 50, 3);

    /* Add the filter criteria. */
    lxw_filter_rule filter_rule4a = {.criteria     = LXW_FILTER_CRITERIA_EQUAL_TO,
                                     .value_string = "East"};

    lxw_filter_rule filter_rule4b = {.criteria     = LXW_FILTER_CRITERIA_GREATER_THAN,
                                     .value        = 3000};

    lxw_filter_rule filter_rule4c = {.criteria     = LXW_FILTER_CRITERIA_LESS_THAN,
                                     .value        = 8000};

    worksheet_filter_column(worksheet4,  0, &filter_rule4a);
    worksheet_filter_column2(worksheet4, 2, &filter_rule4b, &filter_rule4c, LXW_FILTER_AND);


    /*
     * Example 5. Autofilter with a dual filter condition in one of the columns.
     */

    /* Set up the worksheet data. */
    write_worksheet_header(worksheet5, header);

    /* Write the row data. */
    for (i = 0; i < sizeof(data)/sizeof(struct row); i++) {
        worksheet_write_string(worksheet5, i + 1, 0, data[i].region, NULL);
        worksheet_write_string(worksheet5, i + 1, 1, data[i].item,   NULL);
        worksheet_write_number(worksheet5, i + 1, 2, data[i].volume, NULL);
        worksheet_write_string(worksheet5, i + 1, 3, data[i].month,  NULL);

        if (strcmp(data[i].region, "East")  == 0 ||
            strcmp(data[i].region, "North") == 0 ||
            strcmp(data[i].region, "South") == 0)
        {
            /* Row matches the filter, no further action required. */
        }
        else {
            /* We need to hide rows that don't match the filter. */
            worksheet_set_row_opt(worksheet5, i + 1, LXW_DEF_ROW_HEIGHT, NULL, &hidden);
        }
    }

    /* Add the autofilter. */
    worksheet_autofilter(worksheet5, 0, 0, 50, 3);

    /* Add the filter criteria. */
    const char* list[] = {"East", "North", "South", NULL};

    worksheet_filter_list(worksheet5, 0, list);


    /*
     * Example 6. Autofilter with filter for blanks.
     */

    /* Set up the worksheet data. */
    write_worksheet_header(worksheet6, header);

    /* Simulate one blank cell in the data, to test the filter. */
    data[5].region[0] = '\0';


    /* Write the row data. */
    for (i = 0; i < sizeof(data)/sizeof(struct row); i++) {
        worksheet_write_string(worksheet6, i + 1, 0, data[i].region, NULL);
        worksheet_write_string(worksheet6, i + 1, 1, data[i].item,   NULL);
        worksheet_write_number(worksheet6, i + 1, 2, data[i].volume, NULL);
        worksheet_write_string(worksheet6, i + 1, 3, data[i].month,  NULL);

        if (strcmp(data[i].region, "") == 0) {
            /* Row matches the filter, no further action required. */
        }
        else {
            /* We need to hide rows that don't match the filter. */
            worksheet_set_row_opt(worksheet6, i + 1, LXW_DEF_ROW_HEIGHT, NULL, &hidden);
        }
    }

    /* Add the autofilter. */
    worksheet_autofilter(worksheet6, 0, 0, 50, 3);

    /* Add the filter criteria. */
    lxw_filter_rule filter_rule6 = {.criteria  = LXW_FILTER_CRITERIA_BLANKS};

    worksheet_filter_column(worksheet6, 0, &filter_rule6);


    /*
     * Example 7. Autofilter with filter for non-blanks.
     */

    /* Set up the worksheet data. */
    write_worksheet_header(worksheet7, header);

    /* Write the row data. */
    for (i = 0; i < sizeof(data)/sizeof(struct row); i++) {
        worksheet_write_string(worksheet7, i + 1, 0, data[i].region, NULL);
        worksheet_write_string(worksheet7, i + 1, 1, data[i].item,   NULL);
        worksheet_write_number(worksheet7, i + 1, 2, data[i].volume, NULL);
        worksheet_write_string(worksheet7, i + 1, 3, data[i].month,  NULL);

        if (strcmp(data[i].region, "") != 0) {
            /* Row matches the filter, no further action required. */
        }
        else {
            /* We need to hide rows that don't match the filter. */
            worksheet_set_row_opt(worksheet7, i + 1, LXW_DEF_ROW_HEIGHT, NULL, &hidden);
        }
    }

    /* Add the autofilter. */
    worksheet_autofilter(worksheet7, 0, 0, 50, 3);

    /* Add the filter criteria. */
    lxw_filter_rule filter_rule7 = {.criteria  = LXW_FILTER_CRITERIA_NON_BLANKS};

    worksheet_filter_column(worksheet7, 0, &filter_rule7);



    return workbook_close(workbook);
}


void write_worksheet_header(lxw_worksheet *worksheet, lxw_format *header) {

    /* Make the columns wider for clarity. */
    worksheet_set_column(worksheet, 0, 3, 12, NULL);


    /* Write the column headers. */
    worksheet_set_row(worksheet, 0, 20, header);
    worksheet_write_string(worksheet, 0, 0, "Region", NULL);
    worksheet_write_string(worksheet, 0, 1, "Item",   NULL);
    worksheet_write_string(worksheet, 0, 2, "Volume", NULL);
    worksheet_write_string(worksheet, 0, 3, "Month",  NULL);

}
