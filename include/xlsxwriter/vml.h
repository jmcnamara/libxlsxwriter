/*
 * libxlsxwriter
 *
 * Copyright 2017, Xyato, coszyx@gmail.com. See LICENSE.txt.
 *
 * vml - A libxlsxwriter library for creating Excel XLSX
 *                  vml files.
 *
 */
#ifndef __LXW_VML_H__
#define __LXW_VML_H__

#include <string.h>
#include <stdint.h>

#include "common.h"
#include "worksheet.h"

/*
 * Struct to represent vml
 */
typedef struct lxw_vml {
    FILE *file;

} lxw_vml;

/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

lxw_vml *lxw_vml_new();
void lxw_vml_free(lxw_vml *vml);
void lxw_vml_assemble_xml_file(lxw_vml *self, lxw_worksheet *worksheet);

/* Declarations required for unit testing. */
#ifdef TESTING

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_COMMENT_H__ */
