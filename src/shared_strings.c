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

/*****************************************************************************
 *
 * Private functions.
 *
 ****************************************************************************/

/*
 * Create a new SST SharedString hash table object.
 */
lxw_sst *
_new_sst()
{
    /* Create the new hash table. */
    lxw_sst *sst = calloc(1, sizeof(lxw_sst));
    RETURN_ON_MEM_ERROR(sst, NULL);

    /* Add the sst element buckets. */
    sst->buckets = calloc(NUM_SST_BUCKETS, sizeof(struct sst_bucket_list *));
    GOTO_LABEL_ON_MEM_ERROR(sst->buckets, mem_error1);

    /* Add a list for tracking the insertion order. */
    sst->order_list = calloc(1, sizeof(struct sst_order_list));
    GOTO_LABEL_ON_MEM_ERROR(sst->order_list, mem_error2);

    /* Initialise the order list. */
    STAILQ_INIT(sst->order_list);

    /* Store the number of buckets to calculate the load factor. */
    sst->num_buckets = NUM_SST_BUCKETS;

    return sst;

mem_error2:
    free(sst->order_list);

mem_error1:
    free(sst);

    return NULL;
}

/*
 * Free a SST SharedString hash table object.
 */
void
_free_sst(lxw_sst *sst)
{
    size_t i;
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

    /* Free the buckets from the hash table. */
    for (i = 0; i < sst->num_buckets; i++) {
        if (sst->buckets[i])
            free(sst->buckets[i]);
    }

    free(sst->order_list);
    free(sst->buckets);
    free(sst);
}

/*
 * FNV hash function for SST table string keys. See:
 * http://en.wikipedia.org/wiki/Fowler-Noll-Vo_hash_function
 */
size_t
_generate_sst_hash_key(const char *string)
{
    size_t string_len = strlen(string);
    size_t hash = 2166136261;
    size_t i;

    for (i = 0; i < string_len; i++)
        hash = (hash * 16777619) ^ (unsigned char) string[i];

    return hash % NUM_SST_BUCKETS;
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
    size_t hash_key = _generate_sst_hash_key(string);
    struct sst_bucket_list *list = NULL;
    struct sst_element *element = NULL;

    if (!sst->buckets[hash_key]) {
        /* The string isn't in the SST SharedString hash table. */

        /* Create a linked list in the bucket to hold the sst strings. */
        list = calloc(1, sizeof(struct sst_bucket_list));
        GOTO_LABEL_ON_MEM_ERROR(list, mem_error1);

        /* Initialise the bucket linked list. */
        SLIST_INIT(list);

        /* Create an sst element to add to the linked list. */
        element = calloc(1, sizeof(struct sst_element));
        GOTO_LABEL_ON_MEM_ERROR(element, mem_error1);

        /* Store the string and its index. */
        element->index = sst->unique_count;
        element->string = lxw_strdup(string);

        /* Add the sst element to the bucket linked list. */
        SLIST_INSERT_HEAD(list, element, sst_list_pointers);

        /* Also add it to the insertion order linked list. */
        STAILQ_INSERT_TAIL(sst->order_list, element, sst_order_pointers);

        /* Store the bucket list at the hash index. */
        sst->buckets[hash_key] = list;

        /* Update the bucket and SST string counts. */
        sst->used_buckets++;
        sst->string_count++;
        sst->unique_count++;

        return element->index;
    }
    else {
        /* The sting is already in the table or there is a hash collision. */
        list = sst->buckets[hash_key];

        /* Iterate over the strings in the bucket's linked list. */
        SLIST_FOREACH(element, list, sst_list_pointers) {
            if (strcmp(element->string, string) == 0) {
                /* The string already exists in the table. Update the
                 * non-unique string count and return the index. */
                sst->string_count++;
                return element->index;
            }
        }

        /* String doesn't exist in the list so this is a hash collision.
         * Create an sst element to add to the linked list. */
        element = calloc(1, sizeof(struct sst_element));
        GOTO_LABEL_ON_MEM_ERROR(element, mem_error2);

        /* Store the string and its index. */
        element->index = sst->unique_count;
        element->string = lxw_strdup(string);

        /* Add the sst element to the bucket linked list. */
        SLIST_INSERT_HEAD(list, element, sst_list_pointers);

        /* Also add it to the insertion order linked list. */
        STAILQ_INSERT_TAIL(sst->order_list, element, sst_order_pointers);

        /* Update the SST string counts. */
        sst->string_count++;
        sst->unique_count++;

        return element->index;
    }

mem_error1:
    free(list);

mem_error2:
    free(element);
    return -1;
}
