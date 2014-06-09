
/* Compare expected results with the XML data written to the output
 * test file.
 */
#define RUN_XLSX_STREQ(exp, got)                                    \
    fflush(testfile);                                               \
    int file_size = ftell(testfile);                                \
                                                                    \
    got = (char*)calloc(file_size + 1, 1);                          \
                                                                    \
    rewind(testfile);                                               \
    fread(got, file_size, 1, testfile);                             \
                                                                    \
    EXPECT_STREQ((exp), (got));                                     \
                                                                    \
    if (got)                                                        \
        free(got);                                                  \
                                                                    \
    fclose(testfile)

/* Compare expected results with the XML data written to the output
 * test file. Same as the previous macro but only shows the difference
 * from where it starts. Suitable for long strings of XML data.
 */
#define RUN_XLSX_STREQ_SHORT(exp, got)                              \
    fflush(testfile);                                               \
    int file_size = ftell(testfile);                                \
                                                                    \
    got = (char*)calloc(file_size + 1, 1);                          \
                                                                    \
    rewind(testfile);                                               \
    fread(got, file_size, 1, testfile);                             \
                                                                    \
    /* Start comparison from first difference. */                   \
    char *got_short = got;                                          \
    char *exp_short = exp;                                          \
    while (*exp_short && *exp_short == *got_short) {                \
        exp_short++;                                                \
        got_short++;                                                \
    }                                                               \
                                                                    \
    EXPECT_STREQ(exp_short, got_short);                             \
                                                                    \
    if (got)                                                        \
        free(got);                                                  \
                                                                    \
    fclose(testfile)


#define TEST_COL_TO_NAME(num, abs, exp)                             \
    _xl_col_to_name(got, num, abs);                                 \
    EXPECT_STREQ(exp, got);


#define TEST_ROWCOL_TO_CELL(row, col, exp)                          \
    xl_rowcol_to_cell(got, row, col);                               \
    EXPECT_STREQ(exp, got);


#define TEST_ROWCOL_TO_CELL_ABS(row, col, row_abs, col_abs, exp)    \
    xl_rowcol_to_cell_abs(got, row, col, row_abs, col_abs);         \
    EXPECT_STREQ(exp, got);


#define TEST_XL_RANGE(row1, col1, row2, col2, exp)                  \
    xl_range(got, row1, col1, row2, col2);                          \
    EXPECT_STREQ(exp, got);


#define TEST_XL_RANGE_ABS(row1, col1, row2, col2, exp)              \
    xl_range_abs(got, row1, col1, row2, col2);                      \
    EXPECT_STREQ(exp, got);


#define TEST_DATETIME_TIME(_hour, _min, _sec, exp)                  \
    datetime = (lxw_datetime*)calloc(1, sizeof(lxw_datetime));      \
    datetime->hour  = _hour;                                        \
    datetime->min   = _min;                                         \
    datetime->sec   = _sec;                                         \
                                                                    \
    got = _datetime_to_excel_date(datetime, 0);                     \
                                                                    \
    ASSERT_DOUBLE_EQ(exp, got);                                     \
    free(datetime);

#define TEST_DATETIME_DATE(_year, _month, _day, exp)                \
    datetime = (lxw_datetime*)calloc(1, sizeof(lxw_datetime));      \
    datetime->year  = _year;                                        \
    datetime->month = _month;                                       \
    datetime->day   = _day;                                         \
                                                                    \
    got = _datetime_to_excel_date(datetime, 0);                     \
                                                                    \
    ASSERT_DOUBLE_EQ(exp, got);                                     \
    free(datetime);

#define TEST_DATETIME_DATE_1904(_year, _month, _day, exp)           \
    datetime = (lxw_datetime*)calloc(1, sizeof(lxw_datetime));      \
    datetime->year  = _year;                                        \
    datetime->month = _month;                                       \
    datetime->day   = _day;                                         \
                                                                    \
    got = _datetime_to_excel_date(datetime, 1);                     \
                                                                    \
    ASSERT_DOUBLE_EQ(exp, got);                                     \
    free(datetime);

#define TEST_DATETIME(_year, _month, _day, _hour, _min, _sec, exp)  \
    datetime = (lxw_datetime*)calloc(1, sizeof(lxw_datetime));      \
    datetime->year  = _year;                                        \
    datetime->month = _month;                                       \
    datetime->day   = _day;                                         \
    datetime->hour  = _hour;                                        \
    datetime->min   = _min;                                         \
    datetime->sec   = _sec;                                         \
                                                                    \
    got = _datetime_to_excel_date(datetime, 0);                     \
                                                                    \
    ASSERT_DOUBLE_EQ(exp, got);                                     \
    free(datetime);
