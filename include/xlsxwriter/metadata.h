/*
 * libxlsxwriter
 *
 * Copyright 2014-2021, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 * metadata - A libxlsxwriter library for creating Excel XLSX metadata files.
 *
 */
#ifndef __LXW_METADATA_H__
#define __LXW_METADATA_H__

#include <stdint.h>

#include "common.h"

/*
 * Struct to represent a metadata object.
 */
typedef struct lxw_metadata {

    FILE *file;

} lxw_metadata;


/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

lxw_metadata *lxw_metadata_new(void);
void lxw_metadata_free(lxw_metadata *metadata);
void lxw_metadata_assemble_xml_file(lxw_metadata *self);

/* Declarations required for unit testing. */
#ifdef TESTING

STATIC void _metadata_xml_declaration(lxw_metadata *self);

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_METADATA_H__ */
