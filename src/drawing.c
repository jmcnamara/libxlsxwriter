/*****************************************************************************
 * drawing - A library for creating Excel XLSX drawing files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/drawing.h"
#include "xlsxwriter/utility.h"

/*
 * Forward declarations.
 */

/*****************************************************************************
 *
 * Private functions.
 *
 ****************************************************************************/

/*
 * Create a new drawing object.
 */
lxw_drawing *
_new_drawing()
{
    lxw_drawing *drawing = calloc(1, sizeof(lxw_drawing));
    GOTO_LABEL_ON_MEM_ERROR(drawing, mem_error);

    return drawing;

mem_error:
    _free_drawing(drawing);
    return NULL;
}

/*
 * Free a drawing object.
 */
void
_free_drawing(lxw_drawing *drawing)
{
    if (!drawing)
        return;

    free(drawing);
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
_drawing_xml_declaration(lxw_drawing *self)
{
    _xml_declaration(self->file);
}

/*****************************************************************************
 *
 * XML file assembly functions.
 *
 ****************************************************************************/

/*
 * Assemble and write the XML file.
 */
void
_drawing_assemble_xml_file(lxw_drawing *self)
{
    /* Write the XML declaration. */
    _drawing_xml_declaration(self);

    _xml_end_tag(self->file, "xdr:wsDr");
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/
