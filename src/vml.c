/*****************************************************************************
 * vml - A library for creating Excel XLSX vml files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2017, Xyato, coszyx@gmail.com. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/vml.h"
#include "xlsxwriter/utility.h"
#include <ctype.h>

/*****************************************************************************
 *
 * XML functions.
 *
 ****************************************************************************/
STATIC void
_write_xml_namespace(lxw_vml *self)
{
	struct xml_attribute_list attributes;
	struct xml_attribute *attribute;

	LXW_INIT_ATTRIBUTES();
	LXW_PUSH_ATTRIBUTES_STR("xmlns:v", "urn:schemas-microsoft-com:vml");
	LXW_PUSH_ATTRIBUTES_STR("xmlns:o", "urn:schemas-microsoft-com:office:office");
	LXW_PUSH_ATTRIBUTES_STR("xmlns:x", "urn:schemas-microsoft-com:office:excel");

	lxw_xml_start_tag(self->file, "xml", &attributes);

	LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_idmap(lxw_vml *self, char *data_id_str)
{
	struct xml_attribute_list attributes;
	struct xml_attribute *attribute;

	LXW_INIT_ATTRIBUTES();
	LXW_PUSH_ATTRIBUTES_STR("v:ext", "edit");
	LXW_PUSH_ATTRIBUTES_STR("data", data_id_str);

	lxw_xml_empty_tag(self->file, "o:idmap", &attributes);

	LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_shapelayout(lxw_vml *self, char *data_id_str)
{
	struct xml_attribute_list attributes;
	struct xml_attribute *attribute;

	LXW_INIT_ATTRIBUTES();
	LXW_PUSH_ATTRIBUTES_STR("v:ext", "edit");

	lxw_xml_start_tag(self->file, "o:shapelayout", &attributes);

	_write_idmap(self, data_id_str);

	lxw_xml_end_tag(self->file, "o:shapelayout");

	LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_stroke(lxw_vml *self)
{
	struct xml_attribute_list attributes;
	struct xml_attribute *attribute;

	LXW_INIT_ATTRIBUTES();
	LXW_PUSH_ATTRIBUTES_STR("joinstyle", "miter");

	lxw_xml_empty_tag(self->file, "v:stroke", &attributes);

	LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_comment_path(lxw_vml *self, char *gradientshapeok, char *connecttype)
{
	struct xml_attribute_list attributes;
	struct xml_attribute *attribute;

	LXW_INIT_ATTRIBUTES();

	if (gradientshapeok != NULL) {
		LXW_PUSH_ATTRIBUTES_STR("gradientshapeok", "t");
	}
	
	LXW_PUSH_ATTRIBUTES_STR("o:connecttype", connecttype);

	lxw_xml_empty_tag(self->file, "v:path", &attributes);

	LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_comment_shapetype(lxw_vml *self)
{
	struct xml_attribute_list attributes;
	struct xml_attribute *attribute;

	LXW_INIT_ATTRIBUTES();
	LXW_PUSH_ATTRIBUTES_STR("id", "_x0000_t202");
	LXW_PUSH_ATTRIBUTES_STR("coordsize", "21600,21600");
	LXW_PUSH_ATTRIBUTES_INT("o:spt", 202);
	LXW_PUSH_ATTRIBUTES_STR("path", "m,l,21600r21600,l21600,xe");

	lxw_xml_start_tag(self->file, "v:shapetype", &attributes);

	_write_stroke(self);

	_write_comment_path(self, "t", "rect");

	lxw_xml_end_tag(self->file, "v:shapetype");

	LXW_FREE_ATTRIBUTES();
}

/*
 * Assemble and write the XML file.
 */
void
lxw_vml_assemble_xml_file(lxw_vml *self, lxw_worksheet *worksheet)
{
	_write_xml_namespace(self);

	_write_shapelayout(self, worksheet->vml_data_id_str);

	if (worksheet->comment_count > 0) {
		_write_comment_shapetype(self);

		lxw_comment_write_shape(worksheet->comment, self->file, worksheet->vml_shape_id);
	}
	
	lxw_xml_end_tag(self->file, "xml");
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/
void
lxw_vml_free(lxw_vml *vml)
{
	free(vml);
}

lxw_vml * 
lxw_vml_new()
{
	lxw_vml *vml = calloc(1, sizeof(lxw_vml));
	return vml;
}
