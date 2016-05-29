/*
 * libxlsxwriter
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 */

/**
 * @file common.h
 *
 * @brief Common functions and defines for the libxlsxwriter library.
 *
 * <!-- Copyright 2014-2016, John McNamara, jmcnamara@cpan.org -->
 *
 */
#ifndef __LXW_COMMON_H__
#define __LXW_COMMON_H__

#include <time.h>
#include "xlsxwriter/third_party/queue.h"
#include "xlsxwriter/third_party/tree.h"

#ifndef TESTING
#define STATIC static
#else
#define STATIC
#endif

/** Integer data type to represent a row value. Equivalent to `uint32_t`.
 *
 * The maximum row in Excel is 1,048,576.
 */
typedef uint32_t lxw_row_t;

/** Integer data type to represent a column value. Equivalent to `uint16_t`.
 *
 * The maximum column in Excel is 16,384.
 */
typedef uint16_t lxw_col_t;

/** Boolean values used in libxlsxwriter. */
enum lxw_boolean {
    /** False value. */
    LXW_FALSE,
    /** True value. */
    LXW_TRUE
};

/* Excel sheetname max of 31 chars + \0. */
#define LXW_SHEETNAME_MAX     32

/* Every worksheet char doubled + start and end quotes + \0. */
#define LXW_MAX_SHEETNAME_LENGTH  65

/* Max col: $XFD\0 */
#define LXW_MAX_COL_NAME_LENGTH   5

/* Max cell: $XFWD$1048576\0 */
#define LXW_MAX_CELL_NAME_LENGTH  14

/* Max range: $XFWD$1048576:$XFWD$1048576\0 */
#define LXW_MAX_CELL_RANGE_LENGTH (LXW_MAX_CELL_NAME_LENGTH * 2)

/* Max range formula Sheet1!$A$1:$C$5$ style. */
#define LXW_MAX_FORMULA_RANGE_LENGTH (LXW_MAX_SHEETNAME_LENGTH + LXW_MAX_CELL_RANGE_LENGTH)

#define LXW_EPOCH_1900            0
#define LXW_EPOCH_1904            1

#define LXW_UINT32_T_LENGTH   11        /* Length of 4294967296\0. */
#define LXW_IGNORE            1
#define LXW_FILENAME_LENGTH   128
#define LXW_NO_ERROR          0

#define LXW_SCHEMA_ROOT      "http://schemas.openxmlformats.org"
#define LXW_SCHEMA_DRAWING   LXW_SCHEMA_ROOT "/drawingml/2006"
#define LXW_SCHEMA_OFFICEDOC LXW_SCHEMA_ROOT "/officeDocument/2006"
#define LXW_SCHEMA_PACKAGE   LXW_SCHEMA_ROOT "/package/2006/relationships"
#define LXW_SCHEMA_DOCUMENT  LXW_SCHEMA_ROOT "/officeDocument/2006/relationships"
#define LXW_SCHEMA_CONTENT   LXW_SCHEMA_ROOT "/package/2006/content-types"
#define LXW_SCHEMA_MS       "http://schemas.microsoft.com/office/2006/relationships"

#define LXW_ERROR(message)                      \
    fprintf(stderr, "[ERROR][%s:%d]: " message "\n", __FILE__, __LINE__)

#define LXW_MEM_ERROR()                         \
    LXW_ERROR("Memory allocation failed.")

#define GOTO_LABEL_ON_MEM_ERROR(pointer, label) \
    if (!pointer) {                             \
        LXW_MEM_ERROR();                        \
        goto label;                             \
    }

#define RETURN_ON_MEM_ERROR(pointer, error)     \
    if (!pointer) {                             \
        LXW_MEM_ERROR();                        \
        return error;                           \
    }

#define RETURN_VOID_ON_MEM_ERROR(pointer)       \
    if (!pointer) {                             \
        LXW_MEM_ERROR();                        \
        return;                                 \
    }

#define RETURN_ON_ERROR(error)                  \
    if (error)                                  \
        return error;

#define LXW_WARN(message)                       \
    fprintf(stderr, "[WARNING]: " message "\n")

/* Can't use variadic macros here since we support ANSI C. */
#define LXW_WARN_FORMAT(message, var)           \
    fprintf(stderr, "[WARNING]: " message "\n", var)

#define LXW_WARN_FORMAT2(message, var1, var2)    \
    fprintf(stderr, "[WARNING]: " message "\n", var1, var2)

#ifndef LXW_BIG_ENDIAN
#define LXW_UINT32_NETWORK(n) ((((n) & 0xFF)       << 24) | \
                               (((n) & 0xFF00)     <<  8) | \
                               (((n) & 0xFF0000)   >>  8) | \
                               (((n) & 0xFF000000) >> 24))
#define LXW_UINT16_NETWORK(n) ((((n) & 0x00FF) << 8) | (((n) & 0xFF00) >> 8))
#else
#define LXW_UINT32_NETWORK(n) (n)
#define LXW_UINT16_NETWORK(n) (n)
#endif

/* Compilers that have a native snprintf() can use it directly. */
#ifdef _MSC_VER
#define LXW_HAS_SNPRINTF
#endif

#ifdef LXW_HAS_SNPRINTF
#define lxw_snprintf snprintf
#else
#define lxw_snprintf __builtin_snprintf
#endif

/* Define the queue.h structs for the formats list. */
STAILQ_HEAD(lxw_formats, lxw_format);

/* Define the queue.h structs for the generic data structs. */
STAILQ_HEAD(lxw_tuples, lxw_tuple);

typedef struct lxw_tuple {
    char *key;
    char *value;

    STAILQ_ENTRY (lxw_tuple) list_pointers;
} lxw_tuple;


 /* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

/* Declarations required for unit testing. */
#ifdef TESTING

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_COMMON_H__ */
