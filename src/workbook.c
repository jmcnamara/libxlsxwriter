/*****************************************************************************
 * workbook - A library for creating Excel XLSX workbook files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/workbook.h"
#include "xlsxwriter/utility.h"
#include "xlsxwriter/packager.h"
#include "xlsxwriter/hash_table.h"

/*
 * Forward declarations.
 */

/*****************************************************************************
 *
 * Private functions.
 *
 ****************************************************************************/

/*
 * Free a workbook object.
 */
void
_free_workbook(lxw_workbook *workbook)
{
    lxw_worksheet *worksheet;
    lxw_format *format;

    if (!workbook)
        return;

    if (workbook->properties) {
        free(workbook->properties->title);
        free(workbook->properties->subject);
        free(workbook->properties->author);
        free(workbook->properties->manager);
        free(workbook->properties->company);
        free(workbook->properties->category);
        free(workbook->properties->keywords);
        free(workbook->properties->comments);
        free(workbook->properties->status);
    }

    free(workbook->properties);

    /* Free the worksheets in the workbook. */
    while (!STAILQ_EMPTY(workbook->worksheets)) {
        worksheet = STAILQ_FIRST(workbook->worksheets);
        STAILQ_REMOVE_HEAD(workbook->worksheets, list_pointers);
        _free_worksheet(worksheet);
    }

    /* Free the formats in the workbook. */
    while (!STAILQ_EMPTY(workbook->formats)) {
        format = STAILQ_FIRST(workbook->formats);
        STAILQ_REMOVE_HEAD(workbook->formats, list_pointers);
        _free_format(format);
    }

    _free_lxw_hash(workbook->xf_format_indices);
    _free_sst(workbook->sst);
    free(workbook->worksheets);
    free(workbook->formats);
    free(workbook);
}

/*
 * Set the default index for each format. This is only used for testing.
 */
void
_set_default_xf_indices(lxw_workbook *self)
{
    lxw_format *format;

    STAILQ_FOREACH(format, self->formats, list_pointers) {
        _get_xf_index(format);
    }
}

/*
 * Iterate through the XF Format objects and give them an index to non-default
 * font elements.
 */
STATIC void
_prepare_fonts(lxw_workbook *self)
{

    lxw_hash_table *fonts = _new_lxw_hash(128, 1, 1);
    lxw_hash_element *hash_element;
    lxw_format *format;
    uint16_t index = 0;

    STAILQ_FOREACH(format, self->formats, list_pointers) {
        lxw_font *key = _get_font_key(format);

        if (key) {
            /* Look up the format in the hash table. */
            hash_element = _hash_key_exists(fonts, key, sizeof(lxw_font));

            if (hash_element) {
                /* Font has already been used. */
                format->font_index = *(uint16_t *) hash_element->value;
                format->has_font = LXW_FALSE;
                free(key);
            }
            else {
                /* This is a new font. */
                uint16_t *font_index = calloc(1, sizeof(uint16_t));
                *font_index = index;
                format->font_index = index;
                format->has_font = 1;
                _insert_hash_element(fonts, key, font_index,
                                     sizeof(lxw_font));
                index++;
            }
        }
    }

    _free_lxw_hash(fonts);

    self->font_count = index;
}

/*
 * Iterate through the XF Format objects and give them an index to non-default
 * border elements.
 */
STATIC void
_prepare_borders(lxw_workbook *self)
{

    lxw_hash_table *borders = _new_lxw_hash(128, 1, 1);
    lxw_hash_element *hash_element;
    lxw_format *format;
    uint16_t index = 0;

    STAILQ_FOREACH(format, self->formats, list_pointers) {
        lxw_border *key = _get_border_key(format);

        if (key) {
            /* Look up the format in the hash table. */
            hash_element = _hash_key_exists(borders, key, sizeof(lxw_border));

            if (hash_element) {
                /* Border has already been used. */
                format->border_index = *(uint16_t *) hash_element->value;
                format->has_border = LXW_FALSE;
                free(key);
            }
            else {
                /* This is a new border. */
                uint16_t *border_index = calloc(1, sizeof(uint16_t));
                *border_index = index;
                format->border_index = index;
                format->has_border = 1;
                _insert_hash_element(borders, key, border_index,
                                     sizeof(lxw_border));
                index++;
            }
        }
    }

    _free_lxw_hash(borders);

    self->border_count = index;
}

/*
 * Iterate through the XF Format objects and give them an index to non-default
 * fill elements.
 */
STATIC void
_prepare_fills(lxw_workbook *self)
{

    lxw_hash_table *fills = _new_lxw_hash(128, 1, 1);
    lxw_hash_element *hash_element;
    lxw_format *format;
    uint16_t index = 2;
    lxw_fill *default_fill_1 = calloc(1, sizeof(lxw_fill));
    lxw_fill *default_fill_2 = calloc(1, sizeof(lxw_fill));
    uint16_t *fill_index1 = calloc(1, sizeof(uint16_t));
    uint16_t *fill_index2 = calloc(1, sizeof(uint16_t));

    if (!default_fill_1 || !default_fill_2 || !fill_index1 || !fill_index2) {
        MEM_ERROR();
        return;
    }

    /* Add the default fills. */
    default_fill_1->pattern = LXW_PATTERN_NONE;
    default_fill_1->fg_color = LXW_COLOR_UNSET;
    default_fill_1->bg_color = LXW_COLOR_UNSET;
    *fill_index1 = 0;
    _insert_hash_element(fills, default_fill_1, fill_index1,
                         sizeof(lxw_fill));

    default_fill_2->pattern = LXW_PATTERN_GRAY_125;
    default_fill_2->fg_color = LXW_COLOR_UNSET;
    default_fill_2->bg_color = LXW_COLOR_UNSET;
    *fill_index2 = 1;
    _insert_hash_element(fills, default_fill_2, fill_index2,
                         sizeof(lxw_fill));

    STAILQ_FOREACH(format, self->formats, list_pointers) {
        lxw_fill *key = _get_fill_key(format);

        /* The following logical statements jointly take care of special */
        /* cases in relation to cell colours and patterns:               */
        /* 1. For a solid fill (pattern == 1) Excel reverses the role of */
        /*    foreground and background colours, and                     */
        /* 2. If the user specifies a foreground or background colour    */
        /*    withouth a pattern they probably wanted a solid fill, so   */
        /*    we fill in the defaults.                                   */
        if (format->pattern == LXW_PATTERN_SOLID
            && format->bg_color != LXW_COLOR_UNSET
            && format->fg_color != LXW_COLOR_UNSET) {
            lxw_color_t tmp = format->fg_color;
            format->fg_color = format->bg_color;
            format->bg_color = tmp;
        }

        if (format->pattern <= LXW_PATTERN_SOLID
            && format->bg_color != LXW_COLOR_UNSET
            && format->fg_color == LXW_COLOR_UNSET) {
            format->fg_color = format->bg_color;
            format->bg_color = LXW_COLOR_UNSET;
            format->pattern = LXW_PATTERN_SOLID;
        }

        if (format->pattern <= LXW_PATTERN_SOLID
            && format->bg_color == LXW_COLOR_UNSET
            && format->fg_color != LXW_COLOR_UNSET) {
            format->bg_color = LXW_COLOR_UNSET;
            format->pattern = LXW_PATTERN_SOLID;
        }

        if (key) {
            /* Look up the format in the hash table. */
            hash_element = _hash_key_exists(fills, key, sizeof(lxw_fill));

            if (hash_element) {
                /* Fill has already been used. */
                format->fill_index = *(uint16_t *) hash_element->value;
                format->has_fill = LXW_FALSE;
                free(key);
            }
            else {
                /* This is a new fill. */
                uint16_t *fill_index = calloc(1, sizeof(uint16_t));
                *fill_index = index;
                format->fill_index = index;
                format->has_fill = 1;
                _insert_hash_element(fills, key, fill_index,
                                     sizeof(lxw_fill));
                index++;
            }
        }
    }

    _free_lxw_hash(fills);

    self->fill_count = index;
}

/*
 * Iterate through the XF Format objects and give them an index to non-default
 * number format elements. Note, user defined records start from index 0xA4.
 */
STATIC void
_prepare_num_formats(lxw_workbook *self)
{

    lxw_hash_table *num_formats = _new_lxw_hash(128, 0, 1);
    lxw_hash_element *hash_element;
    lxw_format *format;
    uint16_t index = 0xA4;
    uint16_t num_format_count = 0;
    char *num_format;
    uint16_t *num_format_index;

    STAILQ_FOREACH(format, self->formats, list_pointers) {

        /* Format already has a number format index. */
        if (format->num_format_index)
            continue;

        /* Check if there is a user defined number format string. */
        num_format = format->num_format;

        if (strlen(num_format)) {
            /* Look up the num_format in the hash table. */
            hash_element = _hash_key_exists(num_formats, num_format,
                                            strlen(num_format));

            if (hash_element) {
                /* Num_Format has already been used. */
                format->num_format_index = *(uint16_t *) hash_element->value;
            }
            else {
                /* This is a new num_format. */
                num_format_index = calloc(1, sizeof(uint16_t));
                *num_format_index = index;
                format->num_format_index = index;
                _insert_hash_element(num_formats, num_format,
                                     num_format_index, strlen(num_format));
                index++;
                num_format_count++;
            }
        }
    }

    _free_lxw_hash(num_formats);

    self->num_format_count = num_format_count;
}

/*
 * Prepare workbook and sub-objects for writing.
 */
STATIC void
_prepare_workbook(lxw_workbook *self)
{
    lxw_worksheet *worksheet;

    /* Add a default worksheet if non have been added. */
    if (!self->num_sheets)
        workbook_add_worksheet(self, NULL);

    /* Ensure that at least one worksheet has been selected. */
    if (self->active_tab == 0) {
        worksheet = STAILQ_FIRST(self->worksheets);
        worksheet->selected = 1;
        worksheet->hidden = 0;
    }

    /* Set the active sheet. */
    STAILQ_FOREACH(worksheet, self->worksheets, list_pointers) {
        if (worksheet->index == self->active_tab)
            worksheet->active = 1;
    }

    /* Set the font index for the format objects. */
    _prepare_fonts(self);

    /* Set the number format index for the format objects. */
    _prepare_num_formats(self);

    /* Set the border index for the format objects. */
    _prepare_borders(self);

    /* Set the fill index for the format objects. */
    _prepare_fills(self);

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
_workbook_xml_declaration(lxw_workbook *self)
{
    _xml_declaration(self->file);
}

/*
 * Write the <workbook> element.
 */
STATIC void
_write_workbook(lxw_workbook *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char xmlns[] = "http://schemas.openxmlformats.org"
        "/spreadsheetml/2006/main";
    char xmlns_r[] = "http://schemas.openxmlformats.org"
        "/officeDocument/2006/relationships";

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("xmlns", xmlns);
    _PUSH_ATTRIBUTES_STR("xmlns:r", xmlns_r);

    _xml_start_tag(self->file, "workbook", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <fileVersion> element.
 */
STATIC void
_write_file_version(lxw_workbook *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("appName", "xl");
    _PUSH_ATTRIBUTES_STR("lastEdited", "4");
    _PUSH_ATTRIBUTES_STR("lowestEdited", "4");
    _PUSH_ATTRIBUTES_STR("rupBuild", "4505");

    _xml_empty_tag(self->file, "fileVersion", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <workbookPr> element.
 */
STATIC void
_write_workbook_pr(lxw_workbook *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("defaultThemeVersion", "124226");

    _xml_empty_tag(self->file, "workbookPr", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <workbookView> element.
 */
STATIC void
_write_workbook_view(lxw_workbook *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("xWindow", "240");
    _PUSH_ATTRIBUTES_STR("yWindow", "15");
    _PUSH_ATTRIBUTES_STR("windowWidth", "16095");
    _PUSH_ATTRIBUTES_STR("windowHeight", "9660");

    if (self->first_sheet)
        _PUSH_ATTRIBUTES_INT("firstSheet", self->first_sheet);

    if (self->active_tab)
        _PUSH_ATTRIBUTES_INT("activeTab", self->active_tab);

    _xml_empty_tag(self->file, "workbookView", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <bookViews> element.
 */
STATIC void
_write_book_views(lxw_workbook *self)
{
    _xml_start_tag(self->file, "bookViews", NULL);

    _write_workbook_view(self);

    _xml_end_tag(self->file, "bookViews");
}

/*
 * Write the <sheet> element.
 */
STATIC void
_write_sheet(lxw_workbook *self, const char *name, uint32_t sheet_id,
             uint8_t hidden)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char r_id[MAX_ATTRIBUTE_LENGTH] = "rId1";

    __builtin_snprintf(r_id, ATTR_32, "rId%d", sheet_id);

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("name", name);
    _PUSH_ATTRIBUTES_INT("sheetId", sheet_id);

    if (hidden)
        _PUSH_ATTRIBUTES_STR("state", "hidden");

    _PUSH_ATTRIBUTES_STR("r:id", r_id);

    _xml_empty_tag(self->file, "sheet", &attributes);

    _FREE_ATTRIBUTES();
}

/*
 * Write the <sheets> element.
 */
STATIC void
_write_sheets(lxw_workbook *self)
{
    lxw_worksheet *worksheet;

    _xml_start_tag(self->file, "sheets", NULL);

    STAILQ_FOREACH(worksheet, self->worksheets, list_pointers) {
        _write_sheet(self, worksheet->name, worksheet->index + 1,
                     worksheet->hidden);
    }

    _xml_end_tag(self->file, "sheets");
}

/*
 * Write the <calcPr> element.
 */
STATIC void
_write_calc_pr(lxw_workbook *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    _INIT_ATTRIBUTES();
    _PUSH_ATTRIBUTES_STR("calcId", "124519");
    _PUSH_ATTRIBUTES_STR("fullCalcOnLoad", "1");

    _xml_empty_tag(self->file, "calcPr", &attributes);

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
_workbook_assemble_xml_file(lxw_workbook *self)
{
    /* Write the XML declaration. */
    _workbook_xml_declaration(self);

    /* Write the root workbook element. */
    _write_workbook(self);

    /* Write the XLSX file version. */
    _write_file_version(self);

    /* Write the workbook properties. */
    _write_workbook_pr(self);

    /* Write the workbook view properties. */
    _write_book_views(self);

    /* Write the worksheet names and ids. */
    _write_sheets(self);

    /* Write the workbook calculation properties. */
    _write_calc_pr(self);

    /* Close the workbook tag. */
    _xml_end_tag(self->file, "workbook");
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/

/*
 * Create a new workbook object.
 */
lxw_workbook *
new_workbook(const char *filename)
{
    lxw_format *format;
    lxw_workbook *workbook;

    /* Create the workbook object. */
    workbook = calloc(1, sizeof(lxw_workbook));
    GOTO_LABEL_ON_MEM_ERROR(workbook, mem_error);
    workbook->filename = filename;

    /* Add the worksheets list. */
    workbook->worksheets = calloc(1, sizeof(struct lxw_worksheets));
    GOTO_LABEL_ON_MEM_ERROR(workbook->worksheets, mem_error);
    STAILQ_INIT(workbook->worksheets);

    /* Add the formats list. */
    workbook->formats = calloc(1, sizeof(struct lxw_formats));
    GOTO_LABEL_ON_MEM_ERROR(workbook->formats, mem_error);
    STAILQ_INIT(workbook->formats);

    /* Add the shared strings table. */
    workbook->sst = _new_sst();
    GOTO_LABEL_ON_MEM_ERROR(workbook->sst, mem_error);

    /* Add the default workbook properties. */
    workbook->properties = calloc(1, sizeof(lxw_doc_properties));
    GOTO_LABEL_ON_MEM_ERROR(workbook->properties, mem_error);

    /* Add a hash table to track format indices. */
    workbook->xf_format_indices = _new_lxw_hash(128, 1, 0);
    GOTO_LABEL_ON_MEM_ERROR(workbook->xf_format_indices, mem_error);

    /* Add the default cell format. */
    format = workbook_add_format(workbook);
    GOTO_LABEL_ON_MEM_ERROR(format, mem_error);

    /* Initialise its index. */
    _get_xf_index(format);

    return workbook;

mem_error:
    _free_workbook(workbook);
    return NULL;
}

/*
 * Add a new worksheet to the Excel workbook.
 */
lxw_worksheet *
workbook_add_worksheet(lxw_workbook *self, const char *sheetname)
{
    lxw_worksheet *worksheet;
    lxw_worksheet_init_data init_data;
    char *new_name = NULL;

    if (sheetname) {
        /* Use the user supplied name. */
        if (strlen(sheetname) > LXW_SHEETNAME_MAX)
            return NULL;
        else
            init_data.name = __builtin_strdup(sheetname);
    }
    else {
        /* Use the default SheetN name. */
        new_name = malloc(LXW_SHEETNAME_LEN);
        RETURN_ON_MEM_ERROR(new_name, NULL);
        __builtin_snprintf(new_name, LXW_SHEETNAME_LEN, "Sheet%d",
                           self->num_sheets + 1);
        init_data.name = new_name;
    }

    init_data.hidden = 0;
    init_data.index = self->num_sheets;
    init_data.sst = self->sst;
    init_data.optimize = self->optimize;

    /* Create a new worksheet object. */
    worksheet = _new_worksheet(&init_data);

    if (worksheet) {
        self->num_sheets++;
        STAILQ_INSERT_TAIL(self->worksheets, worksheet, list_pointers);
    }
    else {
        free(new_name);
    }

    return worksheet;
}

/*
 * Add a new format to the Excel workbook.
 */
lxw_format *
workbook_add_format(lxw_workbook *self)
{
    /* Create a new format object. */
    lxw_format *format = _new_format();

    format->xf_format_indices = self->xf_format_indices;
    format->num_xf_formats = &self->num_xf_formats;

    if (format)
        STAILQ_INSERT_TAIL(self->formats, format, list_pointers);

    return format;
}

/*
 * Call finalisation code and close file.
 */
uint8_t
workbook_close(lxw_workbook *self)
{
    lxw_packager *packager;
    uint8_t error = 0;

    /* Prepare workbook and sub-objects for writing. */
    _prepare_workbook(self);

    packager = _new_packager(self->filename);
    GOTO_LABEL_ON_MEM_ERROR(packager, mem_error);

    packager->workbook = self;

    error = _create_package(packager);

    _free_packager(packager);
    _free_workbook(self);

    return error;

mem_error:
    return 1;
}
