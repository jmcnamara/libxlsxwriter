/*
 * libxlsxwriter
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 * common - Common functions and defines for the libxlsxwriter library.
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

enum lxw_boolean {
    LXW_FALSE,
    LXW_TRUE
};

#define LXW_SHEETNAME_MAX  32
#define LXW_SHEETNAME_LEN  65
#define LXW_UINT32_T_LEN   11   /* Length of 4294967296\0. */
#define LXW_IGNORE          1
#define FILENAME_LEN      128
#define LXW_NO_ERROR        0

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

#define LXW_WARN(message)                       \
    fprintf(stderr, "[WARN]: " message "\n")

#define LXW_WARN_FORMAT(message, var)           \
    fprintf(stderr, "[WARN]: " message "\n", var)

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
#define HAS_SNPRINTF
#endif

#ifdef HAS_SNPRINTF
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
