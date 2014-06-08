/*****************************************************************************
 * worksheet - A library for creating Excel XLSX worksheet files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/worksheet.h"
#include "xlsxwriter/format.h"
#include "xlsxwriter/utility.h"

#define XL_ROW_MAX 1048576
#define XL_COL_MAX 16384
#define XL_STR_MAX 32767

/*
 * Forward declarations.
 */
STATIC void _write_rows(lxw_worksheet *self);

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

    TAILQ_INIT(worksheet->table);

    worksheet->file = NULL;
    worksheet->dim_rowmax = 0;
    worksheet->dim_colmax = 0;
    worksheet->dim_rowmin = XL_ROW_MAX;
    worksheet->dim_colmin = XL_COL_MAX;

    if (init_data) {
        worksheet->name = init_data->name;
        worksheet->index = init_data->index;
        worksheet->hidden = init_data->hidden;
        worksheet->sst = init_data->sst;
    }

    return worksheet;

mem_error:
    _free_worksheet(worksheet);
    return NULL;
}

/*
 * Free a worksheet object.
 */
void
_free_worksheet(lxw_worksheet *worksheet)
{
    lxw_row *row;
    lxw_cell *cell;

    if (!worksheet)
        return;

    if (worksheet->table) {

        while (!TAILQ_EMPTY(worksheet->table)) {
            row = TAILQ_FIRST(worksheet->table);

            while (!TAILQ_EMPTY(row->cells)) {
                cell = TAILQ_FIRST(row->cells);
                TAILQ_REMOVE(row->cells, cell, list_pointers);
                if (cell->type == FORMULA_CELL) {
                    free(cell->u.formula);
                    /* free(cell->formula_result.string); */
                }
                free(cell);
            }
            TAILQ_REMOVE(worksheet->table, row, list_pointers);
            free(row->cells);
            free(row);
        }

        free(worksheet->table);
    }

    free(worksheet->name);
    free(worksheet);
}

/*
 * Create a new worksheet row object.
 */
STATIC lxw_row *
_new_row(int32_t row_num)
{
    lxw_row *row = calloc(1, sizeof(lxw_row));

    if (row) {
        row->row_num = row_num;
        row->cells = calloc(1, sizeof(struct lxw_table_cells));

        if (row->cells)
            TAILQ_INIT(row->cells);
        else
            MEM_ERROR();
    }
    else {
        MEM_ERROR();
    }

    return row;
}

/*
 * Create a new worksheet number cell object.
 */
STATIC lxw_cell *
_new_number_cell(uint32_t row_num,
                 uint16_t col_num, double value, lxw_format *format)
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
_new_string_cell(uint32_t row_num,
                 uint16_t col_num, int32_t string_id, lxw_format *format)
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
 * Create a new worksheet formula cell object.
 */
STATIC lxw_cell *
_new_formula_cell(uint32_t row_num,
                  uint16_t col_num, char *formula, lxw_format *format)
{
    lxw_cell *cell = calloc(1, sizeof(lxw_cell));
    RETURN_ON_MEM_ERROR(cell, cell);

    cell->row_num = row_num;
    cell->col_num = col_num;
    cell->type = FORMULA_CELL;
    cell->format = format;
    cell->u.formula = formula;

    return cell;
}

/*
 * Create a new worksheet blank cell object.
 */
STATIC lxw_cell *
_new_blank_cell(uint32_t row_num, uint16_t col_num, lxw_format *format)
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
 * Get or create the row object for a given row number.
 */
STATIC lxw_row *
_get_row(struct lxw_table_rows *table, uint32_t row_num)
{
    lxw_row *new_row;
    lxw_row *first_row = TAILQ_FIRST(table);
    lxw_row *last_row = TAILQ_LAST(table, lxw_table_rows);
    lxw_row *current_row;

    /* If the data table has no rows add a new row and return it. */
    if (!first_row) {
        new_row = _new_row(row_num);
        TAILQ_INSERT_HEAD(table, new_row, list_pointers);
        return new_row;
    }

    /* If the row number is the last row then return it. */
    if (row_num == last_row->row_num) {
        return last_row;
    }

    /* If the row number is after the current last row we append */
    /* a new row and return it. */
    if (row_num > last_row->row_num) {
        new_row = _new_row(row_num);
        TAILQ_INSERT_TAIL(table, new_row, list_pointers);
        return new_row;
    }

    /* If the row number is the first row then return it. */
    if (row_num == first_row->row_num) {
        return first_row;
    }

    /* If the row number is before the current first row we prepend */
    /* a new row and return it. */
    if (row_num < first_row->row_num) {
        new_row = _new_row(row_num);
        TAILQ_INSERT_HEAD(table, new_row, list_pointers);
        return new_row;
    }

    /* Otherwise we iterate through the list of rows and either return */
    /* and existing row or insert an new one in sorted order. */
    TAILQ_FOREACH(current_row, table, list_pointers) {

        if (row_num == current_row->row_num) {
            return current_row;
        }

        if (row_num < TAILQ_NEXT(current_row, list_pointers)->row_num) {
            /* Note: there is always a non-NULL _NEXT() since we already */
            /* checked above that the row_num is less than _LAST(). */
            new_row = _new_row(row_num);
            TAILQ_INSERT_AFTER(table, current_row, new_row, list_pointers);
            return new_row;
        }
    }

    return NULL;
}

/*
 * Insert a cell object in the cell list of a row object.
 */
STATIC void
_insert_cell(struct lxw_table_cells *cell_list,
             lxw_cell *cell, uint16_t col_num)
{
    lxw_cell *first_cell = TAILQ_FIRST(cell_list);
    lxw_cell *last_cell = TAILQ_LAST(cell_list, lxw_table_cells);
    lxw_cell *current_cell;

    /* If the row has no cells add the new at the start of the cell list. */
    if (!first_cell) {
        TAILQ_INSERT_HEAD(cell_list, cell, list_pointers);
        return;
    }

    /* If it is the last cell then replace it and free existing cell. */
    if (col_num == last_cell->col_num) {
        TAILQ_INSERT_TAIL(cell_list, cell, list_pointers);
        TAILQ_REMOVE(cell_list, last_cell, list_pointers);
        free(last_cell);
        return;
    }

    /* Append the cell if the col number is after the last cell. */
    if (col_num > last_cell->col_num) {
        TAILQ_INSERT_TAIL(cell_list, cell, list_pointers);
        return;
    }

    /* If it is the first cell then replace it and free existing cell. */
    if (col_num == first_cell->col_num) {
        TAILQ_INSERT_HEAD(cell_list, cell, list_pointers);
        TAILQ_REMOVE(cell_list, first_cell, list_pointers);
        free(first_cell);
        return;
    }

    /* Prepend the cell if the col number is before the first cell. */
    if (col_num < first_cell->col_num) {
        TAILQ_INSERT_HEAD(cell_list, cell, list_pointers);
        return;
    }

    /* Otherwise we iterate through the list of cells and either replace */
    /* an existing cell or insert an new one in sorted order. */
    TAILQ_FOREACH(current_cell, cell_list, list_pointers) {

        if (col_num == current_cell->col_num) {
            TAILQ_INSERT_AFTER(cell_list, current_cell, cell, list_pointers);
            TAILQ_REMOVE(cell_list, current_cell, list_pointers);
            free(current_cell);
            return;
        }

        if (col_num < TAILQ_NEXT(current_cell, list_pointers)->col_num) {
            /* Note: there is always a non-NULL _NEXT() since we already */
            /* checked above that the col_num is less than _LAST(). */
            TAILQ_INSERT_AFTER(cell_list, current_cell, cell, list_pointers);
            return;
        }
    }

    return;
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

    /* Default range ref for empty worksheet. */
    char ref[MAX_CELL_RANGE_LENGTH] = "A1";

    /* For non-empty worksheet fill in the cell range. */
    if (!TAILQ_EMPTY(self->table)) {
        xl_range(ref,
                 self->dim_rowmin, self->dim_colmin,
                 self->dim_rowmax, self->dim_colmax);
    }

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("ref", ref);

    _xml_empty_tag(self->file, "dimension", &attributes);

    _FREE_ATTRIBUTES();
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

    if (self->selected)
        _PUSH_ATTRIBUTES_STR("tabSelected", "1");

    _PUSH_ATTRIBUTES_STR("workbookViewId", "0");

    _xml_empty_tag(self->file, "sheetView", &attributes);

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
    if (TAILQ_EMPTY(self->table)) {
        _xml_empty_tag(self->file, "sheetData", NULL);
    }
    else {
        _xml_start_tag(self->file, "sheetData", NULL);
        _write_rows(self);
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
    char left[] = "0.7";
    char right[] = "0.7";
    char top[] = "0.75";
    char bottom[] = "0.75";
    char header[] = "0.3";
    char footer[] = "0.3";

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("left", left);
    _PUSH_ATTRIBUTES_STR("right", right);
    _PUSH_ATTRIBUTES_STR("top", top);
    _PUSH_ATTRIBUTES_STR("bottom", bottom);
    _PUSH_ATTRIBUTES_STR("header", header);
    _PUSH_ATTRIBUTES_STR("footer", footer);

    _xml_empty_tag(self->file, "pageMargins", &attributes);

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

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_INT("r", row->row_num + 1);

    if (spans)
        _PUSH_ATTRIBUTES_STR("spans", spans);

    if (TAILQ_EMPTY(row->cells))
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

    __builtin_snprintf(data, ATTR_32, "%.16g", cell->u.number);

    _xml_data_element(self->file, "v", data, NULL);

}

/*
 * Write out a string worksheet cell.
 */
STATIC void
_write_string_cell(lxw_worksheet *self, lxw_cell *cell)
{
    char data[ATTR_32];

    __builtin_snprintf(data, ATTR_32, "%d", cell->u.string_id);

    _xml_data_element(self->file, "v", data, NULL);
}

/*
 * Write out a formula worksheet cell with a numeric result.
 */
STATIC void
_write_formula_num_cell(lxw_worksheet *self, lxw_cell *cell)
{
    char data[ATTR_32];

    __builtin_snprintf(data, ATTR_32, "%.16g", cell->formula_result.number);

    _xml_data_element(self->file, "f", cell->u.formula, NULL);
    _xml_data_element(self->file, "v", data, NULL);
}

/*
 *  Calculate the "spans" attribute of the <row> tag. This is an XLSX
 * optimisation and isn't strictly required. However, it makes comparing
 * files easier.
 *
 * The span is the same for each block of 16 rows.
 */
STATIC void
_calculate_spans(struct lxw_row *row, char *span, int32_t *block_num)
{
    uint16_t span_col_min = TAILQ_FIRST(row->cells)->col_num;
    uint16_t span_col_max = TAILQ_LAST(row->cells, lxw_table_cells)->col_num;
    uint16_t col_min;
    uint16_t col_max;
    *block_num = row->row_num / 16;

    row = TAILQ_NEXT(row, list_pointers);

    while (row && (int32_t) (row->row_num / 16) == *block_num) {

        if (!TAILQ_EMPTY(row->cells)) {
            col_min = TAILQ_FIRST(row->cells)->col_num;
            col_max = TAILQ_LAST(row->cells, lxw_table_cells)->col_num;

            if (col_min < span_col_min)
                span_col_min = col_min;

            if (col_max > span_col_max)
                span_col_max = col_max;
        }

        row = TAILQ_NEXT(row, list_pointers);
    }

    __builtin_snprintf(span, MAX_CELL_RANGE_LENGTH,
                       "%d:%d", span_col_min + 1, span_col_max + 1);
}

/*
 * Write out a generic worksheet cell.
 */
STATIC void
_write_cell(lxw_worksheet *self, lxw_cell *cell)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char range[MAX_CELL_NAME_LENGTH] = { 0 };

    xl_rowcol_to_cell_abs(range, cell->row_num, cell->col_num, 0, 0);

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("r", range);

    if (cell->format) {
        int32_t index = _get_xf_index(cell->format);
        if (index)
            _PUSH_ATTRIBUTES_INT("s", index);
    }

    if (cell->type == STRING_CELL)
        _PUSH_ATTRIBUTES_STR("t", "s");

    if (cell->type == NUMBER_CELL) {
        _xml_start_tag(self->file, "c", &attributes);
        _write_number_cell(self, cell);
        _xml_end_tag(self->file, "c");
    }
    else if (cell->type == STRING_CELL) {
        _xml_start_tag(self->file, "c", &attributes);
        _write_string_cell(self, cell);
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

    _FREE_ATTRIBUTES();
}

/*
 * Write out the worksheet data as a series of rows and cells.
 */
STATIC void
_write_rows(lxw_worksheet *self)
{
    lxw_row *row;
    lxw_cell *cell;
    int32_t block_num = -1;
    char spans[MAX_CELL_RANGE_LENGTH] = { 0 };

    TAILQ_FOREACH(row, self->table, list_pointers) {

        if ((int32_t) row->row_num / 16 > block_num)
            _calculate_spans(row, spans, &block_num);

        if (!TAILQ_EMPTY(row->cells)) {
            _write_row(self, row, spans);

            TAILQ_FOREACH(cell, row->cells, list_pointers) {
                _write_cell(self, cell);
            }
        }
        _xml_end_tag(self->file, "row");
    }
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
                  uint32_t row_num,
                  uint16_t col_num, int8_t ignore_row, int8_t ignore_col)
{
    if (row_num >= XL_ROW_MAX)
        return LXW_RANGE_ERROR;

    if (col_num >= XL_COL_MAX)
        return LXW_RANGE_ERROR;

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
 * Assemble and write the XML file.
 */
void
_worksheet_assemble_xml_file(lxw_worksheet *self)
{
    /* Write the XML declaration. */
    _worksheet_xml_declaration(self);

    /* Write the worksheet element. */
    _worksheet_write_worksheet(self);

    /* Write the worksheet dimensions. */
    _worksheet_write_dimension(self);

    /* Write the sheet view properties. */
    _worksheet_write_sheet_views(self);

    /* Write the sheet format properties. */
    _worksheet_write_sheet_format_pr(self);

    /* Write the sheetData element. */
    _worksheet_write_sheet_data(self);

    /* Write the worksheet page_margins. */
    _worksheet_write_page_margins(self);

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
worksheet_write_number(lxw_worksheet *worksheet,
                       uint32_t row_num,
                       uint16_t col_num, double value, lxw_format *format)
{
    lxw_row *row;
    lxw_cell *cell;
    int8_t err = _check_dimensions(worksheet, row_num, col_num, 0, 0);

    if (err)
        return err;

    row = _get_row(worksheet->table, row_num);
    cell = _new_number_cell(row_num, col_num, value, format);

    _insert_cell(row->cells, cell, col_num);

    return 0;
}

/*
 * Write a string to an Excel file.
 */
int8_t
worksheet_write_string(lxw_worksheet *worksheet,
                       uint32_t row_num,
                       uint16_t col_num, const char *string,
                       lxw_format *format)
{
    lxw_row *row;
    lxw_cell *cell;
    int32_t string_id;
    int8_t err = _check_dimensions(worksheet, row_num, col_num, 0, 0);

    if (err)
        return err;

    /* Treat a NULL string with formatting as a blank cell. */
    if (!string && format)
        return worksheet_write_blank(worksheet, row_num, col_num, format);

    /* Get the SST string ID for the string. */
    string_id = _get_sst_index(worksheet->sst, string);

    if (string_id < 0)
        return LXW_STRING_HASH_ERROR;

    if (strlen(string) > XL_STR_MAX)
        return LXW_STRING_LENGTH_ERROR;

    row = _get_row(worksheet->table, row_num);
    cell = _new_string_cell(row_num, col_num, string_id, format);

    _insert_cell(row->cells, cell, col_num);

    return 0;
}

/*
 * Write a formula with a numerical result to a cell in Excel.
 */
int8_t
worksheet_write_formula_num(lxw_worksheet *worksheet,
                            uint32_t row_num,
                            uint16_t col_num,
                            const char *formula,
                            lxw_format *format, double result)
{
    lxw_row *row;
    lxw_cell *cell;
    char *formula_copy;
    int8_t err = _check_dimensions(worksheet, row_num, col_num, 0, 0);

    if (err)
        return err;

    /* Strip leading "=" from formula. */
    if (formula[0] == '=')
        formula_copy = __builtin_strdup(formula + 1);
    else
        formula_copy = __builtin_strdup(formula);

    row = _get_row(worksheet->table, row_num);
    cell = _new_formula_cell(row_num, col_num, formula_copy, format);
    cell->formula_result.number = result;

    _insert_cell(row->cells, cell, col_num);

    return 0;
}

/*
 *Write a formula with a default result to a cell in Excel .

 */ int8_t
worksheet_write_formula(lxw_worksheet *worksheet,
                        uint32_t row_num,
                        uint16_t col_num, const char *formula,
                        lxw_format *format)
{
    return worksheet_write_formula_num(worksheet, row_num, col_num, formula,
                                       format, 0);
}

/*
 * Write a blank cell with a format to a cell in Excel.

 */
int8_t
worksheet_write_blank(lxw_worksheet *worksheet,
                      uint32_t row_num, uint16_t col_num, lxw_format *format)
{
    lxw_row *row;
    lxw_cell *cell;
    int8_t err;

    /* Blank cells without formatting are ignored by Excel. */
    if (!format)
        return 0;

    err = _check_dimensions(worksheet, row_num, col_num, 0, 0);

    if (err)
        return err;

    row = _get_row(worksheet->table, row_num);
    cell = _new_blank_cell(row_num, col_num, format);

    _insert_cell(row->cells, cell, col_num);

    return 0;
}

/*
 * Write a date and or time to a cell in Excel.
 */
int8_t
worksheet_write_datetime(lxw_worksheet *worksheet,
                         uint32_t row_num,
                         uint16_t col_num, lxw_datetime *datetime,
                         lxw_format *format)
{
    lxw_row *row;
    lxw_cell *cell;
    double excel_date;
    int8_t err = _check_dimensions(worksheet, row_num, col_num, 0, 0);

    if (err)
        return err;

    excel_date = _datetime_to_excel_date(datetime, EPOCH_1900);

    row = _get_row(worksheet->table, row_num);
    cell = _new_number_cell(row_num, col_num, excel_date, format);

    _insert_cell(row->cells, cell, col_num);

    return 0;
}
