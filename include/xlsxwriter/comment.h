/*
 * libxlsxwriter
 *
 * Copyright 2017, Xyato, coszyx@gmail.com. See LICENSE.txt.
 *
 * comment - A libxlsxwriter library for creating Excel XLSX
 *                  comment files.
 *
 */
#ifndef __LXW_COMMENT_H__
#define __LXW_COMMENT_H__

#include <string.h>
#include <stdint.h>

#include "common.h"
#include "worksheet.h"

#define LXW_RB_GENERATE_COMMENT(name, type, field, cmp)       \
    RB_GENERATE_INSERT_COLOR(name, type, field, static)   \
    RB_GENERATE_REMOVE_COLOR(name, type, field, static)   \
    RB_GENERATE_INSERT(name, type, field, cmp, static)    \
    RB_GENERATE_REMOVE(name, type, field, static)         \
    RB_GENERATE_FIND(name, type, field, cmp, static)      \
    RB_GENERATE_NEXT(name, type, field, static)           \
    RB_GENERATE_MINMAX(name, type, field, static)         \
    /* Add unused struct to allow adding a semicolon */   \
    struct lxw_rb_generate_comment{int unused;}

#define LXW_RB_GENERATE_COMMENT_ROW(name, type, field, cmp)       \
    RB_GENERATE_INSERT_COLOR(name, type, field, static)   \
    RB_GENERATE_REMOVE_COLOR(name, type, field, static)   \
    RB_GENERATE_INSERT(name, type, field, cmp, static)    \
    RB_GENERATE_REMOVE(name, type, field, static)         \
    RB_GENERATE_FIND(name, type, field, cmp, static)      \
    RB_GENERATE_NEXT(name, type, field, static)           \
    RB_GENERATE_MINMAX(name, type, field, static)         \
    /* Add unused struct to allow adding a semicolon */   \
struct lxw_rb_generate_row_comment{int unused;}

typedef struct lxw_vertices {
    uint32_t col_start;
    uint32_t row_start;
    uint32_t x1;
    uint32_t y1;
    uint32_t col_end;
    uint32_t row_end;
    uint32_t x2;
    uint32_t y2;
    uint32_t x_abs;
    uint32_t y_abs;
    uint32_t width;
    uint32_t height;
} lxw_vertices;

/*
 * Struct to represent a comment.
 */
typedef struct lxw_comment_data {
    lxw_row_t row_num;
    lxw_col_t col_num;
    char *text;
    char *author;
    enum lxw_boolean visible;
    char *color;

    struct lxw_vertices vertices;
    RB_ENTRY (lxw_comment_data) tree_pointers;
} lxw_comment_data;

RB_HEAD(lxw_comment_data_rbt, lxw_comment_data);

typedef struct lxw_comment_row {
    lxw_row_t row_num;
    struct lxw_comment_data_rbt *comments;

    RB_ENTRY (lxw_comment_row) tree_pointers;
} lxw_comment_row;

/* Define the tree.h RB structs for the red-black head types. */
RB_HEAD(lxw_comment_row_rbt, lxw_comment_row);

typedef struct lxw_comment {
    FILE *file;
    lxw_hash_table *author_ids;
    struct lxw_comment_row_rbt *comment_row_list;
} lxw_comment;

/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

lxw_comment *lxw_comment_new();
void lxw_comment_free(lxw_comment *comment);
void lxw_comment_assemble_xml_file(lxw_comment *self);
lxw_error lxw_comment_write(struct lxw_worksheet *worksheet, lxw_row_t row_num, lxw_col_t col_num, const char *string, struct lxw_comment_option *option);
void lxw_comment_write_shape(lxw_comment* self, FILE* file, uint32_t vml_shape_id);

/* Declarations required for unit testing. */
#ifdef TESTING

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_COMMENT_H__ */
