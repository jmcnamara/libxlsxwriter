/*****************************************************************************
 * custom - A library for creating Excel XLSX custom files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/custom.h"
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
 * Create a new custom object.
 */
lxw_custom *
lxw_custom_new()
{
    lxw_custom *custom = calloc(1, sizeof(lxw_custom));
    GOTO_LABEL_ON_MEM_ERROR(custom, mem_error);

    return custom;

mem_error:
    lxw_custom_free(custom);
    return NULL;
}

/*
 * Free a custom object.
 */
void
lxw_custom_free(lxw_custom *custom)
{
    if (!custom)
        return;

    free(custom);
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
_custom_xml_declaration(lxw_custom *self)
{
    lxw_xml_declaration(self->file);
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
lxw_custom_assemble_xml_file(lxw_custom *self)
{
    /* Write the XML declaration. */
    _custom_xml_declaration(self);

    lxw_xml_end_tag(self->file, "custom");
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/
