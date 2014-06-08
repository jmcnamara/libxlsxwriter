/*****************************************************************************
 * theme - A library for creating Excel XLSX theme files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/theme.h"
#include "xlsxwriter/utility.h"

#pragma GCC diagnostic ignored "-Woverlength-strings"

/*
 * Forward declarations.
 */

/*****************************************************************************
 *
 * Private functions.
 *
 ****************************************************************************/

/*
 * Create a new theme object.
 */
lxw_theme *
_new_theme()
{
    lxw_theme *theme = calloc(1, sizeof(lxw_theme));
    GOTO_LABEL_ON_MEM_ERROR(theme, mem_error);

    return theme;

mem_error:
    _free_theme(theme);
    return NULL;
}

/*
 * Free a theme object.
 */
void
_free_theme(lxw_theme *theme)
{
    if (!theme)
        return;

    free(theme);
}

/*****************************************************************************
 *
 * XML functions.
 *
 ****************************************************************************/

/* This library isn't a xmlwriter. */

/*****************************************************************************
 *
 * XML file assembly functions.
 *
 ****************************************************************************/

/*
 * Assemble and write the XML file.
 */
void
_theme_assemble_xml_file(lxw_theme *self)
{
    fputs(THEME_DATA, self->file);
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/
