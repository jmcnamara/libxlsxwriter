/*****************************************************************************
 * shared_strings - A library for creating Excel XLSX sst files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/shared_strings.h"
#include "xlsxwriter/utility.h"
#include <ctype.h>

/*
 * Forward declarations.
 */

STATIC int _element_cmp(struct sst_element *element1,
    struct sst_element *element2);

LXW_RB_GENERATE_ELEMENT(sst_rb_tree, sst_element, sst_tree_pointers,
    _element_cmp);

/*****************************************************************************
 *
 * Private functions.
 *
 ****************************************************************************/

/*
 * Create a new SST SharedString object.
 */
lxw_sst *
_new_sst()
{
    /* Create the new shared string table. */
    lxw_sst *sst = calloc(1, sizeof(lxw_sst));
    RETURN_ON_MEM_ERROR(sst, NULL);

    /* Add the sst RB tree. */
    sst->rb_tree = calloc(1, sizeof(struct sst_rb_tree));
    GOTO_LABEL_ON_MEM_ERROR(sst->rb_tree, mem_error1);

    /* Add a list for tracking the insertion order. */
    sst->order_list = calloc(1, sizeof(struct sst_order_list));
    GOTO_LABEL_ON_MEM_ERROR(sst->order_list, mem_error2);

    /* Initialise the order list. */
    STAILQ_INIT(sst->order_list);

    /* Initialize the RB tree. */
    RB_INIT(sst->rb_tree);

    return sst;

mem_error2:
    free(sst->rb_tree);

mem_error1:
    free(sst);

    return NULL;
}

/*
 * Free a SST SharedString table object.
 */
void
_free_sst(lxw_sst *sst)
{
    struct sst_element *sst_element;
    struct sst_element *sst_element_temp;

    if (!sst)
        return;

    /* Free the sst_elements and their data using the ordered linked list. */
    STAILQ_FOREACH_SAFE(sst_element, sst->order_list, sst_order_pointers,
                        sst_element_temp) {
        if (sst_element && sst_element->string)
            free(sst_element->string);
        if (sst_element)
            free(sst_element);
    }

    free(sst->order_list);
    free(sst->rb_tree);
    free(sst);
}

/*
 * Comparator for the element structure
 */
STATIC int
_element_cmp(struct sst_element *element1, struct sst_element *element2)
{
    return strcmp(element1->string, element2->string);
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
_sst_xml_declaration(lxw_sst *self)
{
    _xml_declaration(self->file);
}

/*
 * Write the <t> element.
 */
void
_write_t(lxw_sst *self, char *string)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    _INIT_ATTRIBUTES();

    /* Add attribute to preserve leading or trailing whitespace. */
    if (isspace((unsigned char) string[0])
        || isspace((unsigned char) string[strlen(string) - 1]))
        _PUSH_ATTRIBUTES_STR("xml:space", "preserve");

    _xml_data_element(self->file, "t", string, &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <si> element.
 */
void
_write_si(lxw_sst *self, char *string)
{
    _xml_start_tag(self->file, "si", NULL);

    /* Write the t element. */
    _write_t(self, string);

    _xml_end_tag(self->file, "si");
}

/*
 * Write the <sst> element.
 */
void
_write_sst(lxw_sst *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char xmlns[] =
        "http://schemas.openxmlformats.org/spreadsheetml/2006/main";

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("xmlns", xmlns);
    _PUSH_ATTRIBUTES_INT("count", self->string_count);
    _PUSH_ATTRIBUTES_INT("uniqueCount", self->unique_count);

    _xml_start_tag(self->file, "sst", &attributes);

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
_write_sst_strings(lxw_sst *self)
{
    struct sst_element *sst_element;

    STAILQ_FOREACH(sst_element, self->order_list, sst_order_pointers) {
        /* Write the si element. */
        _write_si(self, sst_element->string);
    }
}

/*
 * Assemble and write the XML file.
 */
void
_sst_assemble_xml_file(lxw_sst *self)
{
    /* Write the XML declaration. */
    _sst_xml_declaration(self);

    /* Write the sst element. */
    _write_sst(self);

    /* Write the sst strings. */
    _write_sst_strings(self);

    /* Close the sst tag. */
    _xml_end_tag(self->file, "sst");
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/
/*
 * Add to or find a string in the SST SharedString table and return it's index.
 */
int32_t
_get_sst_index(lxw_sst *sst, const char *string)
{
    struct sst_element *element;
    struct sst_element *existing_element;

    /* Create an sst element to potentially add to the table. */
    element = calloc(1, sizeof(struct sst_element));
    if (!element)
        return -1;

    /* Create potential new element with the string and its index. */
    element->index = sst->unique_count;
    element->string = lxw_strdup(string);

    /* Try to insert it and see whether we already have that string. */
    existing_element = RB_INSERT(sst_rb_tree, sst->rb_tree, element);

    /* If existing_element is not NULL, then it already existed. */
    /* Free new created element. */
    if (existing_element) {
        free(element->string);
        free(element);
        sst->string_count++;
        return existing_element->index;
    }

    /* If it didn't exist, also add it to the insertion order linked list. */
    STAILQ_INSERT_TAIL(sst->order_list, element, sst_order_pointers);

    /* Update SST string counts. */
    sst->string_count++;
    sst->unique_count++;
    return element->index;
}
