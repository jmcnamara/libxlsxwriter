/*****************************************************************************
 * chartsheet - A library for creating Excel XLSX chartsheet files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2018, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/chartsheet.h"
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
 * Create a new chartsheet object.
 */
lxw_chartsheet *
lxw_chartsheet_new()
{
    lxw_chartsheet *chartsheet = calloc(1, sizeof(lxw_chartsheet));
    GOTO_LABEL_ON_MEM_ERROR(chartsheet, mem_error);

    /* Use an embedded worksheet instance to write XML records that are
     * shared with worksheet.c. */
    chartsheet->worksheet = lxw_worksheet_new(NULL);
    GOTO_LABEL_ON_MEM_ERROR(chartsheet->worksheet, mem_error);

    return chartsheet;

mem_error:
    lxw_chartsheet_free(chartsheet);
    return NULL;
}

/*
 * Free a chartsheet object.
 */
void
lxw_chartsheet_free(lxw_chartsheet *chartsheet)
{
    if (!chartsheet)
        return;

    lxw_worksheet_free(chartsheet->worksheet);

    if (chartsheet->drawing)
        lxw_drawing_free(chartsheet->drawing);

    free(chartsheet);
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
_chartsheet_xml_declaration(lxw_chartsheet *self)
{
    lxw_xml_declaration(self->file);
}

/*
 * Write the <chartsheet> element.
 */
STATIC void
_chartsheet_write_chartsheet(lxw_chartsheet *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char xmlns[] = "http://schemas.openxmlformats.org/"
        "spreadsheetml/2006/main";
    char xmlns_r[] = "http://schemas.openxmlformats.org/"
        "officeDocument/2006/relationships";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("xmlns", xmlns);
    LXW_PUSH_ATTRIBUTES_STR("xmlns:r", xmlns_r);

    lxw_xml_start_tag(self->file, "chartsheet", &attributes);
    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <sheetPr> element.
 */
STATIC void
_chartsheet_write_sheet_pr(lxw_chartsheet *self)
{
    lxw_xml_empty_tag(self->file, "sheetPr", NULL);
}

/*
 * Write the <sheetViews> element.
 */
STATIC void
_chartsheet_write_sheet_views(lxw_chartsheet *self)
{
    lxw_worksheet_write_sheet_views(self->worksheet);
}

/*
 * Write the <pageMargins> element.
 */
STATIC void
_chartsheet_write_page_margins(lxw_chartsheet *self)
{
    lxw_worksheet_write_page_margins(self->worksheet);
}

/*
 * Write the <drawing> element.
 */
STATIC void
_chartsheet_write_drawing(lxw_chartsheet *self, uint16_t id)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char r_id[LXW_MAX_ATTRIBUTE_LENGTH];

    lxw_snprintf(r_id, LXW_ATTR_32, "rId%d", id);

    LXW_INIT_ATTRIBUTES();

    LXW_PUSH_ATTRIBUTES_STR("r:id", r_id);

    lxw_xml_empty_tag(self->file, "drawing", &attributes);

    LXW_FREE_ATTRIBUTES();

}

/*
 * Write the <drawing> elements.
 */
STATIC void
_chartsheet_write_drawings(lxw_chartsheet *self)
{
    if (!self->drawing)
        return;

    self->rel_count++;

    _chartsheet_write_drawing(self, self->rel_count);
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
lxw_chartsheet_assemble_xml_file(lxw_chartsheet *self)
{
    /* Set the embedded worksheet filehandle to the same as the chartsheet. */
    self->worksheet->file = self->file;

    /* Write the XML declaration. */
    _chartsheet_xml_declaration(self);

    /* Write the chartsheet element. */
    _chartsheet_write_chartsheet(self);

    /* Write the sheetPr element. */
    _chartsheet_write_sheet_pr(self);

    /* Write the sheetViews element. */
    _chartsheet_write_sheet_views(self);

    /* Write the pageMargins element. */
    _chartsheet_write_page_margins(self);

    /* Write the drawing element. */
    _chartsheet_write_drawings(self);

    lxw_xml_end_tag(self->file, "chartsheet");
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/
