/*
 * libxlsxwriter
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 * feature_property_bag - A libxlsxwriter library for creating Excel XLSX
 *                        featurePropertyBag files.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/feature_property_bag.h"
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
 * Create a new feature_property_bag object.
 */
lxw_feature_property_bag *
lxw_feature_property_bag_new(void)
{
    lxw_feature_property_bag *feature_property_bag =
        calloc(1, sizeof(lxw_feature_property_bag));
    GOTO_LABEL_ON_MEM_ERROR(feature_property_bag, mem_error);

    return feature_property_bag;

mem_error:
    lxw_feature_property_bag_free(feature_property_bag);
    return NULL;
}

/*
 * Free a feature_property_bag object.
 */
void
lxw_feature_property_bag_free(lxw_feature_property_bag *feature_property_bag)
{
    if (!feature_property_bag)
        return;

    free(feature_property_bag);
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
_feature_property_bag_xml_declaration(lxw_feature_property_bag *self)
{
    lxw_xml_declaration(self->file);
}

/*
 * Write the <FeaturePropertyBags> element.
 */
STATIC void
_write_feature_property_bags(lxw_feature_property_bag *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("xmlns",
                            "http://schemas.microsoft.com/office/spreadsheetml/2022/featurepropertybag");

    lxw_xml_start_tag(self->file, "FeaturePropertyBags", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the Checkbox <bag> element.
 */
STATIC void
_write_checkbox_bag(lxw_feature_property_bag *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("type", "Checkbox");

    lxw_xml_empty_tag(self->file, "bag", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <bagId> element.
 */
STATIC void
_write_bag_id(lxw_feature_property_bag *self, const char *key, int id)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char id_str[LXW_UINT32_T_LENGTH];

    lxw_snprintf(id_str, LXW_UINT32_T_LENGTH, "%d", id);

    LXW_INIT_ATTRIBUTES();
    if (key && key[0] != '\0')
        LXW_PUSH_ATTRIBUTES_STR("k", key);

    lxw_xml_data_element(self->file, "bagId", id_str, &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the XFControls <bag> element.
 */
STATIC void
_write_xf_controls_bag(lxw_feature_property_bag *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("type", "XFControls");

    lxw_xml_start_tag(self->file, "bag", &attributes);

    _write_bag_id(self, "CellControl", 0);

    lxw_xml_end_tag(self->file, "bag");

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the XFComplement <bag> element.
 */
STATIC void
_write_xf_complement_bag(lxw_feature_property_bag *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("type", "XFComplement");

    lxw_xml_start_tag(self->file, "bag", &attributes);

    _write_bag_id(self, "XFControls", 1);

    lxw_xml_end_tag(self->file, "bag");

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the XFComplements <bag> element.
 */
STATIC void
_write_xf_complements_bag(lxw_feature_property_bag *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("type", "XFComplements");
    LXW_PUSH_ATTRIBUTES_STR("extRef", "XFComplementsMapperExtRef");

    lxw_xml_start_tag(self->file, "bag", &attributes);

    LXW_FREE_ATTRIBUTES();

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("k", "MappedFeaturePropertyBags");
    lxw_xml_start_tag(self->file, "a", &attributes);
    LXW_FREE_ATTRIBUTES();

    _write_bag_id(self, NULL, 2);

    lxw_xml_end_tag(self->file, "a");
    lxw_xml_end_tag(self->file, "bag");
}

/*
 * Write the DXFComplements <bag> element.
 */
STATIC void
_write_dxf_complements_bag(lxw_feature_property_bag *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("type", "DXFComplements");
    LXW_PUSH_ATTRIBUTES_STR("extRef", "DXFComplementsMapperExtRef");

    lxw_xml_start_tag(self->file, "bag", &attributes);

    LXW_FREE_ATTRIBUTES();

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("k", "MappedFeaturePropertyBags");
    lxw_xml_start_tag(self->file, "a", &attributes);
    LXW_FREE_ATTRIBUTES();

    _write_bag_id(self, NULL, 2);

    lxw_xml_end_tag(self->file, "a");
    lxw_xml_end_tag(self->file, "bag");
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
lxw_feature_property_bag_assemble_xml_file(lxw_feature_property_bag *self)
{
    /* Write the XML declaration. */
    _feature_property_bag_xml_declaration(self);

    /* Write the FeaturePropertyBags element. */
    _write_feature_property_bags(self);

    /* Write the Checkbox bag element. */
    _write_checkbox_bag(self);

    /* Write the XFControls bag element. */
    _write_xf_controls_bag(self);

    /* Write the XFComplement bag element. */
    _write_xf_complement_bag(self);

    /* Write the XFComplements bag element. */
    _write_xf_complements_bag(self);

    /* Write the DXFComplements bag element if required. */
    if (self->has_dxf_complements)
        _write_dxf_complements_bag(self);

    lxw_xml_end_tag(self->file, "FeaturePropertyBags");
}
