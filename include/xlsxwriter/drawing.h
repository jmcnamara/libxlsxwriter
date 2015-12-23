/*
 * libxlsxwriter
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 * drawing - A libxlsxwriter library for creating Excel XLSX drawing files.
 *
 */
#ifndef __LXW_DRAWING_H__
#define __LXW_DRAWING_H__

#include <stdint.h>

#include "common.h"

/*
 * Struct to represent a drawing object.
 */
typedef struct lxw_drawing {

    FILE *file;

} lxw_drawing;


/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

lxw_drawing *_new_drawing();
void _free_drawing(lxw_drawing *drawing);
void _drawing_assemble_xml_file(lxw_drawing *self);

/* Declarations required for unit testing. */
#ifdef TESTING

STATIC void _drawing_xml_declaration(lxw_drawing *self);

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_DRAWING_H__ */
