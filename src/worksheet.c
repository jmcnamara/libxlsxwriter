/*****************************************************************************
 * worksheet - A library for creating Excel XLSX worksheet files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include <ctype.h>

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/worksheet.h"
#include "xlsxwriter/format.h"
#include "xlsxwriter/utility.h"
#include "xlsxwriter/relationships.h"

#define LXW_STR_MAX 32767
#define BUFFER_SIZE 4096
#define LXW_PORTRAIT 1
#define LXW_LANDSCAPE 0
#define LXW_PRINT_ACROSS 1

/*
 * Forward declarations.
 */
STATIC void _worksheet_write_rows(lxw_worksheet *self);
STATIC int _row_cmp(lxw_row *row1, lxw_row *row2);
STATIC int _cell_cmp(lxw_cell *cell1, lxw_cell *cell2);

LXW_RB_GENERATE_ROW(lxw_table_rows, lxw_row, tree_pointers, _row_cmp);
LXW_RB_GENERATE_CELL(lxw_table_cells, lxw_cell, tree_pointers, _cell_cmp);

/*****************************************************************************
 *
 * Private functions.
 *
 ****************************************************************************/
/*
 * Create a new worksheet object.
 */
lxw_worksheet *
_new_worksheet(lxw_worksheet_init_data *init_data)
{
    lxw_worksheet *worksheet = calloc(1, sizeof(lxw_worksheet));
    GOTO_LABEL_ON_MEM_ERROR(worksheet, mem_error);

    worksheet->table = calloc(1, sizeof(struct lxw_table_rows));
    GOTO_LABEL_ON_MEM_ERROR(worksheet->table, mem_error);

    worksheet->hyperlinks = calloc(1, sizeof(struct lxw_table_rows));
    GOTO_LABEL_ON_MEM_ERROR(worksheet->hyperlinks, mem_error);

    /* Initialise the cached rows. */
    worksheet->table->cached_row_num = LXW_ROW_MAX + 1;
    worksheet->hyperlinks->cached_row_num = LXW_ROW_MAX + 1;

    if (init_data && init_data->optimize) {
        worksheet->array = calloc(LXW_COL_MAX, sizeof(struct lxw_cell *));
        GOTO_LABEL_ON_MEM_ERROR(worksheet->array, mem_error);
    }

    worksheet->col_options =
        calloc(LXW_COL_META_MAX, sizeof(lxw_col_options *));
    worksheet->col_options_max = LXW_COL_META_MAX;
    GOTO_LABEL_ON_MEM_ERROR(worksheet->col_options, mem_error);

    worksheet->col_formats = calloc(LXW_COL_META_MAX, sizeof(lxw_format *));
    worksheet->col_formats_max = LXW_COL_META_MAX;
    GOTO_LABEL_ON_MEM_ERROR(worksheet->col_formats, mem_error);

    worksheet->optimize_row = calloc(1, sizeof(struct lxw_row));
    GOTO_LABEL_ON_MEM_ERROR(worksheet->optimize_row, mem_error);
    worksheet->optimize_row->height = LXW_DEF_ROW_HEIGHT;

    worksheet->merged_ranges = calloc(1, sizeof(struct lxw_merged_ranges));
    GOTO_LABEL_ON_MEM_ERROR(worksheet->merged_ranges, mem_error);

    worksheet->selections = calloc(1, sizeof(struct lxw_selections));
    GOTO_LABEL_ON_MEM_ERROR(worksheet->selections, mem_error);

    worksheet->external_hyperlinks = calloc(1, sizeof(struct lxw_rel_tuples));
    GOTO_LABEL_ON_MEM_ERROR(worksheet->external_hyperlinks, mem_error);

    RB_INIT(worksheet->table);
    RB_INIT(worksheet->hyperlinks);
    STAILQ_INIT(worksheet->merged_ranges);
    STAILQ_INIT(worksheet->selections);
    STAILQ_INIT(worksheet->external_hyperlinks);

    if (init_data && init_data->optimize) {
        worksheet->optimize_tmpfile = lxw_tmpfile();
        worksheet->file = worksheet->optimize_tmpfile;
    }

    /* Initialise the worksheet dimensions. */
    worksheet->dim_rowmax = 0;
    worksheet->dim_colmax = 0;
    worksheet->dim_rowmin = LXW_ROW_MAX;
    worksheet->dim_colmin = LXW_COL_MAX;

    /* Initialise the page setup properties. */
    worksheet->fit_height = 0;
    worksheet->fit_width = 0;
    worksheet->page_start = 0;
    worksheet->print_scale = 100;
    worksheet->fit_page = 0;
    worksheet->orientation = LXW_TRUE;
    worksheet->page_order = 0;
    worksheet->page_setup_changed = LXW_FALSE;
    worksheet->page_view = LXW_FALSE;
    worksheet->paper_size = 0;
    worksheet->vertical_dpi = 0;
    worksheet->horizontal_dpi = 0;
    worksheet->margin_left = 0.7;
    worksheet->margin_right = 0.7;
    worksheet->margin_top = 0.75;
    worksheet->margin_bottom = 0.75;
    worksheet->margin_header = 0.3;
    worksheet->margin_footer = 0.3;
    worksheet->print_gridlines = 0;
    worksheet->screen_gridlines = 1;
    worksheet->print_options_changed = 0;

    if (init_data) {
        worksheet->name = init_data->name;
        worksheet->quoted_name = init_data->quoted_name;
        worksheet->index = init_data->index;
        worksheet->hidden = init_data->hidden;
        worksheet->sst = init_data->sst;
        worksheet->optimize = init_data->optimize;
        worksheet->active_sheet = init_data->active_sheet;

    }

    return worksheet;

mem_error:
    _free_worksheet(worksheet);
    return NULL;
}

/*
 * Free a worksheet cell.
 */
void
_free_cell(lxw_cell *cell)
{
    if (!cell)
        return;

    if (cell->type != NUMBER_CELL && cell->type != STRING_CELL
        && cell->type != BLANK_CELL) {

        free(cell->u.string);
    }

    free(cell->user_data1);
    free(cell->user_data2);

    free(cell);
}

/*
 * Free a worksheet row.
 */
void
_free_row(lxw_row *row)
{
    lxw_cell *cell;
    lxw_cell *next_cell;

    if (!row)
        return;

    for (cell = RB_MIN(lxw_table_cells, row->cells); cell; cell = next_cell) {
        next_cell = RB_NEXT(lxw_table_cells, row->cells, cell);
        RB_REMOVE(lxw_table_cells, row->cells, cell);
        _free_cell(cell);
    }

    free(row->cells);
    free(row);
}

/*
 * Free a worksheet object.
 */
void
_free_worksheet(lxw_worksheet *worksheet)
{
    lxw_row *row;
    lxw_row *next_row;
    lxw_col_t col;
    lxw_merged_range *merged_range;
    lxw_selection *selection;
    lxw_rel_tuple *external_hyperlink;

    if (!worksheet)
        return;

    if (worksheet->col_options) {
        for (col = 0; col < worksheet->col_options_max; col++) {
            if (worksheet->col_options[col])
                free(worksheet->col_options[col]);
        }
    }

    free(worksheet->col_options);
    free(worksheet->col_sizes);
    free(worksheet->col_formats);

    if (worksheet->table) {
        for (row = RB_MIN(lxw_table_rows, worksheet->table); row;
             row = next_row) {
            next_row = RB_NEXT(lxw_table_rows, worksheet->table, row);
            RB_REMOVE(lxw_table_rows, worksheet->table, row);
            _free_row(row);
        }

        free(worksheet->table);
    }

    if (worksheet->hyperlinks) {

        for (row = RB_MIN(lxw_table_rows, worksheet->hyperlinks); row;
             row = next_row) {

            next_row = RB_NEXT(lxw_table_rows, worksheet->hyperlinks, row);
            RB_REMOVE(lxw_table_rows, worksheet->hyperlinks, row);
            _free_row(row);
        }

        free(worksheet->hyperlinks);
    }

    if (worksheet->merged_ranges) {
        while (!STAILQ_EMPTY(worksheet->merged_ranges)) {
            merged_range = STAILQ_FIRST(worksheet->merged_ranges);
            STAILQ_REMOVE_HEAD(worksheet->merged_ranges, list_pointers);
            free(merged_range);
        }

        free(worksheet->merged_ranges);
    }

    if (worksheet->selections) {
        while (!STAILQ_EMPTY(worksheet->selections)) {
            selection = STAILQ_FIRST(worksheet->selections);
            STAILQ_REMOVE_HEAD(worksheet->selections, list_pointers);
            free(selection);
        }

        free(worksheet->selections);
    }

    while (!STAILQ_EMPTY(worksheet->external_hyperlinks)) {
        external_hyperlink = STAILQ_FIRST(worksheet->external_hyperlinks);
        STAILQ_REMOVE_HEAD(worksheet->external_hyperlinks, list_pointers);
        free(external_hyperlink->type);
        free(external_hyperlink->target);
        free(external_hyperlink->target_mode);
        free(external_hyperlink);
    }

    free(worksheet->external_hyperlinks);

    if (worksheet->array) {
        for (col = 0; col < LXW_COL_MAX; col++) {
            _free_cell(worksheet->array[col]);
        }
        free(worksheet->array);
    }

    if (worksheet->optimize_row)
        free(worksheet->optimize_row);

    free(worksheet->name);
    free(worksheet->quoted_name);

    free(worksheet);
    worksheet = NULL;
}

/*
 * Create a new worksheet row object.
 */
STATIC lxw_row *
_new_row(lxw_row_t row_num)
{
    lxw_row *row = calloc(1, sizeof(lxw_row));

    if (row) {
        row->row_num = row_num;
        row->cells = calloc(1, sizeof(struct lxw_table_cells));
        row->height = LXW_DEF_ROW_HEIGHT;

        if (row->cells)
            RB_INIT(row->cells);
        else
            LXW_MEM_ERROR();
    }
    else {
        LXW_MEM_ERROR();
    }

    return row;
}

/*
 * Create a new worksheet number cell object.
 */
STATIC lxw_cell *
_new_number_cell(lxw_row_t row_num,
                 lxw_col_t col_num, double value, lxw_format *format)
{
    lxw_cell *cell = calloc(1, sizeof(lxw_cell));
    RETURN_ON_MEM_ERROR(cell, cell);

    cell->row_num = row_num;
    cell->col_num = col_num;
    cell->type = NUMBER_CELL;
    cell->format = format;
    cell->u.number = value;

    return cell;
}

/*
 * Create a new worksheet string cell object.
 */
STATIC lxw_cell *
_new_string_cell(lxw_row_t row_num,
                 lxw_col_t col_num, int32_t string_id, lxw_format *format)
{
    lxw_cell *cell = calloc(1, sizeof(lxw_cell));
    RETURN_ON_MEM_ERROR(cell, cell);

    cell->row_num = row_num;
    cell->col_num = col_num;
    cell->type = STRING_CELL;
    cell->format = format;
    cell->u.string_id = string_id;

    return cell;
}

/*
 * Create a new worksheet inline_string cell object.
 */
STATIC lxw_cell *
_new_inline_string_cell(lxw_row_t row_num,
                        lxw_col_t col_num, char *string, lxw_format *format)
{
    lxw_cell *cell = calloc(1, sizeof(lxw_cell));
    RETURN_ON_MEM_ERROR(cell, cell);

    cell->row_num = row_num;
    cell->col_num = col_num;
    cell->type = INLINE_STRING_CELL;
    cell->format = format;
    cell->u.string = string;

    return cell;
}

/*
 * Create a new worksheet formula cell object.
 */
STATIC lxw_cell *
_new_formula_cell(lxw_row_t row_num,
                  lxw_col_t col_num, char *formula, lxw_format *format)
{
    lxw_cell *cell = calloc(1, sizeof(lxw_cell));
    RETURN_ON_MEM_ERROR(cell, cell);

    cell->row_num = row_num;
    cell->col_num = col_num;
    cell->type = FORMULA_CELL;
    cell->format = format;
    cell->u.string = formula;

    return cell;
}

/*
 * Create a new worksheet array formula cell object.
 */
STATIC lxw_cell *
_new_array_formula_cell(lxw_row_t row_num, lxw_col_t col_num, char *formula,
                        char *range, lxw_format *format)
{
    lxw_cell *cell = calloc(1, sizeof(lxw_cell));
    RETURN_ON_MEM_ERROR(cell, cell);

    cell->row_num = row_num;
    cell->col_num = col_num;
    cell->type = ARRAY_FORMULA_CELL;
    cell->format = format;
    cell->u.string = formula;
    cell->user_data1 = range;

    return cell;
}

/*
 * Create a new worksheet blank cell object.
 */
STATIC lxw_cell *
_new_blank_cell(lxw_row_t row_num, lxw_col_t col_num, lxw_format *format)
{
    lxw_cell *cell = calloc(1, sizeof(lxw_cell));
    RETURN_ON_MEM_ERROR(cell, cell);

    cell->row_num = row_num;
    cell->col_num = col_num;
    cell->type = BLANK_CELL;
    cell->format = format;

    return cell;
}

/*
 * Create a new worksheet hyperlink cell object.
 */
STATIC lxw_cell *
_new_hyperlink_cell(lxw_row_t row_num, lxw_col_t col_num,
                    enum cell_types link_type, char *url, char *string,
                    char *tooltip)
{
    lxw_cell *cell = calloc(1, sizeof(lxw_cell));
    RETURN_ON_MEM_ERROR(cell, cell);

    cell->row_num = row_num;
    cell->col_num = col_num;
    cell->type = link_type;
    cell->u.string = url;
    cell->user_data1 = string;
    cell->user_data2 = tooltip;

    return cell;
}

/*
 * Get or create the row object for a given row number.
 */
STATIC lxw_row *
_get_row_list(struct lxw_table_rows *table, lxw_row_t row_num)
{
    lxw_row *row;
    lxw_row *existing_row;

    if (table->cached_row_num == row_num)
        return table->cached_row;

    /* Create a new row and try and insert it. */
    row = _new_row(row_num);
    existing_row = RB_INSERT(lxw_table_rows, table, row);

    /* If existing_row is not NULL, then it already existed. Free new row */
    /* and return existing_row. */
    if (existing_row) {
        _free_row(row);
        row = existing_row;
    }

    table->cached_row = row;
    table->cached_row_num = row_num;

    return row;
}

/*
 * Get or create the row object for a given row number.
 */
STATIC lxw_row *
_get_row(lxw_worksheet *self, lxw_row_t row_num)
{
    lxw_row *row;

    if (!self->optimize) {
        row = _get_row_list(self->table, row_num);
        return row;
    }
    else {
        if (row_num < self->optimize_row->row_num) {
            return NULL;
        }
        else if (row_num == self->optimize_row->row_num) {
            return self->optimize_row;
        }
        else {
            /* Flush row. */
            _worksheet_write_single_row(self);
            row = self->optimize_row;
            row->row_num = row_num;
            return row;
        }
    }
}

/*
 * Insert a cell object in the cell list of a row object.
 */
STATIC void
_insert_cell_list(struct lxw_table_cells *cell_list,
                  lxw_cell *cell, lxw_col_t col_num)
{
    lxw_cell *existing_cell;

    cell->col_num = col_num;

    existing_cell = RB_INSERT(lxw_table_cells, cell_list, cell);

    /* If existing_cell is not NULL, then that cell already existed. */
    /* Remove existing_cell and add new one in again. */
    if (existing_cell) {
        RB_REMOVE(lxw_table_cells, cell_list, existing_cell);

        /* Add it in again. */
        RB_INSERT(lxw_table_cells, cell_list, cell);
        _free_cell(existing_cell);
    }

    return;
}

/*
 * Insert a cell object into the cell list or array.
 */
STATIC void
_insert_cell(lxw_worksheet *self, lxw_row_t row_num, lxw_col_t col_num,
             lxw_cell *cell)
{
    lxw_row *row = _get_row(self, row_num);

    if (!self->optimize) {
        row->data_changed = LXW_TRUE;
        _insert_cell_list(row->cells, cell, col_num);
    }
    else {
        if (row) {
            row->data_changed = LXW_TRUE;

            /* Overwrite an existing cell if necessary. */
            if (self->array[col_num])
                _free_cell(self->array[col_num]);

            self->array[col_num] = cell;
        }
    }
}

/*
 * Insert a hyperlink object into the hyperlink list.
 */
STATIC void
_insert_hyperlink(lxw_worksheet *self, lxw_row_t row_num, lxw_col_t col_num,
                  lxw_cell *link)
{
    lxw_row *row = _get_row_list(self->hyperlinks, row_num);

    _insert_cell_list(row->cells, link, col_num);
}

/*
 * Next power of two for column reallocs. Taken from bithacks in the public
 * domain.
 */
STATIC lxw_col_t
_next_power_of_two(uint16_t col)
{
    col--;
    col |= col >> 1;
    col |= col >> 2;
    col |= col >> 4;
    col |= col >> 8;
    col++;

    return col;
}

/*
 * Check that row and col are within the allowed Excel range and store max
 * and min values for use in other methods/elements.
 *
 * The ignore_row/ignore_col flags are used to indicate that we wish to
 * perform the dimension check without storing the value.
 */
STATIC int8_t
_check_dimensions(lxw_worksheet *self,
                  lxw_row_t row_num,
                  lxw_col_t col_num, int8_t ignore_row, int8_t ignore_col)
{
    if (row_num >= LXW_ROW_MAX)
        return -LXW_ERROR_WORKSHEET_INDEX_OUT_OF_RANGE;

    if (col_num >= LXW_COL_MAX)
        return -LXW_ERROR_WORKSHEET_INDEX_OUT_OF_RANGE;

    /* In optimization mode we don't change dimensions for rows that are */
    /* already written. */
    if (!ignore_row && !ignore_col && self->optimize) {
        if (row_num < self->optimize_row->row_num)
            return -LXW_ERROR_WORKSHEET_INDEX_OUT_OF_RANGE;
    }

    if (!ignore_row) {
        if (row_num < self->dim_rowmin)
            self->dim_rowmin = row_num;
        if (row_num > self->dim_rowmax)
            self->dim_rowmax = row_num;
    }

    if (!ignore_col) {
        if (col_num < self->dim_colmin)
            self->dim_colmin = col_num;
        if (col_num > self->dim_colmax)
            self->dim_colmax = col_num;
    }

    return 0;
}

/*
 * Comparator for the row structure red/black tree.
 */
STATIC int
_row_cmp(lxw_row *row1, lxw_row *row2)
{
    if (row1->row_num > row2->row_num)
        return 1;
    if (row1->row_num < row2->row_num)
        return -1;
    return 0;
}

/*
 * Comparator for the cell structure red/black tree.
 */
STATIC int
_cell_cmp(lxw_cell *cell1, lxw_cell *cell2)
{
    if (cell1->col_num > cell2->col_num)
        return 1;
    if (cell1->col_num < cell2->col_num)
        return -1;
    return 0;
}

/*****************************************************************************
 *
 * XML functions.
 *
 ****************************************************************************/
/*
 * Write the XML declaration.
 */
STATIC void
_worksheet_xml_declaration(lxw_worksheet *self)
{
    _xml_declaration(self->file);
}

/*
 * Write the <worksheet> element.
 */
STATIC void
_worksheet_write_worksheet(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char xmlns[] = "http://schemas.openxmlformats.org/"
        "spreadsheetml/2006/main";
    char xmlns_r[] = "http://schemas.openxmlformats.org/"
        "officeDocument/2006/relationships";

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("xmlns", xmlns);
    _PUSH_ATTRIBUTES_STR("xmlns:r", xmlns_r);

    _xml_start_tag(self->file, "worksheet", &attributes);
    _FREE_ATTRIBUTES();
}

/*
 * Write the <dimension> element.
 */
STATIC void
_worksheet_write_dimension(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char ref[MAX_CELL_RANGE_LENGTH];
    lxw_row_t dim_rowmin = self->dim_rowmin;
    lxw_row_t dim_rowmax = self->dim_rowmax;
    lxw_col_t dim_colmin = self->dim_colmin;
    lxw_col_t dim_colmax = self->dim_colmax;

    if (dim_rowmin == LXW_ROW_MAX && dim_colmin == LXW_COL_MAX) {
        /* If the rows and cols are still the defaults then no dimensions have
         * been set and we use the default range "A1". */
        lxw_range(ref, 0, 0, 0, 0);
    }
    else if (dim_rowmin == LXW_ROW_MAX && dim_colmin != LXW_COL_MAX) {
        /* If the rows aren't set but the columns are then the dimensions have
         * been changed via set_column(). */
        lxw_range(ref, 0, dim_colmin, 0, dim_colmax);
    }
    else {
        lxw_range(ref, dim_rowmin, dim_colmin, dim_rowmax, dim_colmax);
    }

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("ref", ref);

    _xml_empty_tag(self->file, "dimension", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <pane> element for freeze panes.
 */
STATIC void
_worksheet_write_freeze_panes(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    lxw_selection *selection;
    lxw_selection *user_selection;
    lxw_row_t row = self->panes.first_row;
    lxw_col_t col = self->panes.first_col;
    lxw_row_t top_row = self->panes.top_row;
    lxw_col_t left_col = self->panes.left_col;

    char row_cell[MAX_CELL_NAME_LENGTH];
    char col_cell[MAX_CELL_NAME_LENGTH];
    char top_left_cell[MAX_CELL_NAME_LENGTH];
    char active_pane[LXW_PANE_NAME_LENGTH];

    user_selection = calloc(1, sizeof(lxw_selection));
    RETURN_VOID_ON_MEM_ERROR(user_selection);

    _INIT_ATTRIBUTES();

    lxw_rowcol_to_cell(top_left_cell, top_row, left_col);

    /* Set the active pane. */
    if (row && col) {
        strcpy(active_pane, "bottomRight");

        lxw_rowcol_to_cell(row_cell, row, 0);
        lxw_rowcol_to_cell(col_cell, 0, col);

        selection = calloc(1, sizeof(lxw_selection));
        if (selection) {
            strcpy(selection->pane, "topRight");
            strcpy(selection->active_cell, col_cell);
            strcpy(selection->sqref, col_cell);

            STAILQ_INSERT_TAIL(self->selections, selection, list_pointers);
        }

        selection = calloc(1, sizeof(lxw_selection));
        if (selection) {
            strcpy(selection->pane, "bottomLeft");
            strcpy(selection->active_cell, row_cell);
            strcpy(selection->sqref, row_cell);

            STAILQ_INSERT_TAIL(self->selections, selection, list_pointers);
        }

        selection = calloc(1, sizeof(lxw_selection));
        if (selection) {
            strcpy(selection->pane, "bottomRight");
            strcpy(selection->active_cell, user_selection->active_cell);
            strcpy(selection->sqref, user_selection->sqref);

            STAILQ_INSERT_TAIL(self->selections, selection, list_pointers);
        }
    }
    else if (col) {
        strcpy(active_pane, "topRight");

        selection = calloc(1, sizeof(lxw_selection));
        if (selection) {
            strcpy(selection->pane, "topRight");
            strcpy(selection->active_cell, user_selection->active_cell);
            strcpy(selection->sqref, user_selection->sqref);

            STAILQ_INSERT_TAIL(self->selections, selection, list_pointers);
        }
    }
    else {
        strcpy(active_pane, "bottomLeft");

        selection = calloc(1, sizeof(lxw_selection));
        if (selection) {
            strcpy(selection->pane, "bottomLeft");
            strcpy(selection->active_cell, user_selection->active_cell);
            strcpy(selection->sqref, user_selection->sqref);

            STAILQ_INSERT_TAIL(self->selections, selection, list_pointers);
        }
    }

    if (col)
        _PUSH_ATTRIBUTES_INT("xSplit", col);

    if (row)
        _PUSH_ATTRIBUTES_INT("ySplit", row);

    _PUSH_ATTRIBUTES_STR("topLeftCell", top_left_cell);
    _PUSH_ATTRIBUTES_STR("activePane", active_pane);

    if (self->panes.type == FREEZE_PANES)
        _PUSH_ATTRIBUTES_STR("state", "frozen");
    else if (self->panes.type == FREEZE_SPLIT_PANES)
        _PUSH_ATTRIBUTES_STR("state", "frozenSplit");

    _xml_empty_tag(self->file, "pane", &attributes);

    free(user_selection);

    _FREE_ATTRIBUTES();
}

/*
 * Convert column width from user units to pane split width.
 */
STATIC uint32_t
_worksheet_calculate_x_split_width(double x_split)
{
    uint32_t width;
    uint32_t pixels;
    uint32_t points;
    uint32_t twips;
    double max_digit_width = 7.0;       /* For Calabri 11. */
    double padding = 5.0;

    /* Convert to pixels. */
    if (x_split < 1.0) {
        pixels = (uint32_t) (x_split * (max_digit_width + padding) + 0.5);
    }
    else {
        pixels = (uint32_t) (x_split * max_digit_width + 0.5) + 5;
    }

    /* Convert to points. */
    points = (pixels * 3) / 4;

    /* Convert to twips (twentieths of a point). */
    twips = points * 20;

    /* Add offset/padding. */
    width = twips + 390;

    return width;
}

/*
 * Write the <pane> element for split panes.
 */
STATIC void
_worksheet_write_split_panes(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    lxw_selection *selection;
    lxw_selection *user_selection;
    lxw_row_t row = self->panes.first_row;
    lxw_col_t col = self->panes.first_col;
    lxw_row_t top_row = self->panes.top_row;
    lxw_col_t left_col = self->panes.left_col;
    double x_split = self->panes.x_split;
    double y_split = self->panes.y_split;
    uint8_t has_selection = LXW_FALSE;

    char row_cell[MAX_CELL_NAME_LENGTH];
    char col_cell[MAX_CELL_NAME_LENGTH];
    char top_left_cell[MAX_CELL_NAME_LENGTH];
    char active_pane[LXW_PANE_NAME_LENGTH];

    user_selection = calloc(1, sizeof(lxw_selection));
    RETURN_VOID_ON_MEM_ERROR(user_selection);

    _INIT_ATTRIBUTES();

    /* Convert the row and col to 1/20 twip units with padding. */
    if (y_split > 0.0)
        y_split = (uint32_t) (20 * y_split + 300);

    if (x_split > 0.0)
        x_split = _worksheet_calculate_x_split_width(x_split);

    /* For non-explicit topLeft definitions, estimate the cell offset based on
     * the pixels dimensions. This is only a workaround and doesn"t take
     * adjusted cell dimensions into account.
     */
    if (top_row == row && left_col == col) {
        top_row = (lxw_row_t) (0.5 + (y_split - 300.0) / 20.0 / 15.0);
        left_col = (lxw_col_t) (0.5 + (x_split - 390.0) / 20.0 / 3.0 / 16.0);
    }

    lxw_rowcol_to_cell(top_left_cell, top_row, left_col);

    /* If there is no selection set the active cell to the top left cell. */
    if (!has_selection) {
        strcpy(user_selection->active_cell, top_left_cell);
        strcpy(user_selection->sqref, top_left_cell);
    }

    /* Set the active pane. */
    if (y_split > 0.0 && x_split > 0.0) {
        strcpy(active_pane, "bottomRight");

        lxw_rowcol_to_cell(row_cell, top_row, 0);
        lxw_rowcol_to_cell(col_cell, 0, left_col);

        selection = calloc(1, sizeof(lxw_selection));
        if (selection) {
            strcpy(selection->pane, "topRight");
            strcpy(selection->active_cell, col_cell);
            strcpy(selection->sqref, col_cell);

            STAILQ_INSERT_TAIL(self->selections, selection, list_pointers);
        }

        selection = calloc(1, sizeof(lxw_selection));
        if (selection) {
            strcpy(selection->pane, "bottomLeft");
            strcpy(selection->active_cell, row_cell);
            strcpy(selection->sqref, row_cell);

            STAILQ_INSERT_TAIL(self->selections, selection, list_pointers);
        }

        selection = calloc(1, sizeof(lxw_selection));
        if (selection) {
            strcpy(selection->pane, "bottomRight");
            strcpy(selection->active_cell, user_selection->active_cell);
            strcpy(selection->sqref, user_selection->sqref);

            STAILQ_INSERT_TAIL(self->selections, selection, list_pointers);
        }
    }
    else if (x_split > 0.0) {
        strcpy(active_pane, "topRight");

        selection = calloc(1, sizeof(lxw_selection));
        if (selection) {
            strcpy(selection->pane, "topRight");
            strcpy(selection->active_cell, user_selection->active_cell);
            strcpy(selection->sqref, user_selection->sqref);

            STAILQ_INSERT_TAIL(self->selections, selection, list_pointers);
        }
    }
    else {
        strcpy(active_pane, "bottomLeft");

        selection = calloc(1, sizeof(lxw_selection));
        if (selection) {
            strcpy(selection->pane, "bottomLeft");
            strcpy(selection->active_cell, user_selection->active_cell);
            strcpy(selection->sqref, user_selection->sqref);

            STAILQ_INSERT_TAIL(self->selections, selection, list_pointers);
        }
    }

    if (x_split > 0.0)
        _PUSH_ATTRIBUTES_DBL("xSplit", x_split);

    if (y_split > 0.0)
        _PUSH_ATTRIBUTES_DBL("ySplit", y_split);

    _PUSH_ATTRIBUTES_STR("topLeftCell", top_left_cell);

    if (has_selection)
        _PUSH_ATTRIBUTES_STR("activePane", active_pane);

    _xml_empty_tag(self->file, "pane", &attributes);

    free(user_selection);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <selection> element.
 */
STATIC void
_worksheet_write_selection(lxw_worksheet *self, lxw_selection * selection)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    _INIT_ATTRIBUTES();

    if (*selection->pane)
        _PUSH_ATTRIBUTES_STR("pane", selection->pane);

    if (*selection->active_cell)
        _PUSH_ATTRIBUTES_STR("activeCell", selection->active_cell);

    if (*selection->sqref)
        _PUSH_ATTRIBUTES_STR("sqref", selection->sqref);

    _xml_empty_tag(self->file, "selection", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <selection> elements.
 */
STATIC void
_worksheet_write_selections(lxw_worksheet *self)
{
    lxw_selection *selection;

    STAILQ_FOREACH(selection, self->selections, list_pointers) {
        _worksheet_write_selection(self, selection);
    }
}

/*
 * Write the frozen or split <pane> elements.
 */
STATIC void
_worksheet_write_panes(lxw_worksheet *self)
{
    if (self->panes.type == NO_PANES)
        return;

    else if (self->panes.type == FREEZE_PANES)
        _worksheet_write_freeze_panes(self);

    else if (self->panes.type == FREEZE_SPLIT_PANES)
        _worksheet_write_freeze_panes(self);

    else if (self->panes.type == SPLIT_PANES)
        _worksheet_write_split_panes(self);
}

/*
 * Write the <sheetView> element.
 */
STATIC void
_worksheet_write_sheet_view(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    _INIT_ATTRIBUTES();

    if (!self->screen_gridlines)
        _PUSH_ATTRIBUTES_STR("showGridLines", "0");

    if (self->selected)
        _PUSH_ATTRIBUTES_STR("tabSelected", "1");

    /* Set the page view/layout mode if required. */
    if (self->page_view)
        _PUSH_ATTRIBUTES_STR("view", "pageLayout");

    _PUSH_ATTRIBUTES_STR("workbookViewId", "0");

    if (self->panes.type != NO_PANES || !STAILQ_EMPTY(self->selections)) {
        _xml_start_tag(self->file, "sheetView", &attributes);
        _worksheet_write_panes(self);
        _worksheet_write_selections(self);
        _xml_end_tag(self->file, "sheetView");
    }
    else {
        _xml_empty_tag(self->file, "sheetView", &attributes);
    }

    _FREE_ATTRIBUTES();
}

/*
 * Write the <sheetViews> element.
 */
STATIC void
_worksheet_write_sheet_views(lxw_worksheet *self)
{
    _xml_start_tag(self->file, "sheetViews", NULL);

    /* Write the sheetView element. */
    _worksheet_write_sheet_view(self);

    _xml_end_tag(self->file, "sheetViews");
}

/*
 * Write the <sheetFormatPr> element.
 */
STATIC void
_worksheet_write_sheet_format_pr(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char default_row_height[] = "15";

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("defaultRowHeight", default_row_height);

    _xml_empty_tag(self->file, "sheetFormatPr", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <sheetData> element.
 */
STATIC void
_worksheet_write_sheet_data(lxw_worksheet *self)
{
    if (RB_EMPTY(self->table)) {
        _xml_empty_tag(self->file, "sheetData", NULL);
    }
    else {
        _xml_start_tag(self->file, "sheetData", NULL);
        _worksheet_write_rows(self);
        _xml_end_tag(self->file, "sheetData");
    }
}

/*
 * Write the <sheetData> element when the memory optimisation is on. In which
 * case we read the data stored in the temp file and rewrite it to the XML
 * sheet file.
 */
STATIC void
_worksheet_write_optimized_sheet_data(lxw_worksheet *self)
{
    uint16_t read_size = 1;
    char buffer[BUFFER_SIZE];

    if (self->dim_rowmin == LXW_ROW_MAX) {
        /* If the dimensions aren"t defined then there is no data to write. */
        _xml_empty_tag(self->file, "sheetData", NULL);
    }
    else {

        _xml_start_tag(self->file, "sheetData", NULL);

        /* Flush and rewind the temp file. */
        fflush(self->optimize_tmpfile);
        rewind(self->optimize_tmpfile);

        while (read_size) {
            read_size = fread(buffer, 1, BUFFER_SIZE, self->optimize_tmpfile);
            fwrite(buffer, 1, read_size, self->file);
        }

        fclose(self->optimize_tmpfile);

        _xml_end_tag(self->file, "sheetData");
    }
}

/*
 * Write the <pageMargins> element.
 */
STATIC void
_worksheet_write_page_margins(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    double left = self->margin_left;
    double right = self->margin_right;
    double top = self->margin_top;
    double bottom = self->margin_bottom;
    double header = self->margin_header;
    double footer = self->margin_footer;

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_DBL("left", left);
    _PUSH_ATTRIBUTES_DBL("right", right);
    _PUSH_ATTRIBUTES_DBL("top", top);
    _PUSH_ATTRIBUTES_DBL("bottom", bottom);
    _PUSH_ATTRIBUTES_DBL("header", header);
    _PUSH_ATTRIBUTES_DBL("footer", footer);

    _xml_empty_tag(self->file, "pageMargins", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <pageSetup> element.
 * The following is an example taken from Excel.
 * <pageSetup
 *     paperSize="9"
 *     scale="110"
 *     fitToWidth="2"
 *     fitToHeight="2"
 *     pageOrder="overThenDown"
 *     orientation="portrait"
 *     blackAndWhite="1"
 *     draft="1"
 *     horizontalDpi="200"
 *     verticalDpi="200"
 *     r:id="rId1"
 * />
 */
STATIC void
_worksheet_write_page_setup(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    _INIT_ATTRIBUTES();

    if (!self->page_setup_changed)
        return;

    /* Set paper size. */
    if (self->paper_size)
        _PUSH_ATTRIBUTES_INT("paperSize", self->paper_size);

    /* Set the print_scale. */
    if (self->print_scale != 100)
        _PUSH_ATTRIBUTES_INT("scale", self->print_scale);

    /* Set the "Fit to page" properties. */
    if (self->fit_page && self->fit_width != 1)
        _PUSH_ATTRIBUTES_INT("fitToWidth", self->fit_width);

    if (self->fit_page && self->fit_height != 1)
        _PUSH_ATTRIBUTES_INT("fitToHeight", self->fit_height);

    /* Set the page print direction. */
    if (self->page_order)
        _PUSH_ATTRIBUTES_STR("pageOrder", "overThenDown");

    /* Set start page. */
    if (self->page_start > 1)
        _PUSH_ATTRIBUTES_INT("firstPageNumber", self->page_start);

    /* Set page orientation. */
    if (self->orientation)
        _PUSH_ATTRIBUTES_STR("orientation", "portrait");
    else
        _PUSH_ATTRIBUTES_STR("orientation", "landscape");

    /* Set start page active flag. */
    if (self->page_start)
        _PUSH_ATTRIBUTES_INT("useFirstPageNumber", 1);

    /* Set the DPI. Mainly only for testing. */
    if (self->horizontal_dpi)
        _PUSH_ATTRIBUTES_INT("horizontalDpi", self->horizontal_dpi);

    if (self->vertical_dpi)
        _PUSH_ATTRIBUTES_INT("verticalDpi", self->vertical_dpi);

    _xml_empty_tag(self->file, "pageSetup", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <printOptions> element.
 */
STATIC void
_worksheet_write_print_options(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    if (!self->print_options_changed)
        return;

    _INIT_ATTRIBUTES();

    /* Set horizontal centering. */
    if (self->hcenter) {
        _PUSH_ATTRIBUTES_STR("horizontalCentered", "1");
    }

    /* Set vertical centering. */
    if (self->vcenter) {
        _PUSH_ATTRIBUTES_STR("verticalCentered", "1");
    }

    /* Enable row and column headers. */
    if (self->print_headers) {
        _PUSH_ATTRIBUTES_STR("headings", "1");
    }

    /* Set printed gridlines. */
    if (self->print_gridlines) {
        _PUSH_ATTRIBUTES_STR("gridLines", "1");
    }

    _xml_empty_tag(self->file, "printOptions", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <row> element.
 */
STATIC void
_write_row(lxw_worksheet *self, lxw_row *row, char *spans)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    int32_t xf_index = 0;

    if (row->format) {
        xf_index = _get_xf_index(row->format);
    }

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_INT("r", row->row_num + 1);

    if (spans)
        _PUSH_ATTRIBUTES_STR("spans", spans);

    if (xf_index)
        _PUSH_ATTRIBUTES_INT("s", xf_index);

    if (row->format)
        _PUSH_ATTRIBUTES_STR("customFormat", "1");

    if (row->height != LXW_DEF_ROW_HEIGHT)
        _PUSH_ATTRIBUTES_INT("ht", row->height);

    if (row->hidden)
        _PUSH_ATTRIBUTES_STR("hidden", "1");

    if (row->height != LXW_DEF_ROW_HEIGHT)
        _PUSH_ATTRIBUTES_STR("customHeight", "1");

    if (row->collapsed)
        _PUSH_ATTRIBUTES_STR("collapsed", "1");

    if (!row->data_changed)
        _xml_empty_tag(self->file, "row", &attributes);
    else
        _xml_start_tag(self->file, "row", &attributes);

    _FREE_ATTRIBUTES();
}

/*****************************************************************************
 *
 * XML file assembly functions.
 *
 ****************************************************************************/

/*
 * Write out a number worksheet cell.
 */
STATIC void
_write_number_cell(lxw_worksheet *self, lxw_cell *cell)
{
    char data[ATTR_32];

    lxw_snprintf(data, ATTR_32, "%.16g", cell->u.number);

    _xml_data_element(self->file, "v", data, NULL);

}

/*
 * Write out a string worksheet cell.
 */
STATIC void
_write_string_cell(lxw_worksheet *self, lxw_cell *cell)
{
    char data[ATTR_32];

    lxw_snprintf(data, ATTR_32, "%d", cell->u.string_id);

    _xml_data_element(self->file, "v", data, NULL);
}

/*
 * Write out a formula worksheet cell with a numeric result.
 */
STATIC void
_write_formula_num_cell(lxw_worksheet *self, lxw_cell *cell)
{
    char data[ATTR_32];

    lxw_snprintf(data, ATTR_32, "%.16g", cell->formula_result);

    _xml_data_element(self->file, "f", cell->u.string, NULL);
    _xml_data_element(self->file, "v", data, NULL);
}

/*
 * Write out an array formula worksheet cell with a numeric result.
 */
STATIC void
_write_array_formula_num_cell(lxw_worksheet *self, lxw_cell *cell)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char data[ATTR_32];

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("t", "array");
    _PUSH_ATTRIBUTES_STR("ref", cell->user_data1);

    lxw_snprintf(data, ATTR_32, "%.16g", cell->formula_result);

    _xml_data_element(self->file, "f", cell->u.string, &attributes);
    _xml_data_element(self->file, "v", data, NULL);

    _FREE_ATTRIBUTES();
}

/*
 * Write out an inline string.
 */
STATIC void
_write_inline_string_cell(lxw_worksheet *self, lxw_cell *cell)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char *string = cell->u.string;

    _INIT_ATTRIBUTES();

    /* Add attribute to preserve leading or trailing whitespace. */
    if (isspace((unsigned char) string[0])
        || isspace((unsigned char) string[strlen(string) - 1]))
        _PUSH_ATTRIBUTES_STR("xml:space", "preserve");

    _xml_start_tag(self->file, "is", NULL);
    _xml_data_element(self->file, "t", string, &attributes);
    _xml_end_tag(self->file, "is");

    _FREE_ATTRIBUTES();
}

/*
 * Calculate the "spans" attribute of the <row> tag. This is an XLSX
 * optimisation and isn't strictly required. However, it makes comparing
 * files easier.
 *
 * The span is the same for each block of 16 rows.
 */
STATIC void
_calculate_spans(struct lxw_row *row, char *span, int32_t *block_num)
{
    lxw_col_t span_col_min = RB_MIN(lxw_table_cells, row->cells)->col_num;
    lxw_col_t span_col_max = RB_MAX(lxw_table_cells, row->cells)->col_num;
    lxw_col_t col_min;
    lxw_col_t col_max;
    *block_num = row->row_num / 16;

    row = RB_NEXT(lxw_table_rows, root, row);

    while (row && (int32_t) (row->row_num / 16) == *block_num) {

        if (!RB_EMPTY(row->cells)) {
            col_min = RB_MIN(lxw_table_cells, row->cells)->col_num;
            col_max = RB_MAX(lxw_table_cells, row->cells)->col_num;

            if (col_min < span_col_min)
                span_col_min = col_min;

            if (col_max > span_col_max)
                span_col_max = col_max;
        }

        row = RB_NEXT(lxw_table_rows, root, row);
    }

    lxw_snprintf(span, MAX_CELL_RANGE_LENGTH,
                 "%d:%d", span_col_min + 1, span_col_max + 1);
}

/*
 * Write out a generic worksheet cell.
 */
STATIC void
_write_cell(lxw_worksheet *self, lxw_cell *cell, lxw_format *row_format)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char range[MAX_CELL_NAME_LENGTH] = { 0 };
    lxw_row_t row_num = cell->row_num;
    lxw_col_t col_num = cell->col_num;
    int32_t index = 0;

    lxw_rowcol_to_cell(range, row_num, col_num);

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("r", range);

    if (cell->format) {
        index = _get_xf_index(cell->format);
    }
    else if (row_format) {
        index = _get_xf_index(row_format);
    }
    else if (col_num < self->col_formats_max && self->col_formats[col_num]) {
        index = _get_xf_index(self->col_formats[col_num]);
    }

    if (index)
        _PUSH_ATTRIBUTES_INT("s", index);

    if (cell->type == NUMBER_CELL) {
        _xml_start_tag(self->file, "c", &attributes);
        _write_number_cell(self, cell);
        _xml_end_tag(self->file, "c");
    }
    else if (cell->type == STRING_CELL) {
        _PUSH_ATTRIBUTES_STR("t", "s");
        _xml_start_tag(self->file, "c", &attributes);
        _write_string_cell(self, cell);
        _xml_end_tag(self->file, "c");
    }
    else if (cell->type == INLINE_STRING_CELL) {
        _PUSH_ATTRIBUTES_STR("t", "inlineStr");
        _xml_start_tag(self->file, "c", &attributes);
        _write_inline_string_cell(self, cell);
        _xml_end_tag(self->file, "c");
    }
    else if (cell->type == FORMULA_CELL) {
        _xml_start_tag(self->file, "c", &attributes);
        _write_formula_num_cell(self, cell);
        _xml_end_tag(self->file, "c");
    }
    else if (cell->type == BLANK_CELL) {
        _xml_empty_tag(self->file, "c", &attributes);
    }
    else if (cell->type == ARRAY_FORMULA_CELL) {
        _xml_start_tag(self->file, "c", &attributes);
        _write_array_formula_num_cell(self, cell);
        _xml_end_tag(self->file, "c");
    }

    _FREE_ATTRIBUTES();
}

/*
 * Write out the worksheet data as a series of rows and cells.
 */
STATIC void
_worksheet_write_rows(lxw_worksheet *self)
{
    lxw_row *row;
    lxw_cell *cell;
    int32_t block_num = -1;
    char spans[MAX_CELL_RANGE_LENGTH] = { 0 };

    RB_FOREACH(row, lxw_table_rows, self->table) {

        if (RB_EMPTY(row->cells)) {
            /* Row data only. No cells. */
            _write_row(self, row, NULL);
        }
        else {
            /* Row and cell data. */
            if ((int32_t) row->row_num / 16 > block_num)
                _calculate_spans(row, spans, &block_num);

            _write_row(self, row, spans);

            RB_FOREACH(cell, lxw_table_cells, row->cells) {
                _write_cell(self, cell, row->format);
            }
            _xml_end_tag(self->file, "row");
        }
    }
}

/*
 * Write out the worksheet data as a single row with cells. This method is
 * used when memory optimisation is on. A single row is written and the data
 * array is reset. That way only one row of data is kept in memory at any one
 * time. We don't write span data in the optimised case since it is optional.
 */
void
_worksheet_write_single_row(lxw_worksheet *self)
{
    lxw_row *row = self->optimize_row;
    lxw_col_t col;

    /* skip row if it doesn"t contain row formatting, cell data or a comment. */
    if (!(row->row_changed || row->data_changed))
        return;

    /* Write the cells if the row contains data. */
    if (!row->data_changed) {
        /* Row data only. No cells. */
        _write_row(self, row, NULL);
    }
    else {
        /* Row and cell data. */
        _write_row(self, row, NULL);

        for (col = self->dim_colmin; col <= self->dim_colmax; col++) {
            if (self->array[col]) {
                _write_cell(self, self->array[col], row->format);
                _free_cell(self->array[col]);
                self->array[col] = NULL;
            }
        }

        _xml_end_tag(self->file, "row");
    }

    /* Reset the row. */
    row->height = LXW_DEF_ROW_HEIGHT;
    row->format = NULL;
    row->hidden = LXW_FALSE;
    row->level = 0;
    row->collapsed = LXW_FALSE;
    row->data_changed = LXW_FALSE;
    row->row_changed = LXW_FALSE;
}

/*
 * Write the <col> element.
 */
STATIC void
_worksheet_write_col_info(lxw_worksheet *self, lxw_col_options *options)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    double width = options->width;
    uint8_t has_custom_width = LXW_TRUE;
    int32_t xf_index = 0;
    double max_digit_width = 7.0;       /* For Calabri 11. */
    double padding = 5.0;

    /* Get the format index. */
    if (options->format) {
        xf_index = _get_xf_index(options->format);
    }

    /* Check if width is the Excel default. */
    if (width == LXW_DEF_COL_WIDTH) {

        /* The default col width changes to 0 for hidden columns. */
        if (options->hidden)
            width = 0;
        else
            has_custom_width = LXW_FALSE;

    }

    /* Convert column width from user units to character width. */
    if (width > 0) {
        if (width < 1) {
            width = (uint16_t) (((uint16_t)
                                 (width * (max_digit_width + padding) + 0.5))
                                / max_digit_width * 256.0) / 256.0;
        }
        else {
            width = (uint16_t) (((uint16_t)
                                 (width * max_digit_width + 0.5) + padding)
                                / max_digit_width * 256.0) / 256.0;
        }
    }

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_INT("min", 1 + options->firstcol);
    _PUSH_ATTRIBUTES_INT("max", 1 + options->lastcol);
    _PUSH_ATTRIBUTES_DBL("width", width);

    if (xf_index)
        _PUSH_ATTRIBUTES_INT("style", xf_index);

    if (options->hidden)
        _PUSH_ATTRIBUTES_STR("hidden", "1");

    if (has_custom_width)
        _PUSH_ATTRIBUTES_STR("customWidth", "1");

    if (options->level)
        _PUSH_ATTRIBUTES_INT("outlineLevel", options->level);

    if (options->collapsed)
        _PUSH_ATTRIBUTES_STR("collapsed", "1");

    _xml_empty_tag(self->file, "col", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <cols> element and <col> sub elements.
 */
STATIC void
_worksheet_write_cols(lxw_worksheet *self)
{
    lxw_col_t col;

    if (!self->col_size_changed)
        return;

    _xml_start_tag(self->file, "cols", NULL);

    for (col = 0; col < self->col_options_max; col++) {
        if (self->col_options[col])
            _worksheet_write_col_info(self, self->col_options[col]);
    }

    _xml_end_tag(self->file, "cols");
}

/*
 * Write the <mergeCell> element.
 */
STATIC void
_worksheet_write_merge_cell(lxw_worksheet *self,
                            lxw_merged_range *merged_range)
{

    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char ref[MAX_CELL_RANGE_LENGTH];

    _INIT_ATTRIBUTES();

    /* Convert the merge dimensions to a cell range. */
    lxw_range(ref, merged_range->first_row, merged_range->first_col,
              merged_range->last_row, merged_range->last_col);

    _PUSH_ATTRIBUTES_STR("ref", ref);

    _xml_empty_tag(self->file, "mergeCell", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <mergeCells> element.
 */
STATIC void
_worksheet_write_merge_cells(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    lxw_merged_range *merged_range;

    if (self->merged_range_count) {
        _INIT_ATTRIBUTES();

        _PUSH_ATTRIBUTES_INT("count", self->merged_range_count);

        _xml_start_tag(self->file, "mergeCells", &attributes);

        STAILQ_FOREACH(merged_range, self->merged_ranges, list_pointers) {
            _worksheet_write_merge_cell(self, merged_range);
        }
        _xml_end_tag(self->file, "mergeCells");

        _FREE_ATTRIBUTES();
    }
}

/*
 * Write the <oddHeader> element.
 */
STATIC void
_worksheet_write_odd_header(lxw_worksheet *self)
{
    _xml_data_element(self->file, "oddHeader", self->header, NULL);
}

/*
 * Write the <oddFooter> element.
 */
STATIC void
_worksheet_write_odd_footer(lxw_worksheet *self)
{
    _xml_data_element(self->file, "oddFooter", self->footer, NULL);
}

/*
 * Write the <headerFooter> element.
 */
STATIC void
_worksheet_write_header_footer(lxw_worksheet *self)
{
    if (!self->header_footer_changed)
        return;

    _xml_start_tag(self->file, "headerFooter", NULL);

    if (self->header[0] != '\0')
        _worksheet_write_odd_header(self);

    if (self->footer[0] != '\0')
        _worksheet_write_odd_footer(self);

    _xml_end_tag(self->file, "headerFooter");
}

/*
 * Write the <pageSetUpPr> element.
 */
STATIC void
_worksheet_write_page_set_up_pr(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    if (!self->fit_page)
        return;

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("fitToPage", "1");

    _xml_empty_tag(self->file, "pageSetUpPr", &attributes);

    _FREE_ATTRIBUTES();

}

/*
 * Write the <sheetPr> element for Sheet level properties.
 */
STATIC void
_worksheet_write_sheet_pr(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    if (!self->fit_page
        && !self->filter_on
        && !self->tab_color
        && !self->outline_changed && !self->vba_codename) {
        return;
    }

    _INIT_ATTRIBUTES();

    if (self->vba_codename)
        _PUSH_ATTRIBUTES_INT("codeName", self->vba_codename);

    if (self->filter_on)
        _PUSH_ATTRIBUTES_STR("filterMode", "1");

    if (self->fit_page || self->tab_color || self->outline_changed) {
        _xml_start_tag(self->file, "sheetPr", &attributes);
        /* _worksheet_write_tab_color(self); */
        /* _worksheet_write_outline_pr(self); */
        _worksheet_write_page_set_up_pr(self);
        _xml_end_tag(self->file, "sheetPr");
    }
    else {
        _xml_empty_tag(self->file, "sheetPr", &attributes);
    }

    _FREE_ATTRIBUTES();

}

/*
 * Write the <brk> element.
 */
STATIC void
_worksheet_write_brk(lxw_worksheet *self, uint32_t id, uint32_t max)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_INT("id", id);
    _PUSH_ATTRIBUTES_INT("max", max);
    _PUSH_ATTRIBUTES_STR("man", "1");

    _xml_empty_tag(self->file, "brk", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <rowBreaks> element.
 */
STATIC void
_worksheet_write_row_breaks(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    uint16_t count = 0;
    uint16_t i;

    if (self->hbreaks == NULL)
        return;

    while (self->hbreaks[count])
        count++;

    if (count > LXW_BREAKS_MAX)
        count = LXW_BREAKS_MAX;

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_INT("count", count);
    _PUSH_ATTRIBUTES_INT("manualBreakCount", count);

    _xml_start_tag(self->file, "rowBreaks", &attributes);

    for (i = 0; i < count; i++)
        _worksheet_write_brk(self, self->hbreaks[i], LXW_COL_MAX - 1);

    _xml_end_tag(self->file, "rowBreaks");

    _FREE_ATTRIBUTES();
}

/*
 * Write the <colBreaks> element.
 */
STATIC void
_worksheet_write_col_breaks(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    uint16_t count = 0;
    uint16_t i;

    if (self->vbreaks == NULL)
        return;

    while (self->vbreaks[count])
        count++;

    if (count > LXW_BREAKS_MAX)
        count = LXW_BREAKS_MAX;

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_INT("count", count);
    _PUSH_ATTRIBUTES_INT("manualBreakCount", count);

    _xml_start_tag(self->file, "colBreaks", &attributes);

    for (i = 0; i < count; i++)
        _worksheet_write_brk(self, self->vbreaks[i], LXW_ROW_MAX - 1);

    _xml_end_tag(self->file, "colBreaks");

    _FREE_ATTRIBUTES();
}

/*
 * Write the <autoFilter> element.
 */
STATIC void
_worksheet_write_auto_filter(lxw_worksheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char range[MAX_CELL_RANGE_LENGTH];

    if (!self->autofilter.in_use)
        return;

    lxw_range(range,
              self->autofilter.first_row,
              self->autofilter.first_col,
              self->autofilter.last_row, self->autofilter.last_col);

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("ref", range);

    _xml_empty_tag(self->file, "autoFilter", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <hyperlink> element for external links.
 */
STATIC void
_worksheet_write_hyperlink_external(lxw_worksheet *self, lxw_row_t row_num,
                                    lxw_col_t col_num, const char *location,
                                    const char *tooltip, uint16_t id)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char ref[MAX_CELL_NAME_LENGTH];
    char r_id[MAX_ATTRIBUTE_LENGTH];

    lxw_rowcol_to_cell(ref, row_num, col_num);

    lxw_snprintf(r_id, ATTR_32, "rId%d", id);

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("ref", ref);
    _PUSH_ATTRIBUTES_STR("r:id", r_id);

    if (location)
        _PUSH_ATTRIBUTES_STR("location", location);

    if (tooltip)
        _PUSH_ATTRIBUTES_STR("tooltip", tooltip);

    _xml_empty_tag(self->file, "hyperlink", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <hyperlink> element for internal links.
 */
STATIC void
_worksheet_write_hyperlink_internal(lxw_worksheet *self, lxw_row_t row_num,
                                    lxw_col_t col_num, const char *location,
                                    const char *display, const char *tooltip)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char ref[MAX_CELL_NAME_LENGTH];

    lxw_rowcol_to_cell(ref, row_num, col_num);

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("ref", ref);

    if (location)
        _PUSH_ATTRIBUTES_STR("location", location);

    if (tooltip)
        _PUSH_ATTRIBUTES_STR("tooltip", tooltip);

    if (display)
        _PUSH_ATTRIBUTES_STR("display", display);

    _xml_empty_tag(self->file, "hyperlink", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Process any stored hyperlinks in row/col order and write the <hyperlinks>
 * element. The attributes are different for internal and external links.
 */
STATIC void
_worksheet_write_hyperlinks(lxw_worksheet *self)
{

    lxw_row *row;
    lxw_cell *link;
    lxw_rel_tuple *relationship;

    if (RB_EMPTY(self->hyperlinks))
        return;

    /* Write the hyperlink elements. */
    _xml_start_tag(self->file, "hyperlinks", NULL);

    RB_FOREACH(row, lxw_table_rows, self->hyperlinks) {

        RB_FOREACH(link, lxw_table_cells, row->cells) {

            if (link->type == HYPERLINK_URL
                || link->type == HYPERLINK_EXTERNAL) {

                self->rel_count++;

                relationship = calloc(1, sizeof(lxw_rel_tuple));
                GOTO_LABEL_ON_MEM_ERROR(relationship, mem_error);

                relationship->type = lxw_strdup("/hyperlink");
                GOTO_LABEL_ON_MEM_ERROR(relationship->type, mem_error);

                relationship->target = lxw_strdup(link->u.string);
                GOTO_LABEL_ON_MEM_ERROR(relationship->target, mem_error);

                relationship->target_mode = lxw_strdup("External");
                GOTO_LABEL_ON_MEM_ERROR(relationship->target_mode, mem_error);

                STAILQ_INSERT_TAIL(self->external_hyperlinks, relationship,
                                   list_pointers);

                _worksheet_write_hyperlink_external(self, link->row_num,
                                                    link->col_num,
                                                    link->user_data1,
                                                    link->user_data2,
                                                    self->rel_count);
            }

            if (link->type == HYPERLINK_INTERNAL) {

                _worksheet_write_hyperlink_internal(self, link->row_num,
                                                    link->col_num,
                                                    link->u.string,
                                                    link->user_data1,
                                                    link->user_data2);
            }

        }

    }

    _xml_end_tag(self->file, "hyperlinks");
    return;

mem_error:
    if (relationship) {
        free(relationship->type);
        free(relationship->target);
        free(relationship->target_mode);
        free(relationship);
    }
    _xml_end_tag(self->file, "hyperlinks");
}

/*
 * Assemble and write the XML file.
 */
void
_worksheet_assemble_xml_file(lxw_worksheet *self)
{
    /* Write the XML declaration. */
    _worksheet_xml_declaration(self);

    /* Write the worksheet element. */
    _worksheet_write_worksheet(self);

    /* Write the worksheet properties. */
    _worksheet_write_sheet_pr(self);

    /* Write the worksheet dimensions. */
    _worksheet_write_dimension(self);

    /* Write the sheet view properties. */
    _worksheet_write_sheet_views(self);

    /* Write the sheet format properties. */
    _worksheet_write_sheet_format_pr(self);

    /* Write the sheet column info. */
    _worksheet_write_cols(self);

    /* Write the sheetData element. */
    if (!self->optimize)
        _worksheet_write_sheet_data(self);
    else
        _worksheet_write_optimized_sheet_data(self);

    /* Write the autoFilter element. */
    _worksheet_write_auto_filter(self);

    /* Write the mergeCells element. */
    _worksheet_write_merge_cells(self);

    /* Write the hyperlink element. */
    _worksheet_write_hyperlinks(self);

    /* Write the printOptions element. */
    _worksheet_write_print_options(self);

    /* Write the worksheet page_margins. */
    _worksheet_write_page_margins(self);

    /* Write the worksheet page setup. */
    _worksheet_write_page_setup(self);

    /* Write the headerFooter element. */
    _worksheet_write_header_footer(self);

    /* Write the rowBreaks element. */
    _worksheet_write_row_breaks(self);

    /* Write the colBreaks element. */
    _worksheet_write_col_breaks(self);

    /* Close the worksheet tag. */
    _xml_end_tag(self->file, "worksheet");
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/

/*
 * Write a number to a cell in Excel.
 */
int8_t
worksheet_write_number(lxw_worksheet *self,
                       lxw_row_t row_num,
                       lxw_col_t col_num, double value, lxw_format *format)
{
    lxw_cell *cell;
    int8_t err;

    err = _check_dimensions(self, row_num, col_num, LXW_FALSE, LXW_FALSE);

    if (err)
        return err;

    cell = _new_number_cell(row_num, col_num, value, format);

    _insert_cell(self, row_num, col_num, cell);

    return 0;
}

/*
 * Write a string to an Excel file.
 */
int8_t
worksheet_write_string(lxw_worksheet *self,
                       lxw_row_t row_num,
                       lxw_col_t col_num, const char *string,
                       lxw_format *format)
{
    lxw_cell *cell;
    int32_t string_id;
    char *string_copy;
    int8_t err;

    if (!string || !strlen(string)) {
        /* Treat a NULL or empty string with formatting as a blank cell. */
        /* Null strings without formats should be ignored.      */
        if (format)
            return worksheet_write_blank(self, row_num, col_num, format);
        else
            return -LXW_ERROR_WORKSHEET_NULL_STRING_IGNORED;
    }

    err = _check_dimensions(self, row_num, col_num, LXW_FALSE, LXW_FALSE);
    if (err)
        return err;

    if (strlen(string) > LXW_STR_MAX)
        return -LXW_ERROR_WORKSHEET_MAX_STRING_LENGTH_EXCEEDED;

    if (!self->optimize) {
        /* Get the SST string ID for the string. */
        string_id = _get_sst_index(self->sst, string);

        if (string_id < 0)
            return -LXW_ERROR_WORKSHEET_STRING_HASH_NOT_FOUND;

        cell = _new_string_cell(row_num, col_num, string_id, format);
    }
    else {
        string_copy = lxw_strdup(string);
        cell = _new_inline_string_cell(row_num, col_num, string_copy, format);
    }

    _insert_cell(self, row_num, col_num, cell);

    return 0;
}

/*
 * Write a formula with a numerical result to a cell in Excel.
 */
int8_t
worksheet_write_formula_num(lxw_worksheet *self,
                            lxw_row_t row_num,
                            lxw_col_t col_num,
                            const char *formula,
                            lxw_format *format, double result)
{
    lxw_cell *cell;
    char *formula_copy;
    int8_t err;

    if (!formula)
        return -LXW_ERROR_WORKSHEET_NULL_STRING_IGNORED;

    err = _check_dimensions(self, row_num, col_num, LXW_FALSE, LXW_FALSE);
    if (err)
        return err;

    /* Strip leading "=" from formula. */
    if (formula[0] == '=')
        formula_copy = lxw_strdup(formula + 1);
    else
        formula_copy = lxw_strdup(formula);

    cell = _new_formula_cell(row_num, col_num, formula_copy, format);
    cell->formula_result = result;

    _insert_cell(self, row_num, col_num, cell);

    return 0;
}

/*
 * Write a formula with a default result to a cell in Excel .
 */
int8_t
worksheet_write_formula(lxw_worksheet *self,
                        lxw_row_t row_num,
                        lxw_col_t col_num, const char *formula,
                        lxw_format *format)
{
    return worksheet_write_formula_num(self, row_num, col_num, formula,
                                       format, 0);
}

/*
 * Write a formula with a numerical result to a cell in Excel.
 */
int8_t
worksheet_write_array_formula_num(lxw_worksheet *self,
                                  lxw_row_t first_row,
                                  lxw_col_t first_col,
                                  lxw_row_t last_row,
                                  lxw_col_t last_col,
                                  const char *formula,
                                  lxw_format *format, double result)
{
    lxw_cell *cell;
    lxw_row_t tmp_row;
    lxw_col_t tmp_col;
    char *formula_copy;
    char *range;
    int8_t err;

    /* Swap last row/col with first row/col as necessary */
    if (first_row > last_row) {
        tmp_row = last_row;
        last_row = first_row;
        first_row = tmp_row;
    }
    if (first_col > last_col) {
        tmp_col = last_col;
        last_col = first_col;
        first_col = tmp_col;
    }

    if (!formula)
        return -LXW_ERROR_WORKSHEET_NULL_STRING_IGNORED;

    /* Check that column number is valid and store the max value */
    err = _check_dimensions(self, last_row, last_col, LXW_FALSE, LXW_FALSE);
    if (err)
        return err;

    /* Define the array range. */
    range = calloc(1, MAX_CELL_RANGE_LENGTH);
    RETURN_ON_MEM_ERROR(range, -1);

    if (first_row == last_row && first_col == last_col)
        lxw_rowcol_to_cell(range, first_row, last_col);
    else
        lxw_range(range, first_row, first_col, last_row, last_col);

    /* Copy and trip leading "{=" from formula. */
    if (formula[0] == '{')
        if (formula[1] == '=')
            formula_copy = lxw_strdup(formula + 2);
        else
            formula_copy = lxw_strdup(formula + 1);
    else
        formula_copy = lxw_strdup(formula);

    /* Strip trailing "}" from formula. */
    if (formula_copy[strlen(formula_copy) - 1] == '}')
        formula_copy[strlen(formula_copy) - 1] = '\0';

    /* Create a new array formula cell object. */
    cell = _new_array_formula_cell(first_row, first_col,
                                   formula_copy, range, format);

    cell->formula_result = result;

    _insert_cell(self, first_row, first_col, cell);

    /* Pad out the rest of the area with formatted zeroes. */
    if (!self->optimize) {
        for (tmp_row = first_row; tmp_row <= last_row; tmp_row++) {
            for (tmp_col = first_col; tmp_col <= last_col; tmp_col++) {
                if (tmp_row == first_row && tmp_col == first_col)
                    continue;

                worksheet_write_number(self, tmp_row, tmp_col, 0, format);
            }
        }
    }

    return 0;
}

/*
 * Write an array formula with a default result to a cell in Excel .
 */
int8_t
worksheet_write_array_formula(lxw_worksheet *self,
                              lxw_row_t first_row,
                              lxw_col_t first_col,
                              lxw_row_t last_row,
                              lxw_col_t last_col,
                              const char *formula, lxw_format *format)
{
    return worksheet_write_array_formula_num(self, first_row, first_col,
                                             last_row, last_col, formula,
                                             format, 0);
}

/*
 * Write a blank cell with a format to a cell in Excel.
 */
int8_t
worksheet_write_blank(lxw_worksheet *self,
                      lxw_row_t row_num, lxw_col_t col_num,
                      lxw_format *format)
{
    lxw_cell *cell;
    int8_t err;

    /* Blank cells without formatting are ignored by Excel. */
    if (!format)
        return 0;

    err = _check_dimensions(self, row_num, col_num, LXW_FALSE, LXW_FALSE);

    if (err)
        return err;

    cell = _new_blank_cell(row_num, col_num, format);

    _insert_cell(self, row_num, col_num, cell);

    return 0;
}

/*
 * Write a date and or time to a cell in Excel.
 */
int8_t
worksheet_write_datetime(lxw_worksheet *self,
                         lxw_row_t row_num,
                         lxw_col_t col_num, lxw_datetime *datetime,
                         lxw_format *format)
{
    lxw_cell *cell;
    double excel_date;
    int8_t err;

    err = _check_dimensions(self, row_num, col_num, LXW_FALSE, LXW_FALSE);

    if (err)
        return err;

    excel_date = _datetime_to_excel_date(datetime, EPOCH_1900);

    cell = _new_number_cell(row_num, col_num, excel_date, format);

    _insert_cell(self, row_num, col_num, cell);

    return 0;
}

/*
 * Write a hyperlink/url to an Excel file.
 */
int8_t
worksheet_write_url_opt(lxw_worksheet *self,
                        lxw_row_t row_num,
                        lxw_col_t col_num, const char *url,
                        lxw_format *format, const char *string,
                        const char *tooltip)
{
    lxw_cell *link;
    char *string_copy = NULL;
    char *url_copy = NULL;
    char *url_external = NULL;
    char *url_string = NULL;
    char *tooltip_copy = NULL;
    char *found_string;
    int8_t err;
    size_t string_size;
    size_t i;
    enum cell_types link_type = HYPERLINK_URL;

    if (!url || !strlen(url))
        return -LXW_ERROR_WORKSHEET_NULL_STRING_IGNORED;

    /* Check the Excel limit of URLS per worksheet. */
    if (self->hlink_count > LXW_MAX_NUMBER_URLS)
        return -LXW_ERROR_WORKSHEET_MAX_NUMBER_URLS_EXCEEDED;

    err = _check_dimensions(self, row_num, col_num, LXW_FALSE, LXW_FALSE);
    if (err)
        return err;

    /* Set the URI scheme from internal links. */
    found_string = strstr(url, "internal:");
    if (found_string)
        link_type = HYPERLINK_INTERNAL;

    /* Set the URI scheme from external links. */
    found_string = strstr(url, "external:");
    if (found_string)
        link_type = HYPERLINK_EXTERNAL;

    if (string) {
        string_copy = lxw_strdup(string);
        GOTO_LABEL_ON_MEM_ERROR(string_copy, mem_error);
    }
    else {
        if (link_type == HYPERLINK_URL) {
            /* Strip the mailto header. */
            found_string = strstr(url, "mailto:");
            if (found_string)
                string_copy = lxw_strdup(url + sizeof("mailto"));
            else
                string_copy = lxw_strdup(url);
        }
        else {
            string_copy = lxw_strdup(url + sizeof("__ternal"));
        }
        GOTO_LABEL_ON_MEM_ERROR(string_copy, mem_error);
    }

    if (url) {
        if (link_type == HYPERLINK_URL)
            url_copy = lxw_strdup(url);
        else
            url_copy = lxw_strdup(url + sizeof("__ternal"));

        GOTO_LABEL_ON_MEM_ERROR(url_copy, mem_error);
    }

    if (tooltip) {
        tooltip_copy = lxw_strdup(tooltip);
        GOTO_LABEL_ON_MEM_ERROR(tooltip_copy, mem_error);
    }

    if (link_type == HYPERLINK_INTERNAL) {
        url_string = lxw_strdup(string_copy);
        GOTO_LABEL_ON_MEM_ERROR(url_string, mem_error);
    }

    /* Escape the URL. */
    if (link_type == HYPERLINK_URL && strlen(url_copy) >= 3) {
        uint8_t not_escaped = 1;

        /* First check if the URL is already escaped by the user. */
        for (i = 0; i <= strlen(url_copy) - 3; i++) {
            if (url_copy[i] == '%' && isxdigit(url_copy[i + 1])
                && isxdigit(url_copy[i + 2])) {

                not_escaped = 0;
                break;
            }
        }

        if (not_escaped) {
            url_external = calloc(1, strlen(url_copy) * 3 + 1);
            GOTO_LABEL_ON_MEM_ERROR(url_external, mem_error);

            for (i = 0; i <= strlen(url_copy); i++) {
                switch (url_copy[i]) {
                    case (' '):
                    case ('"'):
                    case ('%'):
                    case ('<'):
                    case ('>'):
                    case ('['):
                    case (']'):
                    case ('`'):
                    case ('^'):
                    case ('{'):
                    case ('}'):
                        sprintf(url_external + strlen(url_external), "%%%2x",
                                url_copy[i]);
                        break;
                    default:
                        url_external[strlen(url_external)] = url_copy[i];
                }

            }

            free(url_copy);
            url_copy = lxw_strdup(url_external);
            GOTO_LABEL_ON_MEM_ERROR(url_copy, mem_error);

            free(url_external);
            url_external = NULL;
        }
    }

    if (link_type == HYPERLINK_EXTERNAL) {
        /* External Workbook links need to be modified into the right format.
         * The URL will look something like "c:\temp\file.xlsx#Sheet!A1".
         * We need the part to the left of the # as the URL and the part to
         * the right as the "location" string (if it exists).
         */

        /* For external links change the dir separator from Unix to DOS. */
        for (i = 0; i <= strlen(url_copy); i++)
            if (url_copy[i] == '/')
                url_copy[i] = '\\';

        for (i = 0; i <= strlen(string_copy); i++)
            if (string_copy[i] == '/')
                string_copy[i] = '\\';

        found_string = strchr(url_copy, '#');

        if (found_string) {
            url_string = lxw_strdup(found_string + 1);
            GOTO_LABEL_ON_MEM_ERROR(url_string, mem_error);

            *found_string = '\0';
        }

        /* Look for Windows style "C:/" link or Windows share "\\" link. */
        found_string = strchr(url_copy, ':');
        if (!found_string)
            found_string = strstr(url_copy, "\\\\");

        if (found_string) {
            /* Add the file:/// URI to the url if non-local. */
            string_size = sizeof("file:///") + strlen(url_copy);
            url_external = calloc(1, string_size);
            GOTO_LABEL_ON_MEM_ERROR(url_external, mem_error);

            lxw_snprintf(url_external, string_size, "file:///%s", url_copy);

        }

        /* Convert a ./dir/file.xlsx link to dir/file.xlsx. */
        found_string = strstr(url_copy, ".\\");
        if (found_string == url_copy)
            memmove(url_copy, url_copy + 2, strlen(url_copy) - 1);

        if (url_external) {
            free(url_copy);
            url_copy = lxw_strdup(url_external);
            GOTO_LABEL_ON_MEM_ERROR(url_copy, mem_error);

            free(url_external);
            url_external = NULL;
        }

    }

    /* Excel limits escaped URL to 255 characters. */
    if (strlen(url_copy) > 255)
        goto mem_error;

    err = worksheet_write_string(self, row_num, col_num, string_copy, format);
    if (err)
        goto mem_error;

    link = _new_hyperlink_cell(row_num, col_num, link_type, url_copy,
                               url_string, tooltip_copy);
    GOTO_LABEL_ON_MEM_ERROR(link, mem_error);

    _insert_hyperlink(self, row_num, col_num, link);

    free(string_copy);
    self->hlink_count++;
    return 0;

mem_error:
    free(string_copy);
    free(url_copy);
    free(url_external);
    free(url_string);
    free(tooltip_copy);
    return -LXW_ERROR_WORKSHEET_MEMORY_ERROR;
}

/*
 * Write a hyperlink/url to an Excel file.
 */
int8_t
worksheet_write_url(lxw_worksheet *self,
                    lxw_row_t row_num,
                    lxw_col_t col_num, const char *url, lxw_format *format)
{
    return worksheet_write_url_opt(self, row_num, col_num, url, format, NULL,
                                   NULL);
}

/*
 * Set the properties of a single column or a range of columns.
 */
int8_t
worksheet_set_column(lxw_worksheet *self,
                     lxw_col_t firstcol,
                     lxw_col_t lastcol,
                     double width,
                     lxw_format *format, lxw_row_col_options *user_options)
{
    lxw_col_options *copied_options;
    uint8_t ignore_row = LXW_TRUE;
    uint8_t ignore_col = LXW_TRUE;
    uint8_t hidden = LXW_FALSE;
    uint8_t level = 0;
    uint8_t collapsed = LXW_FALSE;
    lxw_col_t col;
    int8_t err;

    if (user_options) {
        hidden = user_options->hidden;
        level = user_options->level;
        collapsed = user_options->collapsed;
    }

    /* Ensure second col is larger than first. */
    if (firstcol > lastcol) {
        lxw_col_t tmp = firstcol;
        firstcol = lastcol;
        lastcol = tmp;
    }

    /* Ensure that the cols are valid and store max and min values.
     * NOTE: The check shouldn't modify the row dimensions and should only
     *       modify the column dimensions in certain cases. */
    if (format != NULL || (width != LXW_DEF_COL_WIDTH && hidden))
        ignore_col = LXW_FALSE;

    err = _check_dimensions(self, 0, firstcol, ignore_row, ignore_col);

    if (!err)
        err = _check_dimensions(self, 0, lastcol, ignore_row, ignore_col);

    if (err)
        return err;

    /* Resize the col_options array if required. */
    if (firstcol >= self->col_options_max) {
        lxw_col_t col;
        lxw_col_t old_size = self->col_options_max;
        lxw_col_t new_size = _next_power_of_two(firstcol + 1);
        lxw_col_options **new_ptr = realloc(self->col_options,
                                            new_size *
                                            sizeof(lxw_col_options *));

        if (new_ptr) {
            for (col = old_size; col < new_size; col++)
                new_ptr[col] = NULL;

            self->col_options = new_ptr;
            self->col_options_max = new_size;
        }
        else {
            return -LXW_ERROR_WORKSHEET_MEMORY_ERROR;
        }
    }

    /* Resize the col_formats array if required. */
    if (lastcol >= self->col_formats_max) {
        lxw_col_t col;
        lxw_col_t old_size = self->col_formats_max;
        lxw_col_t new_size = _next_power_of_two(lastcol + 1);
        lxw_format **new_ptr = realloc(self->col_formats,
                                       new_size * sizeof(lxw_format *));

        if (new_ptr) {
            for (col = old_size; col < new_size; col++)
                new_ptr[col] = NULL;

            self->col_formats = new_ptr;
            self->col_formats_max = new_size;
        }
        else {
            return -LXW_ERROR_WORKSHEET_MEMORY_ERROR;
        }
    }

    copied_options = calloc(1, sizeof(lxw_col_options));
    RETURN_ON_MEM_ERROR(copied_options, -LXW_ERROR_WORKSHEET_MEMORY_ERROR);

    /* Store the column option based on the first column. */
    copied_options->firstcol = firstcol;
    copied_options->lastcol = lastcol;
    copied_options->width = width;
    copied_options->format = format;
    copied_options->hidden = hidden;
    copied_options->level = level;
    copied_options->collapsed = collapsed;
    self->col_options[firstcol] = copied_options;

    /* Store the column formats for use when writing cell data. */
    for (col = firstcol; col <= lastcol; col++) {
        self->col_formats[col] = format;
    }

    /* Store the column change to allow optimisations. */
    self->col_size_changed = LXW_TRUE;

    return 0;
}

/*
 * Set the properties of a row.
 */
int8_t
worksheet_set_row(lxw_worksheet *self,
                  lxw_row_t row_num,
                  double height,
                  lxw_format *format, lxw_row_col_options *user_options)
{

    lxw_col_t min_col;
    uint8_t hidden = LXW_FALSE;
    uint8_t level = 0;
    uint8_t collapsed = LXW_FALSE;
    lxw_row *row;
    int8_t err;

    if (user_options) {
        hidden = user_options->hidden;
        level = user_options->level;
        collapsed = user_options->collapsed;
    }

    /* Use minimum col in _check_dimensions(). */
    if (self->dim_colmin != LXW_COL_MAX)
        min_col = self->dim_colmin;
    else
        min_col = 0;

    err = _check_dimensions(self, row_num, min_col, LXW_FALSE, LXW_FALSE);

    if (err)
        return err;

    /* If the height is 0 the row is hidden and the height is the default. */
    if (height == 0) {
        hidden = LXW_TRUE;
        height = LXW_DEF_ROW_HEIGHT;

    }

    row = _get_row(self, row_num);

    row->height = height;
    row->format = format;
    row->hidden = hidden;
    row->level = level;
    row->collapsed = collapsed;
    row->row_changed = LXW_TRUE;

    return 0;

}

/*
 * Merge a range of cells. The first cell should contain the data and the others
 * should be blank. All cells should contain the same format.
 */
uint8_t
worksheet_merge_range(lxw_worksheet *self, lxw_row_t first_row,
                      lxw_col_t first_col, lxw_row_t last_row,
                      lxw_col_t last_col, const char *string,
                      lxw_format *format)
{
    lxw_merged_range *merged_range;
    lxw_row_t tmp_row;
    lxw_col_t tmp_col;
    int8_t err;

    /* Excel doesn't allow a single cell to be merged */
    if (first_row == last_row && first_col == last_col)
        return 1;

    /* Swap last row/col with first row/col as necessary */
    if (first_row > last_row) {
        tmp_row = last_row;
        last_row = first_row;
        first_row = tmp_row;
    }
    if (first_col > last_col) {
        tmp_col = last_col;
        last_col = first_col;
        first_col = tmp_col;
    }

    /* Check that column number is valid and store the max value */
    err = _check_dimensions(self, last_row, last_col, LXW_FALSE, LXW_FALSE);

    if (err)
        return err;

    /* Store the merge range. */
    merged_range = calloc(1, sizeof(lxw_merged_range));
    RETURN_ON_MEM_ERROR(merged_range, 2);

    merged_range->first_row = first_row;
    merged_range->first_col = first_col;
    merged_range->last_row = last_row;
    merged_range->last_col = last_col;

    STAILQ_INSERT_TAIL(self->merged_ranges, merged_range, list_pointers);
    self->merged_range_count++;

    /* Write the first cell */
    worksheet_write_string(self, first_row, first_col, string, format);

    /* Pad out the rest of the area with formatted blank cells. */
    for (tmp_row = first_row; tmp_row <= last_row; tmp_row++) {
        for (tmp_col = first_col; tmp_col <= last_col; tmp_col++) {
            if (tmp_row == first_row && tmp_col == first_col)
                continue;

            worksheet_write_blank(self, tmp_row, tmp_col, format);
        }
    }

    return 0;
}

/*
 * Set the autofilter area in the worksheet.
 */
uint8_t
worksheet_autofilter(lxw_worksheet *self, lxw_row_t first_row,
                     lxw_col_t first_col, lxw_row_t last_row,
                     lxw_col_t last_col)
{
    lxw_row_t tmp_row;
    lxw_col_t tmp_col;
    int8_t err;

    /* Excel doesn't allow a single cell to be merged */
    if (first_row == last_row && first_col == last_col)
        return 1;

    /* Swap last row/col with first row/col as necessary */
    if (first_row > last_row) {
        tmp_row = last_row;
        last_row = first_row;
        first_row = tmp_row;
    }
    if (first_col > last_col) {
        tmp_col = last_col;
        last_col = first_col;
        first_col = tmp_col;
    }

    /* Check that column number is valid and store the max value */
    err = _check_dimensions(self, last_row, last_col, LXW_FALSE, LXW_FALSE);

    if (err)
        return err;

    self->autofilter.in_use = LXW_TRUE;
    self->autofilter.first_row = first_row;
    self->autofilter.first_col = first_col;
    self->autofilter.last_row = last_row;
    self->autofilter.last_col = last_col;

    return 0;
}

/*
 * Set this worksheet as a selected worksheet, i.e. the worksheet has its tab
 * highlighted.
 */
void
worksheet_select(lxw_worksheet *self)
{
    self->selected = LXW_TRUE;

    /* Selected worksheet can"t be hidden. */
    self->hidden = LXW_FALSE;
}

/*
 * Set this worksheet as the active worksheet, i.e. the worksheet that is
 * displayed when the workbook is opened. Also set it as selected.
 */
void
worksheet_activate(lxw_worksheet *self)
{
    self->selected = LXW_TRUE;
    self->active = LXW_TRUE;

    /* Active worksheet can"t be hidden. */
    self->hidden = LXW_FALSE;

    *self->active_sheet = self->index;
}

/*
 * Set panes and mark them as frozen. With extra options.
 */
void
worksheet_freeze_panes_opt(lxw_worksheet *self,
                           lxw_row_t first_row, lxw_col_t first_col,
                           lxw_row_t top_row, lxw_col_t left_col,
                           uint8_t type)
{
    self->panes.first_row = first_row;
    self->panes.first_col = first_col;
    self->panes.top_row = top_row;
    self->panes.left_col = left_col;
    self->panes.x_split = 0.0;
    self->panes.y_split = 0.0;

    if (type)
        self->panes.type = FREEZE_SPLIT_PANES;
    else
        self->panes.type = FREEZE_PANES;
}

/*
 * Set panes and mark them as frozen.
 */
void
worksheet_freeze_panes(lxw_worksheet *self,
                       lxw_row_t first_row, lxw_col_t first_col)
{
    worksheet_freeze_panes_opt(self, first_row, first_col,
                               first_row, first_col, 0);
}

/*
 * Set panes and mark them as split.With extra options.
 */
void
worksheet_split_panes_opt(lxw_worksheet *self,
                          double y_split, double x_split,
                          lxw_row_t top_row, lxw_col_t left_col)
{
    self->panes.first_row = 0;
    self->panes.first_col = 0;
    self->panes.top_row = top_row;
    self->panes.left_col = left_col;
    self->panes.x_split = x_split;
    self->panes.y_split = y_split;
    self->panes.type = SPLIT_PANES;
}

/*
 * Set panes and mark them as split.
 */
void
worksheet_split_panes(lxw_worksheet *self, double y_split, double x_split)
{
    worksheet_split_panes_opt(self, y_split, x_split, 0, 0);
}

/*
 * Set the page orientation as portrait.
 */
void
worksheet_set_portrait(lxw_worksheet *self)
{
    self->orientation = LXW_PORTRAIT;
    self->page_setup_changed = LXW_TRUE;
}

/*
 * Set the page orientation as landscape.
 */
void
worksheet_set_landscape(lxw_worksheet *self)
{
    self->orientation = LXW_LANDSCAPE;
    self->page_setup_changed = LXW_TRUE;
}

/*
 * Set the page view mode for Mac Excel.
 */
void
worksheet_set_page_view(lxw_worksheet *self)
{
    self->page_view = LXW_TRUE;
}

/*
 * Set the paper type. Example. 1 = US Letter, 9 = A4
 */
void
worksheet_set_paper(lxw_worksheet *self, uint8_t paper_size)
{
    self->paper_size = paper_size;
    self->page_setup_changed = LXW_TRUE;
}

/*
 * Set the order in which pages are printed.
 */
void
worksheet_print_across(lxw_worksheet *self)
{
    self->page_order = LXW_PRINT_ACROSS;
    self->page_setup_changed = LXW_TRUE;
}

/*
 * Set all the page margins in inches.
 */
void
worksheet_set_margins(lxw_worksheet *self, double left, double right,
                      double top, double bottom)
{

    if (left >= 0)
        self->margin_left = left;

    if (right >= 0)
        self->margin_right = right;

    if (top >= 0)
        self->margin_top = top;

    if (bottom >= 0)
        self->margin_bottom = bottom;
}

/*
 * Set the page header caption and options.
 */
uint8_t
worksheet_set_header_opt(lxw_worksheet *self, char *string,
                         lxw_header_footer_options *options)
{
    if (options) {
        if (options->margin > 0)
            self->margin_header = options->margin;
    }

    if (!string)
        return 1;

    if (strlen(string) >= LXW_HEADER_FOOTER_MAX)
        return 1;

    strcpy(self->header, string);
    self->header_footer_changed = 1;

    return 0;
}

/*
 * Set the page footer caption and options.
 */
uint8_t
worksheet_set_footer_opt(lxw_worksheet *self, char *string,
                         lxw_header_footer_options *options)
{
    if (options) {
        if (options->margin > 0)
            self->margin_footer = options->margin;
    }

    if (!string)
        return 1;

    if (strlen(string) >= LXW_HEADER_FOOTER_MAX)
        return 1;

    strcpy(self->footer, string);
    self->header_footer_changed = 1;

    return 0;
}

/*
 * Set the page header caption.
 */
uint8_t
worksheet_set_header(lxw_worksheet *self, char *string)
{
    return worksheet_set_header_opt(self, string, NULL);
}

/*
 * Set the page footer caption.
 */
uint8_t
worksheet_set_footer(lxw_worksheet *self, char *string)
{
    return worksheet_set_footer_opt(self, string, NULL);
}

/*
 * Set the option to show/hide gridlines on the screen and the printed page.
 */
void
worksheet_gridlines(lxw_worksheet *self, uint8_t option)
{
    if (option == LXW_HIDE_ALL_GRIDLINES) {
        self->print_gridlines = 0;
        self->screen_gridlines = 0;
    }

    if (option & LXW_SHOW_SCREEN_GRIDLINES) {
        self->screen_gridlines = 1;
    }

    if (option & LXW_SHOW_PRINT_GRIDLINES) {
        self->print_gridlines = 1;
        self->print_options_changed = 1;
    }
}

/*
 * Center the page horizontally.
 */
void
worksheet_center_horizontally(lxw_worksheet *self)
{
    self->print_options_changed = 1;
    self->hcenter = 1;
}

/*
 * Center the page horizontally.
 */
void
worksheet_center_vertically(lxw_worksheet *self)
{
    self->print_options_changed = 1;
    self->vcenter = 1;
}

/*
 * Set the option to print the row and column headers on the printed page.
 */
void
worksheet_print_row_col_headers(lxw_worksheet *self)
{
    self->print_headers = 1;
    self->print_options_changed = 1;
}

/*
 * Set the rows to repeat at the top of each printed page.
 */
uint8_t
worksheet_repeat_rows(lxw_worksheet *self, lxw_row_t first_row,
                      lxw_row_t last_row)
{
    lxw_row_t tmp_row;

    if (first_row > last_row) {
        tmp_row = last_row;
        last_row = first_row;
        first_row = tmp_row;
    }

    if (_check_dimensions(self, last_row, 0, LXW_IGNORE, LXW_IGNORE))
        return 1;

    self->repeat_rows.in_use = LXW_TRUE;
    self->repeat_rows.first_row = first_row;
    self->repeat_rows.last_row = last_row;

    return 0;
}

/*
 * Set the columns to repeat at the left hand side of each printed page.
 */
uint8_t
worksheet_repeat_columns(lxw_worksheet *self, lxw_col_t first_col,
                         lxw_col_t last_col)
{
    lxw_col_t tmp_col;

    if (first_col > last_col) {
        tmp_col = last_col;
        last_col = first_col;
        first_col = tmp_col;
    }

    if (_check_dimensions(self, last_col, 0, LXW_IGNORE, LXW_IGNORE))
        return 1;

    self->repeat_cols.in_use = LXW_TRUE;
    self->repeat_cols.first_col = first_col;
    self->repeat_cols.last_col = last_col;

    return 0;
}

/*
 * Set the print area in the current worksheet.
 */
uint8_t
worksheet_print_area(lxw_worksheet *self, lxw_row_t first_row,
                     lxw_col_t first_col, lxw_row_t last_row,
                     lxw_col_t last_col)
{
    lxw_row_t tmp_row;
    lxw_col_t tmp_col;

    if (first_row > last_row) {
        tmp_row = last_row;
        last_row = first_row;
        first_row = tmp_row;
    }

    if (first_col > last_col) {
        tmp_col = last_col;
        last_col = first_col;
        first_col = tmp_col;
    }

    if (_check_dimensions(self, last_row, last_col, LXW_IGNORE, LXW_IGNORE))
        return 1;

    /* Ignore max area since it is the same as no print area in Excel. */
    if (first_row == 0 && first_col == 0 && last_row == LXW_ROW_MAX - 1
        && last_col == LXW_COL_MAX - 1) {
        return 0;
    }

    self->print_area.in_use = LXW_TRUE;
    self->print_area.first_row = first_row;
    self->print_area.last_row = last_row;
    self->print_area.first_col = first_col;
    self->print_area.last_col = last_col;

    return 0;
}

/* Store the vertical and horizontal number of pages that will define the
 * maximum area printed.
 */
void
worksheet_fit_to_pages(lxw_worksheet *self, uint16_t width, uint16_t height)
{
    self->fit_page = 1;
    self->fit_width = width;
    self->fit_height = height;
    self->page_setup_changed = 1;
}

/*
 * Set the start page number.
 */
void
worksheet_set_start_page(lxw_worksheet *self, uint16_t start_page)
{
    self->page_start = start_page;
}

/*
 * Set the scale factor for the printed page.
 */
void
worksheet_set_print_scale(lxw_worksheet *self, uint16_t scale)
{
    /* Confine the scale to Excel"s range */
    if (scale < 10 || scale > 400)
        return;

    /* Turn off "fit to page" option. */
    self->fit_page = LXW_FALSE;

    self->print_scale = scale;
    self->page_setup_changed = LXW_TRUE;
}

/*
 * Store the horizontal page breaks on a worksheet.
 */
void
worksheet_set_h_pagebreaks(lxw_worksheet *self, lxw_row_t hbreaks[])
{
    self->hbreaks = hbreaks;
}

/*
 * Store the vertical page breaks on a worksheet.
 */
void
worksheet_set_v_pagebreaks(lxw_worksheet *self, lxw_col_t vbreaks[])
{
    self->vbreaks = vbreaks;
}
