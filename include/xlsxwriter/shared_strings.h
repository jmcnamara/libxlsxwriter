/*
 * libxlsxwriter
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 * shared_strings - A libxlsxwriter library for creating Excel XLSX
 *                  sst files.
 *
 */
#ifndef __LXW_SST_H__
#define __LXW_SST_H__

#include <string.h>
#include <stdint.h>
#include "xlsxwriter/third_party/queue.h"

#include "common.h"

#define NUM_SST_BUCKETS 1024

/* STAILQ_HEAD() declaration. */
struct sst_order_list {
    struct sst_element *stqh_first;
    struct sst_element **stqh_last;
};

/* SLIST_HEAD() declaration. */
struct sst_bucket_list {
    struct sst_element *slh_first;
};

/*
 * Elements of the SST table. They contain pointers to allow them to
 * be stored in lists in the the hash table buckets and also pointers to
 * track the insertion order in a separate list.
 */
struct sst_element {
    uint32_t index;
    char *string;

    STAILQ_ENTRY (sst_element) sst_order_pointers;
    SLIST_ENTRY (sst_element) sst_list_pointers;
};

/*
 * Struct to represent a sst.
 */
typedef struct lxw_sst {
    FILE *file;

    uint32_t num_buckets;
    uint32_t used_buckets;
    uint32_t string_count;
    uint32_t unique_count;

    struct sst_order_list *order_list;
    struct sst_bucket_list **buckets;

} lxw_sst;

/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

lxw_sst *_new_sst();
void _free_sst(lxw_sst *sst);
int32_t _get_sst_index(lxw_sst *sst, const char *string);
void _sst_assemble_xml_file(lxw_sst *self);

/* Declarations required for unit testing. */
#ifdef TESTING

STATIC void _sst_xml_declaration(lxw_sst *self);

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_SST_H__ */
