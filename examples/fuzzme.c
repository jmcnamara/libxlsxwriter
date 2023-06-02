#include "xlsxwriter.h"

struct expense {
    char         item[32];
    int          cost;
    lxw_datetime datetime;
};

struct expense expenses[] = {
    {"Rent", 1000, { .year = 2013, .month = 1, .day = 13 } },
    {"Gas",   100, { .year = 2013, .month = 1, .day = 14 } },
    {"Food",  300, { .year = 2013, .month = 1, .day = 16 } },
    {"Gym",    50, { .year = 2013, .month = 1, .day = 20 } },
};

int fuzzme(char *data) {

    /* Create a workbook and add a worksheet. */
    lxw_workbook  *workbook  = workbook_new(data);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    int row = 0;
    int col = 0;
    int i;

    /* Add a bold format to use to highlight cells. */
    lxw_format *bold = workbook_add_format(workbook);
    format_set_bold(bold);

    /* Add a number format for cells with money. */
    lxw_format *money = workbook_add_format(workbook);
    format_set_num_format(money, data);

    /* Add an Excel date format. */
    lxw_format *date_format = workbook_add_format(workbook);
    format_set_num_format(date_format, data);

    /* Adjust the column width. */
    worksheet_set_column(worksheet, 0, 0, 15, NULL);

    /* Write some data header. */
    worksheet_write_string(worksheet, row, col,     data, bold);
    worksheet_write_string(worksheet, row, col + 1, data, bold);

    /* Iterate over the data and write it out element by element. */
    for (i = 0; i < 4; i++) {
        /* Write from the first cell below the headers. */
        row = i + 1;
        worksheet_write_string  (worksheet, row, col,      expenses[i].item,     NULL);
        worksheet_write_datetime(worksheet, row, col + 1, &expenses[i].datetime, date_format);
        worksheet_write_number  (worksheet, row, col + 2,  expenses[i].cost,     money);
    }

    /* Write a total using a formula. */
    worksheet_write_string (worksheet, row + 1, col,     data,       bold);
    worksheet_write_formula(worksheet, row + 1, col + 2, data, money);

    /* Save the workbook and free any allocated memory. */
    workbook_close(workbook);

    return 0;
}

int main(int argc, char *argv[])
{
  FILE *f;
  char buf[12];

  if(argc != 2){
    fprintf(stderr, "Must supply a text file\n");
    return -1;
  }
  f = fopen(argv[1], "r");
  if(f == NULL){
    fprintf(stderr, "Could not open %s\n", argv[1]);
    return -1;
  }
  if(fgets(buf, sizeof(buf), f) == NULL){
    fprintf(stderr, "Could not read from %s\n", argv[1]);
    return -1;
  }
  fuzzme(buf);
  return 0;
}