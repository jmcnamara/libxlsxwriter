/*****************************************************************************
 * core - A library for creating Excel XLSX core files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/core.h"
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
 * Create a new core object.
 */
lxw_core *
_new_core()
{
    lxw_core *core = calloc(1, sizeof(lxw_core));
    GOTO_LABEL_ON_MEM_ERROR(core, mem_error);

    return core;

mem_error:
    _free_core(core);
    return NULL;
}

/*
 * Free a core object.
 */
void
_free_core(lxw_core *core)
{
    if (!core)
        return;

    free(core);
}

/*
 * Convert a time_t struct to a ISO 8601 style "2010-01-01T00:00:00Z" date.
 */
static void
_localtime_to_iso8601_date(time_t *timer, char *str, size_t size)
{
    struct tm *tmp_localtime;
    time_t current_time = time(NULL);

    if (*timer)
        tmp_localtime = localtime(timer);
    else
        tmp_localtime = localtime(&current_time);

    strftime(str, size - 1, "%Y-%m-%dT%H:%M:%SZ", tmp_localtime);
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
_core_xml_declaration(lxw_core *self)
{
    _xml_declaration(self->file);
}

/*
 * Write the <cp:coreProperties> element.
 */
STATIC void
_write_cp_core_properties(lxw_core *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("xmlns:cp",
                         "http://schemas.openxmlformats.org/package/2006/metadata/core-properties");
    _PUSH_ATTRIBUTES_STR("xmlns:dc", "http://purl.org/dc/elements/1.1/");
    _PUSH_ATTRIBUTES_STR("xmlns:dcterms", "http://purl.org/dc/terms/");
    _PUSH_ATTRIBUTES_STR("xmlns:dcmitype", "http://purl.org/dc/dcmitype/");
    _PUSH_ATTRIBUTES_STR("xmlns:xsi",
                         "http://www.w3.org/2001/XMLSchema-instance");

    _xml_start_tag(self->file, "cp:coreProperties", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <dc:creator> element.
 */
STATIC void
_write_dc_creator(lxw_core *self)
{
    if (self->properties->author) {
        _xml_data_element(self->file, "dc:creator", self->properties->author,
                          NULL);
    }
    else {
        _xml_data_element(self->file, "dc:creator", "", NULL);
    }
}

/*
 * Write the <cp:lastModifiedBy> element.
 */
STATIC void
_write_cp_last_modified_by(lxw_core *self)
{
    if (self->properties->author) {
        _xml_data_element(self->file, "cp:lastModifiedBy",
                          self->properties->author, NULL);
    }
    else {
        _xml_data_element(self->file, "cp:lastModifiedBy", "", NULL);
    }
}

/*
 * Write the <dcterms:created> element.
 */
STATIC void
_write_dcterms_created(lxw_core *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char datetime[ATTR_32];

    _localtime_to_iso8601_date(&self->properties->created, datetime, ATTR_32);

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("xsi:type", "dcterms:W3CDTF");

    _xml_data_element(self->file, "dcterms:created", datetime, &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <dcterms:modified> element.
 */
STATIC void
_write_dcterms_modified(lxw_core *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char datetime[ATTR_32];

    _localtime_to_iso8601_date(&self->properties->created, datetime, ATTR_32);

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("xsi:type", "dcterms:W3CDTF");

    _xml_data_element(self->file, "dcterms:modified", datetime, &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <dc:title> element.
 */
STATIC void
_write_dc_title(lxw_core *self)
{
    if (!self->properties->title)
        return;

    _xml_data_element(self->file, "dc:title", self->properties->title, NULL);
}

/*
 * Write the <dc:subject> element.
 */
STATIC void
_write_dc_subject(lxw_core *self)
{
    if (!self->properties->subject)
        return;

    _xml_data_element(self->file, "dc:subject", self->properties->subject,
                      NULL);
}

/*
 * Write the <cp:keywords> element.
 */
STATIC void
_write_cp_keywords(lxw_core *self)
{
    if (!self->properties->keywords)
        return;

    _xml_data_element(self->file, "cp:keywords", self->properties->keywords,
                      NULL);
}

/*
 * Write the <dc:description> element.
 */
STATIC void
_write_dc_description(lxw_core *self)
{
    if (!self->properties->comments)
        return;

    _xml_data_element(self->file, "dc:description",
                      self->properties->comments, NULL);
}

/*
 * Write the <cp:category> element.
 */
STATIC void
_write_cp_category(lxw_core *self)
{
    if (!self->properties->category)
        return;

    _xml_data_element(self->file, "cp:category", self->properties->category,
                      NULL);
}

/*
 * Write the <cp:contentStatus> element.
 */
STATIC void
_write_cp_content_status(lxw_core *self)
{
    if (!self->properties->status)
        return;

    _xml_data_element(self->file, "cp:contentStatus",
                      self->properties->status, NULL);
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
_core_assemble_xml_file(lxw_core *self)
{
    /* Write the XML declaration. */
    _core_xml_declaration(self);

    _write_cp_core_properties(self);
    _write_dc_title(self);
    _write_dc_subject(self);
    _write_dc_creator(self);
    _write_cp_keywords(self);
    _write_dc_description(self);
    _write_cp_last_modified_by(self);
    _write_dcterms_created(self);
    _write_dcterms_modified(self);
    _write_cp_category(self);
    _write_cp_content_status(self);

    _xml_end_tag(self->file, "cp:coreProperties");
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/
