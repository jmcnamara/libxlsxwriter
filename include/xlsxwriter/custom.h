/*
 * libxlsxwriter
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 * custom - A libxlsxwriter library for creating Excel XLSX custom files.
 *
 */
#ifndef __LXW_CUSTOM_H__
#define __LXW_CUSTOM_H__

#include <stdint.h>

#include "common.h"

/*
 * Struct to represent a custom object.
 */
typedef struct lxw_custom {

    FILE *file;

} lxw_custom;


/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

lxw_custom *lxw_custom_new();
void lxw_custom_free(lxw_custom *custom);
void lxw_custom_assemble_xml_file(lxw_custom *self);

/* Declarations required for unit testing. */
#ifdef TESTING

STATIC void _custom_xml_declaration(lxw_custom *self);

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_CUSTOM_H__ */
