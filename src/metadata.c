/*****************************************************************************
 * metadata - A library for creating Excel XLSX metadata files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2022, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/metadata.h"
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
 * Create a new metadata object.
 */
lxw_metadata *
lxw_metadata_new(void)
{
    lxw_metadata *metadata = calloc(1, sizeof(lxw_metadata));
    GOTO_LABEL_ON_MEM_ERROR(metadata, mem_error);

    return metadata;

mem_error:
    lxw_metadata_free(metadata);
    return NULL;
}

/*
 * Free a metadata object.
 */
void
lxw_metadata_free(lxw_metadata *metadata)
{
    if (!metadata)
        return;

    free(metadata);
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
_metadata_xml_declaration(lxw_metadata *self)
{
    lxw_xml_declaration(self->file);
}

/*
 * Write the <metadata> element.
 */
STATIC void
_metadata_write_metadata(lxw_metadata *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char xmlns[] = "http://schemas.openxmlformats.org/"
        "spreadsheetml/2006/main";
    char xmlns_xda[] = "http://schemas.microsoft.com/office/"
        "spreadsheetml/2017/dynamicarray";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("xmlns", xmlns);
    LXW_PUSH_ATTRIBUTES_STR("xmlns:xda", xmlns_xda);

    lxw_xml_start_tag(self->file, "metadata", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <metadataType> element.
 */
STATIC void
_metadata_write_metadata_type(lxw_metadata *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("name", "XLDAPR");
    LXW_PUSH_ATTRIBUTES_INT("minSupportedVersion", 120000);
    LXW_PUSH_ATTRIBUTES_INT("copy", 1);
    LXW_PUSH_ATTRIBUTES_INT("pasteAll", 1);
    LXW_PUSH_ATTRIBUTES_INT("pasteValues", 1);
    LXW_PUSH_ATTRIBUTES_INT("merge", 1);
    LXW_PUSH_ATTRIBUTES_INT("splitFirst", 1);
    LXW_PUSH_ATTRIBUTES_INT("rowColShift", 1);
    LXW_PUSH_ATTRIBUTES_INT("clearFormats", 1);
    LXW_PUSH_ATTRIBUTES_INT("clearComments", 1);
    LXW_PUSH_ATTRIBUTES_INT("assign", 1);
    LXW_PUSH_ATTRIBUTES_INT("coerce", 1);
    LXW_PUSH_ATTRIBUTES_INT("cellMeta", 1);

    lxw_xml_empty_tag(self->file, "metadataType", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <metadataTypes> element.
 */
STATIC void
_metadata_write_metadata_types(lxw_metadata *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("count", 1);

    lxw_xml_start_tag(self->file, "metadataTypes", &attributes);

    /* Write the metadataType element. */
    _metadata_write_metadata_type(self);

    lxw_xml_end_tag(self->file, "metadataTypes");

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <xda:dynamicArrayProperties> element.
 */
STATIC void
_metadata_write_xda_dynamic_array_properties(lxw_metadata *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("fDynamic", "1");
    LXW_PUSH_ATTRIBUTES_STR("fCollapsed", "0");

    lxw_xml_empty_tag(self->file, "xda:dynamicArrayProperties", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <ext> element.
 */
STATIC void
_metadata_write_ext(lxw_metadata *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("uri", "{bdbb8cdc-fa1e-496e-a857-3c3f30c029c3}");

    lxw_xml_start_tag(self->file, "ext", &attributes);

    /* Write the xda:dynamicArrayProperties element. */
    _metadata_write_xda_dynamic_array_properties(self);

    lxw_xml_end_tag(self->file, "ext");

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <futureMetadata> element.
 */
STATIC void
_metadata_write_future_metadata(lxw_metadata *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("name", "XLDAPR");
    LXW_PUSH_ATTRIBUTES_INT("count", 1);

    lxw_xml_start_tag(self->file, "futureMetadata", &attributes);

    lxw_xml_start_tag(self->file, "bk", NULL);

    lxw_xml_start_tag(self->file, "extLst", NULL);

    /* Write the ext element. */
    _metadata_write_ext(self);

    lxw_xml_end_tag(self->file, "extLst");

    lxw_xml_end_tag(self->file, "bk");

    lxw_xml_end_tag(self->file, "futureMetadata");

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <rc> element.
 */
STATIC void
_metadata_write_rc(lxw_metadata *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("t", "1");
    LXW_PUSH_ATTRIBUTES_STR("v", "0");

    lxw_xml_empty_tag(self->file, "rc", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <cellMetadata> element.
 */
STATIC void
_metadata_write_cell_metadata(lxw_metadata *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("count", "1");

    lxw_xml_start_tag(self->file, "cellMetadata", &attributes);

    lxw_xml_start_tag(self->file, "bk", NULL);

    /* Write the rc element. */
    _metadata_write_rc(self);

    lxw_xml_end_tag(self->file, "bk");

    lxw_xml_end_tag(self->file, "cellMetadata");

    LXW_FREE_ATTRIBUTES();
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
lxw_metadata_assemble_xml_file(lxw_metadata *self)
{
    /* Write the XML declaration. */
    _metadata_xml_declaration(self);

    /* Write the metadata element. */
    _metadata_write_metadata(self);

    /* Write the metadataTypes element. */
    _metadata_write_metadata_types(self);

    /* Write the futureMetadata element. */
    _metadata_write_future_metadata(self);

    /* Write the cellMetadata element. */
    _metadata_write_cell_metadata(self);

    lxw_xml_end_tag(self->file, "metadata");
}
