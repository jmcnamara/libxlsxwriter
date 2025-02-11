/*****************************************************************************
 * Test cases for libxlsxwriter.
 *
 * Test to compare output against Excel files.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "xlsxwriter.h"

int main() {

    lxw_workbook  *workbook  = workbook_new("test_outline04.xlsx");
    lxw_worksheet *worksheet4 = workbook_add_worksheet(workbook, "Outline levels");

    lxw_row_col_options level1 = {.level = 1, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level2 = {.level = 2, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level3 = {.level = 3, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level4 = {.level = 4, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level5 = {.level = 5, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level6 = {.level = 6, .hidden = 0, .collapsed = 0};
    lxw_row_col_options level7 = {.level = 7, .hidden = 0, .collapsed = 0};

    worksheet_write_string(worksheet4, 0,  0, "Level 1", NULL);
    worksheet_write_string(worksheet4, 1,  0, "Level 2", NULL);
    worksheet_write_string(worksheet4, 2,  0, "Level 3", NULL);
    worksheet_write_string(worksheet4, 3,  0, "Level 4", NULL);
    worksheet_write_string(worksheet4, 4,  0, "Level 5", NULL);
    worksheet_write_string(worksheet4, 5,  0, "Level 6", NULL);
    worksheet_write_string(worksheet4, 6,  0, "Level 7", NULL);
    worksheet_write_string(worksheet4, 7,  0, "Level 6", NULL);
    worksheet_write_string(worksheet4, 8,  0, "Level 5", NULL);
    worksheet_write_string(worksheet4, 9,  0, "Level 4", NULL);
    worksheet_write_string(worksheet4, 10, 0, "Level 3", NULL);
    worksheet_write_string(worksheet4, 11, 0, "Level 2", NULL);
    worksheet_write_string(worksheet4, 12, 0, "Level 1", NULL);

    worksheet_set_row_opt(worksheet4, 0,  LXW_DEF_ROW_HEIGHT, NULL, &level1);
    worksheet_set_row_opt(worksheet4, 1,  LXW_DEF_ROW_HEIGHT, NULL, &level2);
    worksheet_set_row_opt(worksheet4, 2,  LXW_DEF_ROW_HEIGHT, NULL, &level3);
    worksheet_set_row_opt(worksheet4, 3,  LXW_DEF_ROW_HEIGHT, NULL, &level4);
    worksheet_set_row_opt(worksheet4, 4,  LXW_DEF_ROW_HEIGHT, NULL, &level5);
    worksheet_set_row_opt(worksheet4, 5,  LXW_DEF_ROW_HEIGHT, NULL, &level6);
    worksheet_set_row_opt(worksheet4, 6,  LXW_DEF_ROW_HEIGHT, NULL, &level7);
    worksheet_set_row_opt(worksheet4, 7,  LXW_DEF_ROW_HEIGHT, NULL, &level6);
    worksheet_set_row_opt(worksheet4, 8,  LXW_DEF_ROW_HEIGHT, NULL, &level5);
    worksheet_set_row_opt(worksheet4, 9,  LXW_DEF_ROW_HEIGHT, NULL, &level4);
    worksheet_set_row_opt(worksheet4, 10, LXW_DEF_ROW_HEIGHT, NULL, &level3);
    worksheet_set_row_opt(worksheet4, 11, LXW_DEF_ROW_HEIGHT, NULL, &level2);
    worksheet_set_row_opt(worksheet4, 12, LXW_DEF_ROW_HEIGHT, NULL, &level1);

    return workbook_close(workbook);
}
