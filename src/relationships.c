/*****************************************************************************
 * relationships - A library for creating Excel XLSX relationships files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include <string.h>
#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/relationships.h"
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
 * Create a new relationships object.
 */
lxw_relationships *
_new_relationships()
{
    lxw_relationships *rels = calloc(1, sizeof(lxw_relationships));
    GOTO_LABEL_ON_MEM_ERROR(rels, mem_error);

    rels->relationships = calloc(1, sizeof(struct lxw_rel_tuples));
    GOTO_LABEL_ON_MEM_ERROR(rels->relationships, mem_error);
    STAILQ_INIT(rels->relationships);

    return rels;

mem_error:
    _free_relationships(rels);
    return NULL;
}

/*
 * Free a relationships object.
 */
void
_free_relationships(lxw_relationships *rels)
{
    lxw_rel_tuple *relationship;

    if (!rels)
        return;

    while (!STAILQ_EMPTY(rels->relationships)) {
        relationship = STAILQ_FIRST(rels->relationships);
        STAILQ_REMOVE_HEAD(rels->relationships, list_pointers);
        free(relationship->type);
        free(relationship->target);
        free(relationship->target_mode);
        free(relationship);
    }

    free(rels->relationships);
    free(rels);
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
_relationships_xml_declaration(lxw_relationships *self)
{
    _xml_declaration(self->file);
}

/*
 * Write the <Relationship> element.
 */
STATIC void
_write_relationship(lxw_relationships *self, const char *type,
                    const char *target, const char *target_mode)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char r_id[MAX_ATTRIBUTE_LENGTH] = { 0 };

    self->rel_id++;
    lxw_snprintf(r_id, ATTR_32, "rId%d", self->rel_id);

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("Id", r_id);
    _PUSH_ATTRIBUTES_STR("Type", type);
    _PUSH_ATTRIBUTES_STR("Target", target);

    if (target_mode)
        _PUSH_ATTRIBUTES_STR("TargetMode", target_mode);

    _xml_empty_tag(self->file, "Relationship", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <Relationships> element.
 */
STATIC void
_write_relationships(lxw_relationships *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    lxw_rel_tuple *rel;

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("xmlns", LXW_PACKAGE_SCHEMA);

    _xml_start_tag(self->file, "Relationships", &attributes);

    STAILQ_FOREACH(rel, self->relationships, list_pointers) {
        _write_relationship(self, rel->type, rel->target, rel->target_mode);
    }

    _FREE_ATTRIBUTES();
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
_relationships_assemble_xml_file(lxw_relationships *self)
{
    /* Write the XML declaration. */
    _relationships_xml_declaration(self);

    _write_relationships(self);

    /* Close the relationships tag. */
    _xml_end_tag(self->file, "Relationships");
}

/*
 * Add a generic container relationship to XLSX .rels xml files.
 */
STATIC void
_add_relationship(lxw_relationships *self, const char *schema,
                  const char *type, const char *target,
                  const char *target_mode)
{
    lxw_rel_tuple *relationship;

    if (!schema || !type || !target)
        return;

    relationship = calloc(1, sizeof(lxw_rel_tuple));
    GOTO_LABEL_ON_MEM_ERROR(relationship, mem_error);

    relationship->type = calloc(1, MAX_ATTRIBUTE_LENGTH);
    GOTO_LABEL_ON_MEM_ERROR(relationship->type, mem_error);

    /* Add the schema to the relationship type. */
    lxw_snprintf(relationship->type, MAX_ATTRIBUTE_LENGTH, "%s%s",
                 schema, type);

    relationship->target = lxw_strdup(target);
    GOTO_LABEL_ON_MEM_ERROR(relationship->target, mem_error);

    if (target_mode) {
        relationship->target_mode = lxw_strdup(target_mode);
        GOTO_LABEL_ON_MEM_ERROR(relationship->target_mode, mem_error);
    }

    STAILQ_INSERT_TAIL(self->relationships, relationship, list_pointers);

    return;

mem_error:
    if (relationship) {
        free(relationship->type);
        free(relationship->target);
        free(relationship->target_mode);
        free(relationship);
    }
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/

/*
 * Add a document relationship to XLSX .rels xml files.
 */
void
_add_document_relationship(lxw_relationships *self, const char *type,
                           const char *target)
{
    _add_relationship(self, LXW_DOCUMENT_SCHEMA, type, target, NULL);
}

/*
 * Add a package relationship to XLSX .rels xml files.
 */
void
_add_package_relationship(lxw_relationships *self, const char *type,
                          const char *target)
{
    _add_relationship(self, LXW_PACKAGE_SCHEMA, type, target, NULL);
}

/*
 * Add a MS schema package relationship to XLSX .rels xml files.
 */
void
_add_ms_package_relationship(lxw_relationships *self, const char *type,
                             const char *target)
{
    _add_relationship(self, LXW_MS_SCHEMA, type, target, NULL);
}

/*
 * Add a worksheet relationship to sheet .rels xml files.
 */
void
_add_worksheet_relationship(lxw_relationships *self, const char *type,
                            const char *target, const char *target_mode)
{
    _add_relationship(self, LXW_DOCUMENT_SCHEMA, type, target, target_mode);
}
