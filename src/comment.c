/*****************************************************************************
 * comment - A library for creating Excel XLSX comment files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2017, Xyato, coszyx@gmail.com. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/comment.h"
#include "xlsxwriter/utility.h"

#include <ctype.h>

/*
 * Forward declarations.
 */
STATIC int _comment_row_cmp(lxw_comment_row *row1, lxw_comment_row *row2);
STATIC int _comment_cmp(lxw_comment_data *comment1, lxw_comment_data *comment2);

#ifndef __clang_analyzer__
LXW_RB_GENERATE_COMMENT_ROW(lxw_comment_row_rbt, lxw_comment_row, tree_pointers, _comment_row_cmp);
LXW_RB_GENERATE_COMMENT(lxw_comment_data_rbt, lxw_comment_data, tree_pointers, _comment_cmp);
#endif

/*
 * row1:element to find, row2: element stored in tree
 */
STATIC int
_comment_row_cmp(lxw_comment_row *row1, lxw_comment_row *row2)
{
    if (row1->row_num > row2->row_num)
        return 1;
    if (row1->row_num < row2->row_num)
        return -1;
    return 0;
}

STATIC int
_comment_cmp(lxw_comment_data *comment1, lxw_comment_data *comment2)
{
    if (comment1->col_num > comment2->col_num)
        return 1;
    if (comment1->col_num < comment2->col_num)
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
_comment_xml_declaration(lxw_comment *self)
{
    lxw_xml_declaration(self->file);
}

STATIC void
_write_comments(lxw_comment *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char xmlns[] = "http://schemas.openxmlformats.org/spreadsheetml/2006/main";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("xmlns", xmlns);

    lxw_xml_start_tag(self->file, "comments", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <sz> element.
 */
STATIC void
_write_sz(lxw_comment *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", 8);

    lxw_xml_empty_tag(self->file, "sz", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <color> element.
 */
STATIC void
_write_color(lxw_comment *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("indexed", 81);

    lxw_xml_empty_tag(self->file, "color", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <rFont> element.
 */
STATIC void
_write_r_font(lxw_comment *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", "Tahoma");

    lxw_xml_empty_tag(self->file, "rFont", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <family> element.
 */
STATIC void
_write_family(lxw_comment *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", 2);

    lxw_xml_empty_tag(self->file, "family", &attributes);

    LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_r_pr(lxw_comment *self)
{
    /*Write the <rPr> element.*/
    lxw_xml_start_tag(self->file, "rPr", NULL);

    /* Write the sz element.*/
    _write_sz(self);

    /* Write the color element.*/
    _write_color(self);

    /* Write the rFont element.*/
    _write_r_font(self);

    /* Write the family element.*/
    _write_family(self);

    lxw_xml_end_tag(self->file, "rPr");
}

/*
 * Write the text <t> element.
 */
STATIC void
_write_text_t(lxw_comment *self, char *text)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    lxw_xml_data_element(self->file, "t", text, NULL);

    LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_text_r(lxw_comment *self, char *text)
{
    lxw_xml_start_tag(self->file, "r", NULL);

    _write_r_pr(self);

    _write_text_t(self, text);
    
    lxw_xml_end_tag(self->file, "r");
}

STATIC void
_write_text(lxw_comment *self, char *text)
{
    lxw_xml_start_tag(self->file, "text", NULL);

    _write_text_r(self, text);

    lxw_xml_end_tag(self->file, "text");
}

STATIC void
_write_comment_data(lxw_comment *self, lxw_comment_data* comment_data)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char range[LXW_MAX_CELL_NAME_LENGTH] = { 0 };
    lxw_hash_element *hash_element;
    char* str_author;

    lxw_rowcol_to_cell(range, comment_data->row_num, comment_data->col_num);

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("ref", range);

    if (comment_data->author)
        str_author = comment_data->author;
    else
        str_author = "";
    
    hash_element = lxw_hash_key_exists(self->author_ids, str_author, strlen(str_author));
    if (hash_element)
        LXW_PUSH_ATTRIBUTES_INT("authorId", *(uint32_t *)hash_element->value);
    
    lxw_xml_start_tag(self->file, "comment", &attributes);

    _write_text(self, comment_data->text);

    lxw_xml_end_tag(self->file, "comment");

    LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_authors(lxw_comment *self)
{
    lxw_comment_row *comment_row;
    lxw_comment_data *comment_data;

    uint32_t author_count = 0;

    char *str_author;
    lxw_hash_element *hash_element;

    lxw_xml_start_tag(self->file, "authors", NULL);

    RB_FOREACH(comment_row, lxw_comment_row_rbt, self->comment_row_list) {
        RB_FOREACH(comment_data, lxw_comment_data_rbt, comment_row->comments) {
            if (comment_data->author)
                str_author = comment_data->author;
            else
                str_author = "";

            hash_element = lxw_hash_key_exists(self->author_ids, str_author, strlen(str_author));
            if (!hash_element) {
                uint32_t *author_index = calloc(1, sizeof(uint32_t));
                *author_index = author_count++;

                lxw_insert_hash_element(self->author_ids, str_author, author_index, strlen(str_author));

                lxw_xml_data_element(self->file, "author", str_author, NULL);
            }
        }
    }

    lxw_xml_end_tag(self->file, "authors");
}

STATIC void
_write_comment_list(lxw_comment *self)
{
    lxw_comment_row *comment_row;
    lxw_comment_data *comment_data;

    lxw_xml_start_tag(self->file, "commentList", NULL);

    RB_FOREACH(comment_row, lxw_comment_row_rbt, self->comment_row_list) {
        RB_FOREACH(comment_data, lxw_comment_data_rbt, comment_row->comments) {
            _write_comment_data(self, comment_data);
        }
    }

    lxw_xml_end_tag(self->file, "commentList");
}

/*****************************************************************************
 *
 * Private functions.
 *
 ****************************************************************************/
/*
 * Free a comment data
 */
STATIC void
_free_comment_data(lxw_comment_data *comment_data)
{
    if (!comment_data)
        return;

    free(comment_data->text);
    free(comment_data->author);
    free(comment_data->color);
    free(comment_data);
}

/*
 * Create a new comment data
 */
STATIC lxw_comment_data *
_new_comment_data(lxw_row_t row_num,
                  lxw_col_t col_num, char *text)
{
    lxw_comment_data *comment_data = calloc(1, sizeof(lxw_comment_data));
    if (comment_data) {
        comment_data->row_num = row_num;
        comment_data->col_num = col_num;
        comment_data->text = text;
    }

    return comment_data;
}

/*
 * Free a row comment.
 */
STATIC void
_free_comment_row(lxw_comment_row *comment_row)
{
    lxw_comment_data *comment_data;
    lxw_comment_data *next_comment_data;

    if (!comment_row)
        return;

    for (comment_data = RB_MIN(lxw_comment_data_rbt, comment_row->comments); comment_data; comment_data = next_comment_data) {
        next_comment_data = RB_NEXT(lxw_comment_data_rbt, comment_row->comments, comment_data);
        RB_REMOVE(lxw_comment_data_rbt, comment_row->comments, comment_data);
        _free_comment_data(comment_data);
    }

    free(comment_row->comments);
    free(comment_row);
}

/*
 * Create a new row comment object.
 */
STATIC lxw_comment_row *
_new_commment_row(lxw_row_t row_num)
{
    lxw_comment_row *comment_row = calloc(1, sizeof(lxw_comment_row));

    if (comment_row) {
        comment_row->row_num = row_num;
        comment_row->comments = calloc(1, sizeof(struct lxw_comment_data_rbt));

        if (comment_row->comments)
            RB_INIT(comment_row->comments);
        else {
            free(comment_row);
            comment_row = NULL;
            LXW_MEM_ERROR();
        }
    }
    else {
        LXW_MEM_ERROR();
    }

    return comment_row;
}

/*
 * Get or create the row comment object for a given row number.
 */
STATIC lxw_comment_row *
_get_comment_row(struct lxw_comment_row_rbt *comment_row_tree, lxw_row_t row_num)
{
    lxw_comment_row *new_comment_row;
    lxw_comment_row *existing_comment_row;

    new_comment_row = _new_commment_row(row_num);
    if (new_comment_row) {
        existing_comment_row = RB_INSERT(lxw_comment_row_rbt, comment_row_tree, new_comment_row);

        if (existing_comment_row) {
            _free_comment_row(new_comment_row);
            new_comment_row = existing_comment_row;
        }
    }

    return new_comment_row;
}

/*
 * Insert a comment data object in the comment list of a row.
 */
STATIC void
_insert_comment_list(struct lxw_comment_data_rbt *comment_list,
                  lxw_comment_data *comment_data, lxw_col_t col_num)
{
    lxw_comment_data *existing_comment;

    comment_data->col_num = col_num;

    existing_comment = RB_INSERT(lxw_comment_data_rbt, comment_list, comment_data);
    if (existing_comment) {
        RB_REMOVE(lxw_comment_data_rbt, comment_list, existing_comment);

        RB_INSERT(lxw_comment_data_rbt, comment_list, comment_data);
        _free_comment_data(existing_comment);
    }

    return;
}

STATIC uint32_t
_size_col(lxw_worksheet *worksheet, uint32_t col)
{
    uint32_t max_digit_width = 7;  // For Calabri 11.
    uint32_t padding = 5;
    uint32_t pixels = 0;

    // TODO; [xyato] Look up the cell value to see if it has been changed.
// 	if (lxw_worksheet.col_sizes) {
// 	}
// 	else {
// 	}
    pixels = worksheet->default_col_pixels;

    return pixels;
}

STATIC uint32_t
_size_row(lxw_worksheet *worksheet, uint32_t row)
{
    uint32_t pixels = 0;
    // TODO; [xyato] Look up the cell value to see if it has been changed.
// 	if (lxw_worksheet.row_sizes) {
// 	}
// 	else {
// 	}
    pixels = (uint32_t)(4.0 / 3.0 * worksheet->default_row_height);
    return pixels;
}

STATIC void
_position_object_pixels(lxw_worksheet *worksheet, lxw_comment_option* option, lxw_vertices *vertices)
{
    uint32_t x_abs = 0;
    uint32_t y_abs = 0;
    uint32_t col_id;
    uint32_t row_id;
    uint32_t col_end;
    uint32_t row_end;
    uint32_t width = option->width;
    uint32_t height = option->height;

    // Adjust start column for negative offsets.
    if (option->x_offset < 0 && option->col_start > 0)
        option->x_offset += _size_col(worksheet, --option->col_start);
    
    // Adjust start row for negative offsets.
    if (option->y_offset < 0 && option->row_start > 0)
        option->y_offset += _size_row(worksheet, --option->row_start);

    if (option->x_offset < 0)
        option->x_offset = 0;
    
    if (option->y_offset < 0)
        option->y_offset = 0;

    if (worksheet->col_size_changed) {
        for (col_id = 0; col_id < option->col_start; ++col_id) {
            x_abs += _size_col(worksheet, col_id);
        }
    }
    else {
        x_abs += worksheet->default_col_pixels * option->col_start;
    }

    x_abs += option->x_offset;

    if (worksheet->row_size_changed) {
        for (row_id = 0; row_id < option->row_start; ++row_id) {
            y_abs += _size_row(worksheet, row_id);
        }
    }
    else {
        y_abs += worksheet->default_row_pixels * option->row_start;
    }

    y_abs += option->y_offset;
    
    while (option->x_offset > _size_col(worksheet, option->col_start)) {
        option->x_offset -= _size_col(worksheet, option->col_start);
        ++option->col_start;
    }
    
    while (option->y_offset > _size_row(worksheet, option->row_start)) {
        option->y_offset -= _size_row(worksheet, option->row_start);
        ++option->row_start;
    }

    col_end = option->col_start;
    row_end = option->row_start;

    option->width += option->x_offset;
    option->height += option->y_offset;

    while(option->width > _size_col(worksheet, col_end)) {
        option->width -= _size_col(worksheet, col_end);
        ++col_end;
    }

    while(option->height > _size_row(worksheet, row_end)) {
        option->height -= _size_row(worksheet, row_end);
        ++row_end;
    }

    vertices->col_start = option->col_start;
    vertices->row_start = option->row_start;
    vertices->x1 = option->x_offset;
    vertices->y1 = option->y_offset;
    vertices->col_end = col_end;
    vertices->row_end = row_end;
    vertices->x2 = option->width;
    vertices->y2 = option->height;
    vertices->x_abs = x_abs;
    vertices->y_abs = y_abs;
    vertices->width = width;
    vertices->height = height;
}

STATIC void
_handle_comment_option(lxw_worksheet *worksheet, lxw_comment_data *comment_data, lxw_comment_option* option)
{
    double width = 0;
    double height = 0;

    if (0 == option->width)
        option->width = 128;

    if (0 == option->height)
        option->height = 74;

    if (option->start_cell)	{
        option->row_start = lxw_name_to_row(option->start_cell);
        option->col_start = lxw_name_to_col(option->start_cell);
    }

    if (0 == option->row_start) {
        if (0 == comment_data->row_num) {
            option->row_start = 0;
        }
        else if (LXW_ROW_MAX - 3 == comment_data->row_num) {
            option->row_start = LXW_ROW_MAX - 7;
        }
        else if (LXW_ROW_MAX - 2 == comment_data->row_num) {
            option->row_start = LXW_ROW_MAX - 6;
        }
        else if (LXW_ROW_MAX - 1 == comment_data->row_num) {
            option->row_start = LXW_ROW_MAX - 5;
        }
        else {
            option->row_start = comment_data->row_num - 1;
        }
    }
    
    if (0 == option->y_offset) {
        if (0 == comment_data->row_num) {
            option->y_offset = 2;
        }
        else if (LXW_ROW_MAX - 3 == comment_data->row_num) {
            option->y_offset = 16;
        }
        else if (LXW_ROW_MAX - 2 == comment_data->row_num) {
            option->y_offset = 16;
        }
        else if (LXW_ROW_MAX - 1 == comment_data->row_num) {
            option->y_offset = 14;
        }
        else {
            option->y_offset = 10;
        }
    }
    
    if (0 == option->col_start) {
        if (LXW_COL_MAX - 3 == comment_data->col_num) {
            option->col_start = LXW_COL_MAX - 6;
        }
        else if (LXW_COL_MAX - 2 == comment_data->col_num) {
            option->col_start = LXW_COL_MAX - 5;
        }
        else if (LXW_COL_MAX - 1 == comment_data->col_num) {
            option->col_start = LXW_COL_MAX - 4;
        }
        else {
            option->col_start = comment_data->col_num + 1;
        }
    }
    
    if (0 == option->x_offset) {
        if (LXW_COL_MAX - 3 == comment_data->col_num) {
            option->x_offset = 49;
        }
        else if (LXW_COL_MAX - 2 == comment_data->col_num) {
            option->x_offset = 49;
        }
        else if (LXW_COL_MAX - 1 == comment_data->col_num) {
            option->x_offset = 49;
        }
        else {
            option->x_offset = 15;
        }
    }

    if (option->x_scale) {
        width = option->width * option->x_scale;
        option->width = (uint32_t)(0.5 + width);
    }

    if (option->y_scale) {
        height = option->height * option->y_scale;
        option->height = (uint32_t)(0.5 + height);
    }

    _position_object_pixels(worksheet, option, &comment_data->vertices);

    if (option->author) {
        comment_data->author = lxw_strdup(option->author);
    }

    if (option->color) {
        comment_data->color = lxw_strdup(option->color);
    }
    else {
        comment_data->color = lxw_strdup("#ffffe1");
    }

    comment_data->visible = option->visible;
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/
/*
 * Create a comment object.
 */
lxw_comment *
lxw_comment_new()
{
    lxw_comment *comment = calloc(1, sizeof(lxw_comment));
    RETURN_ON_MEM_ERROR(comment, NULL);

    comment->comment_row_list = calloc(1, sizeof(struct lxw_comment_row_rbt));
    GOTO_LABEL_ON_MEM_ERROR(comment->comment_row_list, mem_error);

    RB_INIT(comment->comment_row_list);

    comment->author_ids = lxw_hash_new(128, 0, 1);

    return comment;

mem_error:
    lxw_comment_free(comment);
    return NULL;
}

/*
 * Free a comment object.
 */
void
lxw_comment_free(lxw_comment *comment)
{
    lxw_comment_row *comment_row;
    lxw_comment_row *next_comment_row;

    if (!comment)
        return;

    if (comment->comment_row_list) {
        for (comment_row = RB_MIN(lxw_comment_row_rbt, comment->comment_row_list); comment_row; comment_row = next_comment_row) {
                next_comment_row = RB_NEXT(lxw_comment_row_rbt, comment->comment_row_list, comment_row);
                RB_REMOVE(lxw_comment_row_rbt, comment->comment_row_list, comment_row);
                _free_comment_row(comment_row);
        }

        free(comment->comment_row_list);
    }

    lxw_hash_free(comment->author_ids);
    free(comment);
}

/*
 * Assemble and write the XML file.
 */
void
lxw_comment_assemble_xml_file(struct lxw_comment *self)
{
    if (!self || RB_EMPTY(self->comment_row_list))
        return;

    /* Write the XML declaration. */
    _comment_xml_declaration(self);

    /* Write the comments element. */
    _write_comments(self);

    /* Write the authors element. */
    _write_authors(self);

    /* Write the commentList element. */
    _write_comment_list(self);

    lxw_xml_end_tag(self->file, "comments");
}

lxw_error
lxw_comment_write(lxw_worksheet *worksheet,
                  lxw_row_t row_num, lxw_col_t col_num, const char *text, lxw_comment_option *option)
{
    lxw_comment_data *comment_data;
    lxw_comment_row *comment_row;
    char *string_copy;
    lxw_comment_option comment_option = { 0 };

    if (option)
        comment_option = *option;

    string_copy = lxw_strdup(text);
    if (!string_copy)
        return LXW_ERROR_MEMORY_MALLOC_FAILED;

    comment_data = _new_comment_data(row_num, col_num, string_copy);
    if (!comment_data)
        return LXW_ERROR_MEMORY_MALLOC_FAILED;

    comment_row = _get_comment_row(worksheet->comment->comment_row_list, row_num);
    if (!comment_row) {
        _free_comment_data(comment_data);
        return LXW_ERROR_MEMORY_MALLOC_FAILED;
    }

    _handle_comment_option(worksheet, comment_data, &comment_option);

    _insert_comment_list(comment_row->comments, comment_data, col_num);

    return LXW_NO_ERROR;
}

STATIC void
_write_comment_fill(FILE *file)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("color2", "#ffffe1");

    lxw_xml_empty_tag(file, "v:fill", &attributes);

    LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_shadow(FILE *file)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("on", "t");
    LXW_PUSH_ATTRIBUTES_STR("color", "black");
    LXW_PUSH_ATTRIBUTES_STR("obscured", "t");

    lxw_xml_empty_tag(file, "v:shadow", &attributes);

    LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_comment_path(FILE *file)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("o:connecttype", "none");

    lxw_xml_empty_tag(file, "v:path", &attributes);

    LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_div(FILE *file)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("style", "text-align:left");

    lxw_xml_start_tag(file, "div", &attributes);

    lxw_xml_end_tag(file, "div");

    LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_comment_textbox(FILE *file)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("style", "mso-direction-alt:auto");

    lxw_xml_start_tag(file, "v:textbox", &attributes);

    _write_div(file);

    lxw_xml_end_tag(file, "v:textbox");

    LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_move_with_cells(FILE *file)
{
    lxw_xml_empty_tag(file, "x:MoveWithCells", NULL);
}

STATIC void
_write_size_with_cells(FILE *file)
{
    lxw_xml_empty_tag(file, "x:SizeWithCells", NULL);
}

STATIC void
_write_anchor(FILE *file, lxw_vertices *vertices)
{
    char data[128];
    lxw_snprintf(data, sizeof(data), "%d, %d, %d, %d, %d, %d, %d, %d", 
        vertices->col_start, vertices->x1, vertices->row_start, vertices->y1, vertices->col_end, vertices->x2, vertices->row_end, vertices->y2);

    lxw_xml_data_element(file, "x:Anchor", data, NULL);
}

STATIC void
_write_auto_fill(FILE *file)
{
    lxw_xml_data_element(file, "x:AutoFill", "False", NULL);
}

STATIC void
_write_row(FILE *file, uint32_t row)
{
    char data[LXW_UINT32_T_LENGTH];
    lxw_snprintf(data, LXW_UINT32_T_LENGTH, "%u", row);

    lxw_xml_data_element(file, "x:Row", data, NULL);
}

STATIC void
_write_column(FILE *file, uint32_t col)
{
    char data[LXW_UINT32_T_LENGTH];
    lxw_snprintf(data, LXW_UINT32_T_LENGTH, "%u", col);

    lxw_xml_data_element(file, "x:Column", data, NULL);
}

STATIC void
_write_visible(FILE *file)
{
    lxw_xml_empty_tag(file, "v:Visible", NULL);
}

STATIC void
_write_comment_client_data(FILE *file, lxw_comment_data *comment_data)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("ObjectType", "Note");

    lxw_xml_start_tag(file, "x:ClientData", &attributes);

    _write_move_with_cells(file);
    _write_size_with_cells(file);
    _write_anchor(file, &comment_data->vertices);
    _write_auto_fill(file);
    _write_row(file, comment_data->row_num);
    _write_column(file, comment_data->col_num);

    if (comment_data->visible) {
        _write_visible(file);
    }

    lxw_xml_end_tag(file, "x:ClientData");

    LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_comment_shape(FILE *file, lxw_comment_data *comment_data, uint32_t shape_id, uint32_t z_index)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    double left;
    double top;
    double width;
    double height;

    char *shape_type = "#_x0000_t202";
    char *insetmode = "auto";
    char *visibility = "hidden";

    char str_shape_id[128] = { 0 };
    char str_style[1024] = { 0 };

    if (comment_data->visible)
        visibility = "visible";

    left = comment_data->vertices.x_abs * 0.75;
    top = comment_data->vertices.y_abs * 0.75;
    width = comment_data->vertices.width * 0.75;
    height = comment_data->vertices.height * 0.75;

    lxw_snprintf(str_shape_id, sizeof(str_shape_id), "_x0000_s%d", shape_id);
    lxw_snprintf(str_style, sizeof(str_style), 
        "position:absolute;margin-left:%.15gpt;margin-top:%.15gpt;width:%.15gpt;height:%.15gpt;z-index:%d;visibility:%s", left, top, width, height, z_index, visibility);

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("id", str_shape_id);
    LXW_PUSH_ATTRIBUTES_STR("type", shape_type);
    LXW_PUSH_ATTRIBUTES_STR("style", str_style);
    LXW_PUSH_ATTRIBUTES_STR("fillcolor", comment_data->color);
    LXW_PUSH_ATTRIBUTES_STR("o:insetmode", insetmode);

    lxw_xml_start_tag(file, "v:shape", &attributes);

    _write_comment_fill(file);

    _write_shadow(file);

    _write_comment_path(file);

    _write_comment_textbox(file);

    _write_comment_client_data(file, comment_data);

    lxw_xml_end_tag(file, "v:shape");

    LXW_FREE_ATTRIBUTES();
}

void
lxw_comment_write_shape(lxw_comment *self, FILE *file, uint32_t vml_shape_id)
{
    lxw_comment_row *comment_row;
    lxw_comment_data *comment_data;
    uint32_t z_index = 1;

    RB_FOREACH(comment_row, lxw_comment_row_rbt, self->comment_row_list) {
        RB_FOREACH(comment_data, lxw_comment_data_rbt, comment_row->comments) {
            ++vml_shape_id;
            _write_comment_shape(file, comment_data, vml_shape_id, z_index);
            ++z_index;
        }
    }
}