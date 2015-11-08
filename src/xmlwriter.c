/*****************************************************************************
 * xmlwriter - A base library for libxlsxwriter libraries.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xlsxwriter/xmlwriter.h"

/* Defines. */
#define MAX_ENCODED_ATTRIBUTE_LENGTH (MAX_ATTRIBUTE_LENGTH*6)

/* Forward declarations. */
char *_escape_attributes(struct xml_attribute *attribute);

char *_escape_data(const char *data);

void _fprint_escaped_attributes(FILE * xmlfile,
                                struct xml_attribute_list *attributes);

void _fprint_escaped_data(FILE * xmlfile, const char *data);

/*
 * Write the XML declaration.
 */
void
_xml_declaration(FILE * xmlfile)
{
    fprintf(xmlfile, "<?xml version=\"1.0\" "
            "encoding=\"UTF-8\" standalone=\"yes\"?>\n");
}

/*
 * Write an XML start tag with optional attributes.
 */
void
_xml_start_tag(FILE * xmlfile,
               const char *tag, struct xml_attribute_list *attributes)
{
    fprintf(xmlfile, "<%s", tag);

    _fprint_escaped_attributes(xmlfile, attributes);

    fprintf(xmlfile, ">");
}

/*
 * Write an XML start tag with optional, unencoded, attributes.
 * This is a minor speed optimisation for elements that don't need encoding.
 */
void
_xml_start_tag_unencoded(FILE * xmlfile,
                         const char *tag,
                         struct xml_attribute_list *attributes)
{
    struct xml_attribute *attribute;

    fprintf(xmlfile, "<%s", tag);

    if (attributes) {
        STAILQ_FOREACH(attribute, attributes, list_entries) {
            fprintf(xmlfile, " %s=\"%s\"", attribute->key, attribute->value);
        }
    }

    fprintf(xmlfile, ">");
}

/*
 * Write an XML end tag.
 */
void
_xml_end_tag(FILE * xmlfile, const char *tag)
{
    fprintf(xmlfile, "</%s>", tag);
}

/*
 * Write an empty XML tag with optional attributes.
 */
void
_xml_empty_tag(FILE * xmlfile,
               const char *tag, struct xml_attribute_list *attributes)
{
    fprintf(xmlfile, "<%s", tag);

    _fprint_escaped_attributes(xmlfile, attributes);

    fprintf(xmlfile, "/>");
}

/*
 * Write an XML start tag with optional, unencoded, attributes.
 * This is a minor speed optimisation for elements that don't need encoding.
 */
void
_xml_empty_tag_unencoded(FILE * xmlfile,
                         const char *tag,
                         struct xml_attribute_list *attributes)
{
    struct xml_attribute *attribute;

    fprintf(xmlfile, "<%s", tag);

    if (attributes) {
        STAILQ_FOREACH(attribute, attributes, list_entries) {
            fprintf(xmlfile, " %s=\"%s\"", attribute->key, attribute->value);
        }
    }

    fprintf(xmlfile, "/>");
}

/*
 * Write an XML element containing data with optional attributes.
 */
void
_xml_data_element(FILE * xmlfile,
                  const char *tag,
                  const char *data, struct xml_attribute_list *attributes)
{
    fprintf(xmlfile, "<%s", tag);

    _fprint_escaped_attributes(xmlfile, attributes);

    fprintf(xmlfile, ">");

    _fprint_escaped_data(xmlfile, data);

    fprintf(xmlfile, "</%s>", tag);
}

/*
 * Escape XML characters in attributes.
 */
char *
_escape_attributes(struct xml_attribute *attribute)
{
    char *encoded = (char *) calloc(MAX_ENCODED_ATTRIBUTE_LENGTH, 1);
    char *p_encoded = encoded;
    char *p_attr = attribute->value;

    while (*p_attr) {
        switch (*p_attr) {
            case '&':
                strcat(p_encoded, "&amp;");
                p_encoded += 5;
                break;
            case '<':
                strcat(p_encoded, "&lt;");
                p_encoded += 4;
                break;
            case '>':
                strcat(p_encoded, "&gt;");
                p_encoded += 4;
                break;
            case '"':
                strcat(p_encoded, "&quot;");
                p_encoded += 6;
                break;
            default:
                *p_encoded = *p_attr;
                p_encoded++;
                break;
        }
        p_attr++;
    }

    return encoded;
}

/*
 * Escape XML characters in data sections of tags.
 * Note, this is different from _escape_attributes()
 * in that double quotes are not escaped by Excel.
 */
char *
_escape_data(const char *data)
{
    size_t encoded_len = (strlen(data) * 5 + 1);

    char *encoded = (char *) calloc(encoded_len, 1);
    char *p_encoded = encoded;

    while (*data) {
        switch (*data) {
            case '&':
                strcat(p_encoded, "&amp;");
                p_encoded += 5;
                break;
            case '<':
                strcat(p_encoded, "&lt;");
                p_encoded += 4;
                break;
            case '>':
                strcat(p_encoded, "&gt;");
                p_encoded += 4;
                break;
            default:
                *p_encoded = *data;
                p_encoded++;
                break;
        }
        data++;
    }

    return encoded;
}

/* TODO */
void
_fprint_escaped_attributes(FILE * xmlfile,
                           struct xml_attribute_list *attributes)
{
    struct xml_attribute *attribute;

    if (attributes) {
        STAILQ_FOREACH(attribute, attributes, list_entries) {
            fprintf(xmlfile, " %s=", attribute->key);

            if (!strpbrk(attribute->value, "&<>\"")) {
                fprintf(xmlfile, "\"%s\"", attribute->value);
            }
            else {
                char *encoded = _escape_attributes(attribute);

                if (encoded) {
                    fprintf(xmlfile, "\"%s\"", encoded);

                    free(encoded);
                }
            }
        }
    }
}

/* TODO */
void
_fprint_escaped_data(FILE * xmlfile, const char *data)
{
    /* Escape the data section of the XML element. */
    if (!strpbrk(data, "&<>")) {
        fprintf(xmlfile, "%s", data);
    }
    else {
        char *encoded = _escape_data(data);
        if (encoded) {
            fprintf(xmlfile, "%s", encoded);
            free(encoded);
        }
    }
}

/* TODO */
struct xml_attribute *
_new_attribute_str(const char *key, const char *value)
{
    struct xml_attribute *attribute = malloc(sizeof(struct xml_attribute));

    LXW_ATTRIBUTE_COPY(attribute->key, key);
    LXW_ATTRIBUTE_COPY(attribute->value, value);

    return attribute;
}

/* TODO */
struct xml_attribute *
_new_attribute_int(const char *key, uint32_t value)
{
    struct xml_attribute *attribute = malloc(sizeof(struct xml_attribute));

    LXW_ATTRIBUTE_COPY(attribute->key, key);
    lxw_snprintf(attribute->value, MAX_ATTRIBUTE_LENGTH, "%d", value);

    return attribute;
}

/* TODO */
struct xml_attribute *
_new_attribute_dbl(const char *key, double value)
{
    struct xml_attribute *attribute = malloc(sizeof(struct xml_attribute));

    LXW_ATTRIBUTE_COPY(attribute->key, key);
    lxw_snprintf(attribute->value, MAX_ATTRIBUTE_LENGTH, "%.16g", value);

    return attribute;
}
