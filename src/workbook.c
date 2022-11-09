/*****************************************************************************
 * workbook - A library for creating Excel XLSX workbook files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2022, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/workbook.h"
#include "xlsxwriter/utility.h"
#include "xlsxwriter/packager.h"
#include "xlsxwriter/hash_table.h"

STATIC int _worksheet_name_cmp(lxw_worksheet_name *name1,
                               lxw_worksheet_name *name2);
STATIC int _chartsheet_name_cmp(lxw_chartsheet_name *name1,
                                lxw_chartsheet_name *name2);
STATIC int _image_md5_cmp(lxw_image_md5 *tuple1, lxw_image_md5 *tuple2);

#ifndef __clang_analyzer__
LXW_RB_GENERATE_WORKSHEET_NAMES(lxw_worksheet_names, lxw_worksheet_name,
                                tree_pointers, _worksheet_name_cmp);
LXW_RB_GENERATE_CHARTSHEET_NAMES(lxw_chartsheet_names, lxw_chartsheet_name,
                                 tree_pointers, _chartsheet_name_cmp);
LXW_RB_GENERATE_IMAGE_MD5S(lxw_image_md5s, lxw_image_md5,
                           tree_pointers, _image_md5_cmp);
#endif

/*
 * Forward declarations.
 */

/*****************************************************************************
 *
 * Private functions.
 *
 ****************************************************************************/

/*
 * Comparators for the sheet names structure red/black tree.
 */
STATIC int
_worksheet_name_cmp(lxw_worksheet_name *name1, lxw_worksheet_name *name2)
{
    return lxw_strcasecmp(name1->name, name2->name);
}

STATIC int
_chartsheet_name_cmp(lxw_chartsheet_name *name1, lxw_chartsheet_name *name2)
{
    return lxw_strcasecmp(name1->name, name2->name);
}

STATIC int
_image_md5_cmp(lxw_image_md5 *tuple1, lxw_image_md5 *tuple2)
{
    return strcmp(tuple1->md5, tuple2->md5);
}

/*
 * Free workbook properties.
 */
STATIC void
_free_doc_properties(lxw_doc_properties *properties)
{
    if (properties) {
        free(properties->title);
        free(properties->subject);
        free(properties->author);
        free(properties->manager);
        free(properties->company);
        free(properties->category);
        free(properties->keywords);
        free(properties->comments);
        free(properties->status);
        free(properties->hyperlink_base);
    }

    free(properties);
}

/*
 * Free workbook custom property.
 */
STATIC void
_free_custom_doc_property(lxw_custom_property *custom_property)
{
    if (custom_property) {
        free(custom_property->name);
        if (custom_property->type == LXW_CUSTOM_STRING)
            free(custom_property->u.string);
    }

    free(custom_property);
}

/*
 * Free a workbook object.
 */
void
lxw_workbook_free(lxw_workbook *workbook)
{
    lxw_sheet *sheet;
    struct lxw_worksheet_name *worksheet_name;
    struct lxw_worksheet_name *next_worksheet_name;
    struct lxw_chartsheet_name *chartsheet_name;
    struct lxw_chartsheet_name *next_chartsheet_name;
    struct lxw_image_md5 *image_md5;
    struct lxw_image_md5 *next_image_md5;
    lxw_chart *chart;
    lxw_format *format;
    lxw_defined_name *defined_name;
    lxw_defined_name *defined_name_tmp;
    lxw_custom_property *custom_property;

    if (!workbook)
        return;

    _free_doc_properties(workbook->properties);

    free(workbook->filename);

    /* Free the sheets in the workbook. */
    if (workbook->sheets) {
        while (!STAILQ_EMPTY(workbook->sheets)) {
            sheet = STAILQ_FIRST(workbook->sheets);

            if (sheet->is_chartsheet)
                lxw_chartsheet_free(sheet->u.chartsheet);
            else
                lxw_worksheet_free(sheet->u.worksheet);

            STAILQ_REMOVE_HEAD(workbook->sheets, list_pointers);
            free(sheet);
        }
        free(workbook->sheets);
    }

    /* Free the sheet lists. The worksheet objects are freed above. */
    free(workbook->worksheets);
    free(workbook->chartsheets);

    /* Free the charts in the workbook. */
    if (workbook->charts) {
        while (!STAILQ_EMPTY(workbook->charts)) {
            chart = STAILQ_FIRST(workbook->charts);
            STAILQ_REMOVE_HEAD(workbook->charts, list_pointers);
            lxw_chart_free(chart);
        }
        free(workbook->charts);
    }

    /* Free the formats in the workbook. */
    if (workbook->formats) {
        while (!STAILQ_EMPTY(workbook->formats)) {
            format = STAILQ_FIRST(workbook->formats);
            STAILQ_REMOVE_HEAD(workbook->formats, list_pointers);
            lxw_format_free(format);
        }
        free(workbook->formats);
    }

    /* Free the defined_names in the workbook. */
    if (workbook->defined_names) {
        defined_name = TAILQ_FIRST(workbook->defined_names);
        while (defined_name) {

            defined_name_tmp = TAILQ_NEXT(defined_name, list_pointers);
            free(defined_name);
            defined_name = defined_name_tmp;
        }
        free(workbook->defined_names);
    }

    /* Free the custom_properties in the workbook. */
    if (workbook->custom_properties) {
        while (!STAILQ_EMPTY(workbook->custom_properties)) {
            custom_property = STAILQ_FIRST(workbook->custom_properties);
            STAILQ_REMOVE_HEAD(workbook->custom_properties, list_pointers);
            _free_custom_doc_property(custom_property);
        }
        free(workbook->custom_properties);
    }

    if (workbook->worksheet_names) {
        for (worksheet_name =
             RB_MIN(lxw_worksheet_names, workbook->worksheet_names);
             worksheet_name; worksheet_name = next_worksheet_name) {

            next_worksheet_name = RB_NEXT(lxw_worksheet_names,
                                          workbook->worksheet_name,
                                          worksheet_name);
            RB_REMOVE(lxw_worksheet_names, workbook->worksheet_names,
                      worksheet_name);
            free(worksheet_name);
        }

        free(workbook->worksheet_names);
    }

    if (workbook->chartsheet_names) {
        for (chartsheet_name =
             RB_MIN(lxw_chartsheet_names, workbook->chartsheet_names);
             chartsheet_name; chartsheet_name = next_chartsheet_name) {

            next_chartsheet_name = RB_NEXT(lxw_chartsheet_names,
                                           workbook->chartsheet_name,
                                           chartsheet_name);
            RB_REMOVE(lxw_chartsheet_names, workbook->chartsheet_names,
                      chartsheet_name);
            free(chartsheet_name);
        }

        free(workbook->chartsheet_names);
    }

    if (workbook->image_md5s) {
        for (image_md5 = RB_MIN(lxw_image_md5s, workbook->image_md5s);
             image_md5; image_md5 = next_image_md5) {

            next_image_md5 =
                RB_NEXT(lxw_image_md5s, workbook->image_md5, image_md5);
            RB_REMOVE(lxw_image_md5s, workbook->image_md5s, image_md5);
            free(image_md5->md5);
            free(image_md5);
        }

        free(workbook->image_md5s);
    }

    if (workbook->header_image_md5s) {
        for (image_md5 = RB_MIN(lxw_image_md5s, workbook->header_image_md5s);
             image_md5; image_md5 = next_image_md5) {

            next_image_md5 =
                RB_NEXT(lxw_image_md5s, workbook->image_md5, image_md5);
            RB_REMOVE(lxw_image_md5s, workbook->header_image_md5s, image_md5);
            free(image_md5->md5);
            free(image_md5);
        }

        free(workbook->header_image_md5s);
    }

    if (workbook->background_md5s) {
        for (image_md5 = RB_MIN(lxw_image_md5s, workbook->background_md5s);
             image_md5; image_md5 = next_image_md5) {

            next_image_md5 =
                RB_NEXT(lxw_image_md5s, workbook->image_md5, image_md5);
            RB_REMOVE(lxw_image_md5s, workbook->background_md5s, image_md5);
            free(image_md5->md5);
            free(image_md5);
        }

        free(workbook->background_md5s);
    }

    lxw_hash_free(workbook->used_xf_formats);
    lxw_hash_free(workbook->used_dxf_formats);
    lxw_sst_free(workbook->sst);
    free(workbook->options.tmpdir);
    free(workbook->ordered_charts);
    free(workbook->vba_project);
    free(workbook->vba_codename);
    free(workbook);
}

/*
 * Set the default index for each format. This is only used for testing.
 */
void
lxw_workbook_set_default_xf_indices(lxw_workbook *self)
{
    lxw_format *format;
    int32_t index = 0;

    STAILQ_FOREACH(format, self->formats, list_pointers) {

        /* Skip the hyperlink format. */
        if (index != 1)
            lxw_format_get_xf_index(format);

        index++;
    }
}

/*
 * Iterate through the XF Format objects and give them an index to non-default
 * font elements.
 */
STATIC void
_prepare_fonts(lxw_workbook *self)
{

    lxw_hash_table *fonts = lxw_hash_new(128, 1, 1);
    lxw_hash_element *hash_element;
    lxw_hash_element *used_format_element;
    uint16_t index = 0;

    LXW_FOREACH_ORDERED(used_format_element, self->used_xf_formats) {
        lxw_format *format = (lxw_format *) used_format_element->value;
        lxw_font *key = lxw_format_get_font_key(format);

        if (key) {
            /* Look up the format in the hash table. */
            hash_element = lxw_hash_key_exists(fonts, key, sizeof(lxw_font));

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
                format->has_font = LXW_TRUE;
                lxw_insert_hash_element(fonts, key, font_index,
                                        sizeof(lxw_font));
                index++;
            }
        }
    }

    lxw_hash_free(fonts);

    /* For DXF formats we only need to check if the properties have changed. */
    LXW_FOREACH_ORDERED(used_format_element, self->used_dxf_formats) {
        lxw_format *format = (lxw_format *) used_format_element->value;

        /* The only font properties that can change for a DXF format are:
         * color, bold, italic, underline and strikethrough. */
        if (format->font_color || format->bold || format->italic
            || format->underline || format->font_strikeout) {
            format->has_dxf_font = LXW_TRUE;
        }
    }

    self->font_count = index;
}

/*
 * Iterate through the XF Format objects and give them an index to non-default
 * border elements.
 */
STATIC void
_prepare_borders(lxw_workbook *self)
{

    lxw_hash_table *borders = lxw_hash_new(128, 1, 1);
    lxw_hash_element *hash_element;
    lxw_hash_element *used_format_element;
    uint16_t index = 0;

    LXW_FOREACH_ORDERED(used_format_element, self->used_xf_formats) {
        lxw_format *format = (lxw_format *) used_format_element->value;
        lxw_border *key = lxw_format_get_border_key(format);

        if (key) {
            /* Look up the format in the hash table. */
            hash_element =
                lxw_hash_key_exists(borders, key, sizeof(lxw_border));

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
                lxw_insert_hash_element(borders, key, border_index,
                                        sizeof(lxw_border));
                index++;
            }
        }
    }

    /* For DXF formats we only need to check if the properties have changed. */
    LXW_FOREACH_ORDERED(used_format_element, self->used_dxf_formats) {
        lxw_format *format = (lxw_format *) used_format_element->value;

        if (format->left || format->right || format->top || format->bottom) {
            format->has_dxf_border = LXW_TRUE;
        }
    }

    lxw_hash_free(borders);

    self->border_count = index;
}

/*
 * Iterate through the XF Format objects and give them an index to non-default
 * fill elements.
 */
STATIC void
_prepare_fills(lxw_workbook *self)
{

    lxw_hash_table *fills = lxw_hash_new(128, 1, 1);
    lxw_hash_element *hash_element;
    lxw_hash_element *used_format_element;
    uint16_t index = 2;
    lxw_fill *default_fill_1 = NULL;
    lxw_fill *default_fill_2 = NULL;
    uint16_t *fill_index1 = NULL;
    uint16_t *fill_index2 = NULL;

    default_fill_1 = calloc(1, sizeof(lxw_fill));
    GOTO_LABEL_ON_MEM_ERROR(default_fill_1, mem_error);

    default_fill_2 = calloc(1, sizeof(lxw_fill));
    GOTO_LABEL_ON_MEM_ERROR(default_fill_2, mem_error);

    fill_index1 = calloc(1, sizeof(uint16_t));
    GOTO_LABEL_ON_MEM_ERROR(fill_index1, mem_error);

    fill_index2 = calloc(1, sizeof(uint16_t));
    GOTO_LABEL_ON_MEM_ERROR(fill_index2, mem_error);

    /* Add the default fills. */
    default_fill_1->pattern = LXW_PATTERN_NONE;
    default_fill_1->fg_color = LXW_COLOR_UNSET;
    default_fill_1->bg_color = LXW_COLOR_UNSET;
    *fill_index1 = 0;
    lxw_insert_hash_element(fills, default_fill_1, fill_index1,
                            sizeof(lxw_fill));

    default_fill_2->pattern = LXW_PATTERN_GRAY_125;
    default_fill_2->fg_color = LXW_COLOR_UNSET;
    default_fill_2->bg_color = LXW_COLOR_UNSET;
    *fill_index2 = 1;
    lxw_insert_hash_element(fills, default_fill_2, fill_index2,
                            sizeof(lxw_fill));

    /* For DXF formats we only need to check if the properties have changed. */
    LXW_FOREACH_ORDERED(used_format_element, self->used_dxf_formats) {
        lxw_format *format = (lxw_format *) used_format_element->value;

        if (format->pattern || format->bg_color || format->fg_color) {
            format->has_dxf_fill = LXW_TRUE;
            format->dxf_bg_color = format->bg_color;
            format->dxf_fg_color = format->fg_color;
        }
    }

    LXW_FOREACH_ORDERED(used_format_element, self->used_xf_formats) {
        lxw_format *format = (lxw_format *) used_format_element->value;
        lxw_fill *key = lxw_format_get_fill_key(format);

        /* The following logical statements jointly take care of special */
        /* cases in relation to cell colors and patterns:                */
        /* 1. For a solid fill (pattern == 1) Excel reverses the role of */
        /*    foreground and background colors, and                      */
        /* 2. If the user specifies a foreground or background color     */
        /*    without a pattern they probably wanted a solid fill, so    */
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
            format->pattern = LXW_PATTERN_SOLID;
        }

        if (key) {
            /* Look up the format in the hash table. */
            hash_element = lxw_hash_key_exists(fills, key, sizeof(lxw_fill));

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
                lxw_insert_hash_element(fills, key, fill_index,
                                        sizeof(lxw_fill));
                index++;
            }
        }
    }

    lxw_hash_free(fills);

    self->fill_count = index;

    return;

mem_error:
    free(fill_index2);
    free(fill_index1);
    free(default_fill_2);
    free(default_fill_1);
    lxw_hash_free(fills);
}

/*
 * Iterate through the XF Format objects and give them an index to non-default
 * number format elements. Note, user defined records start from index 0xA4.
 */
STATIC void
_prepare_num_formats(lxw_workbook *self)
{

    lxw_hash_table *num_formats = lxw_hash_new(128, 0, 1);
    lxw_hash_element *hash_element;
    lxw_hash_element *used_format_element;
    uint16_t index = 0xA4;
    uint16_t num_format_count = 0;
    uint16_t *num_format_index;

    LXW_FOREACH_ORDERED(used_format_element, self->used_xf_formats) {
        lxw_format *format = (lxw_format *) used_format_element->value;

        /* Format already has a number format index. */
        if (format->num_format_index)
            continue;

        /* Check if there is a user defined number format string. */
        if (*format->num_format) {
            char num_format[LXW_FORMAT_FIELD_LEN] = { 0 };
            lxw_snprintf(num_format, LXW_FORMAT_FIELD_LEN, "%s",
                         format->num_format);

            /* Look up the num_format in the hash table. */
            hash_element = lxw_hash_key_exists(num_formats, num_format,
                                               LXW_FORMAT_FIELD_LEN);

            if (hash_element) {
                /* Num_Format has already been used. */
                format->num_format_index = *(uint16_t *) hash_element->value;
            }
            else {
                /* This is a new num_format. */
                num_format_index = calloc(1, sizeof(uint16_t));
                *num_format_index = index;
                format->num_format_index = index;
                lxw_insert_hash_element(num_formats, format->num_format,
                                        num_format_index,
                                        LXW_FORMAT_FIELD_LEN);
                index++;
                num_format_count++;
            }
        }
    }

    LXW_FOREACH_ORDERED(used_format_element, self->used_dxf_formats) {
        lxw_format *format = (lxw_format *) used_format_element->value;

        /* Format already has a number format index. */
        if (format->num_format_index)
            continue;

        /* Check if there is a user defined number format string. */
        if (*format->num_format) {
            char num_format[LXW_FORMAT_FIELD_LEN] = { 0 };
            lxw_snprintf(num_format, LXW_FORMAT_FIELD_LEN, "%s",
                         format->num_format);

            /* Look up the num_format in the hash table. */
            hash_element = lxw_hash_key_exists(num_formats, num_format,
                                               LXW_FORMAT_FIELD_LEN);

            if (hash_element) {
                /* Num_Format has already been used. */
                format->num_format_index = *(uint16_t *) hash_element->value;
            }
            else {
                /* This is a new num_format. */
                num_format_index = calloc(1, sizeof(uint16_t));
                *num_format_index = index;
                format->num_format_index = index;
                lxw_insert_hash_element(num_formats, format->num_format,
                                        num_format_index,
                                        LXW_FORMAT_FIELD_LEN);
                index++;
                /* Don't update num_format_count for DXF formats. */
            }
        }
    }

    lxw_hash_free(num_formats);

    self->num_format_count = num_format_count;
}

/*
 * Prepare workbook and sub-objects for writing.
 */
STATIC void
_prepare_workbook(lxw_workbook *self)
{
    /* Set the font index for the format objects. */
    _prepare_fonts(self);

    /* Set the number format index for the format objects. */
    _prepare_num_formats(self);

    /* Set the border index for the format objects. */
    _prepare_borders(self);

    /* Set the fill index for the format objects. */
    _prepare_fills(self);

}

/*
 * Compare two defined_name structures.
 */
static int
_compare_defined_names(lxw_defined_name *a, lxw_defined_name *b)
{
    int res = strcmp(a->normalised_name, b->normalised_name);

    /* Primary comparison based on defined name. */
    if (res)
        return res;

    /* Secondary comparison based on worksheet name. */
    res = strcmp(a->normalised_sheetname, b->normalised_sheetname);

    return res;
}

/*
 * Process and store the defined names. The defined names are stored with
 * the Workbook.xml but also with the App.xml if they refer to a sheet
 * range like "Sheet1!:A1". The defined names are store in sorted
 * order for consistency with Excel. The names need to be normalized before
 * sorting.
 */
STATIC lxw_error
_store_defined_name(lxw_workbook *self, const char *name,
                    const char *app_name, const char *formula, int16_t index,
                    uint8_t hidden)
{
    lxw_sheet *sheet;
    lxw_worksheet *worksheet;
    lxw_defined_name *defined_name;
    lxw_defined_name *list_defined_name;
    char name_copy[LXW_DEFINED_NAME_LENGTH];
    char *tmp_str;
    char *worksheet_name;

    /* Do some checks on the input data */
    if (!name || !formula)
        return LXW_ERROR_NULL_PARAMETER_IGNORED;

    if (lxw_utf8_strlen(name) > LXW_DEFINED_NAME_LENGTH ||
        lxw_utf8_strlen(formula) > LXW_DEFINED_NAME_LENGTH) {
        return LXW_ERROR_128_STRING_LENGTH_EXCEEDED;
    }

    /* Allocate a new defined_name to be added to the linked list of names. */
    defined_name = calloc(1, sizeof(struct lxw_defined_name));
    RETURN_ON_MEM_ERROR(defined_name, LXW_ERROR_MEMORY_MALLOC_FAILED);

    /* Copy the user input string. */
    lxw_strcpy(name_copy, name);

    /* Set the worksheet index or -1 for a global defined name. */
    defined_name->index = index;
    defined_name->hidden = hidden;

    /* Check for local defined names like like "Sheet1!name". */
    tmp_str = strchr(name_copy, '!');

    if (tmp_str == NULL) {
        /* The name is global. We just store the defined name string. */
        lxw_strcpy(defined_name->name, name_copy);
    }
    else {
        /* The name is worksheet local. We need to extract the sheet name
         * and map it to a sheet index. */

        /* Split the into the worksheet name and defined name. */
        *tmp_str = '\0';
        tmp_str++;
        worksheet_name = name_copy;

        /* Remove any worksheet quoting. */
        if (worksheet_name[0] == '\'')
            worksheet_name++;
        if (worksheet_name[strlen(worksheet_name) - 1] == '\'')
            worksheet_name[strlen(worksheet_name) - 1] = '\0';

        /* Search for worksheet name to get the equivalent worksheet index. */
        STAILQ_FOREACH(sheet, self->sheets, list_pointers) {
            if (sheet->is_chartsheet)
                continue;
            else
                worksheet = sheet->u.worksheet;

            if (strcmp(worksheet_name, worksheet->name) == 0) {
                defined_name->index = worksheet->index;
                lxw_strcpy(defined_name->normalised_sheetname,
                           worksheet_name);
            }
        }

        /* If we didn't find the worksheet name we exit. */
        if (defined_name->index == -1)
            goto mem_error;

        lxw_strcpy(defined_name->name, tmp_str);
    }

    /* Print titles and repeat title pass in the name used for App.xml. */
    if (app_name) {
        lxw_strcpy(defined_name->app_name, app_name);
        lxw_strcpy(defined_name->normalised_sheetname, app_name);
    }
    else {
        lxw_strcpy(defined_name->app_name, name);
    }

    /* We need to normalize the defined names for sorting. This involves
     * removing any _xlnm namespace  and converting it to lowercase. */
    tmp_str = strstr(name_copy, "_xlnm.");

    if (tmp_str)
        lxw_strcpy(defined_name->normalised_name, defined_name->name + 6);
    else
        lxw_strcpy(defined_name->normalised_name, defined_name->name);

    lxw_str_tolower(defined_name->normalised_name);
    lxw_str_tolower(defined_name->normalised_sheetname);

    /* Strip leading "=" from the formula. */
    if (formula[0] == '=')
        lxw_strcpy(defined_name->formula, formula + 1);
    else
        lxw_strcpy(defined_name->formula, formula);

    /* We add the defined name to the list in sorted order. */
    list_defined_name = TAILQ_FIRST(self->defined_names);

    if (list_defined_name == NULL ||
        _compare_defined_names(defined_name, list_defined_name) < 1) {
        /* List is empty or defined name goes to the head. */
        TAILQ_INSERT_HEAD(self->defined_names, defined_name, list_pointers);
        return LXW_NO_ERROR;
    }

    TAILQ_FOREACH(list_defined_name, self->defined_names, list_pointers) {
        int res = _compare_defined_names(defined_name, list_defined_name);

        /* The entry already exists. We exit and don't overwrite. */
        if (res == 0)
            goto mem_error;

        /* New defined name is inserted in sorted order before other entries. */
        if (res < 0) {
            TAILQ_INSERT_BEFORE(list_defined_name, defined_name,
                                list_pointers);
            return LXW_NO_ERROR;
        }
    }

    /* If the entry wasn't less than any of the entries in the list we add it
     * to the end. */
    TAILQ_INSERT_TAIL(self->defined_names, defined_name, list_pointers);
    return LXW_NO_ERROR;

mem_error:
    free(defined_name);
    return LXW_ERROR_MEMORY_MALLOC_FAILED;
}

/*
 * Populate the data cache of a chart data series by reading the data from the
 * relevant worksheet and adding it to the cached in the range object as a
 * list of points.
 *
 * Note, the data cache isn't strictly required by Excel but it helps if the
 * chart is embedded in another application such as PowerPoint and it also
 * helps with comparison testing.
 */
STATIC void
_populate_range_data_cache(lxw_workbook *self, lxw_series_range *range)
{
    lxw_worksheet *worksheet;
    lxw_row_t row_num;
    lxw_col_t col_num;
    lxw_row *row_obj;
    lxw_cell *cell_obj;
    struct lxw_series_data_point *data_point;
    uint16_t num_data_points = 0;

    /* If ignore_cache is set then don't try to populate the cache. This flag
     * may be set manually, for testing, or due to a case where the cache
     * can't be calculated.
     */
    if (range->ignore_cache)
        return;

    /* Currently we only handle 2D ranges so ensure either the rows or cols
     * are the same.
     */
    if (range->first_row != range->last_row
        && range->first_col != range->last_col) {
        range->ignore_cache = LXW_TRUE;
        return;
    }

    /* Check that the sheetname exists. */
    worksheet = workbook_get_worksheet_by_name(self, range->sheetname);
    if (!worksheet) {
        LXW_WARN_FORMAT2("workbook_add_chart(): worksheet name '%s' "
                         "in chart formula '%s' doesn't exist.",
                         range->sheetname, range->formula);
        range->ignore_cache = LXW_TRUE;
        return;
    }

    /* We can't read the data when worksheet optimization is on. */
    if (worksheet->optimize) {
        range->ignore_cache = LXW_TRUE;
        return;
    }

    /* Iterate through the worksheet data and populate the range cache. */
    for (row_num = range->first_row; row_num <= range->last_row; row_num++) {
        row_obj = lxw_worksheet_find_row(worksheet, row_num);

        for (col_num = range->first_col; col_num <= range->last_col;
             col_num++) {

            data_point = calloc(1, sizeof(struct lxw_series_data_point));
            if (!data_point) {
                range->ignore_cache = LXW_TRUE;
                return;
            }

            cell_obj = lxw_worksheet_find_cell_in_row(row_obj, col_num);

            if (cell_obj) {
                if (cell_obj->type == NUMBER_CELL) {
                    data_point->number = cell_obj->u.number;
                }

                if (cell_obj->type == STRING_CELL) {
                    data_point->string = lxw_strdup(cell_obj->sst_string);
                    data_point->is_string = LXW_TRUE;
                    range->has_string_cache = LXW_TRUE;
                }
            }
            else {
                data_point->no_data = LXW_TRUE;
            }

            STAILQ_INSERT_TAIL(range->data_cache, data_point, list_pointers);
            num_data_points++;
        }
    }

    range->num_data_points = num_data_points;

}

/* Convert a chart range such as Sheet1!$A$1:$A$5 to a sheet name and row-col
 * dimensions, or vice-versa. This gives us the dimensions to read data back
 * from the worksheet.
 */
STATIC void
_populate_range_dimensions(lxw_workbook *self, lxw_series_range *range)
{

    char formula[LXW_MAX_FORMULA_RANGE_LENGTH] = { 0 };
    char *tmp_str;
    char *sheetname;

    /* If neither the range formula or sheetname is defined then this probably
     * isn't a valid range.
     */
    if (!range->formula && !range->sheetname) {
        range->ignore_cache = LXW_TRUE;
        return;
    }

    /* If the sheetname is already defined it was already set via
     * chart_series_set_categories() or  chart_series_set_values().
     */
    if (range->sheetname)
        return;

    /* Ignore non-contiguous range like (Sheet1!$A$1:$A$2,Sheet1!$A$4:$A$5) */
    if (range->formula[0] == '(') {
        range->ignore_cache = LXW_TRUE;
        return;
    }

    /* Create a copy of the formula to modify and parse into parts. */
    lxw_snprintf(formula, LXW_MAX_FORMULA_RANGE_LENGTH, "%s", range->formula);

    /* Check for valid formula. Note, This needs stronger validation. */
    tmp_str = strchr(formula, '!');

    if (tmp_str == NULL) {
        range->ignore_cache = LXW_TRUE;
        return;
    }
    else {
        /* Split the formulas into sheetname and row-col data. */
        *tmp_str = '\0';
        tmp_str++;
        sheetname = formula;

        /* Remove any worksheet quoting. */
        if (sheetname[0] == '\'')
            sheetname++;
        if (sheetname[strlen(sheetname) - 1] == '\'')
            sheetname[strlen(sheetname) - 1] = '\0';

        /* Check that the sheetname exists. */
        if (!workbook_get_worksheet_by_name(self, sheetname)) {
            LXW_WARN_FORMAT2("workbook_add_chart(): worksheet name '%s' "
                             "in chart formula '%s' doesn't exist.",
                             sheetname, range->formula);
            range->ignore_cache = LXW_TRUE;
            return;
        }

        range->sheetname = lxw_strdup(sheetname);
        range->first_row = lxw_name_to_row(tmp_str);
        range->first_col = lxw_name_to_col(tmp_str);

        if (strchr(tmp_str, ':')) {
            /* 2D range. */
            range->last_row = lxw_name_to_row_2(tmp_str);
            range->last_col = lxw_name_to_col_2(tmp_str);
        }
        else {
            /* 1D range. */
            range->last_row = range->first_row;
            range->last_col = range->first_col;
        }

    }
}

/* Set the range dimensions and set the data cache.
 */
STATIC void
_populate_range(lxw_workbook *self, lxw_series_range *range)
{
    if (!range)
        return;

    _populate_range_dimensions(self, range);
    _populate_range_data_cache(self, range);
}

/*
 * Add "cached" data to charts to provide the numCache and strCache data for
 * series and title/axis ranges.
 */
STATIC void
_add_chart_cache_data(lxw_workbook *self)
{
    lxw_chart *chart;
    lxw_chart_series *series;
    uint16_t i;

    STAILQ_FOREACH(chart, self->ordered_charts, ordered_list_pointers) {

        _populate_range(self, chart->title.range);
        _populate_range(self, chart->x_axis->title.range);
        _populate_range(self, chart->y_axis->title.range);

        if (STAILQ_EMPTY(chart->series_list))
            continue;

        STAILQ_FOREACH(series, chart->series_list, list_pointers) {
            _populate_range(self, series->categories);
            _populate_range(self, series->values);
            _populate_range(self, series->title.range);

            for (i = 0; i < series->data_label_count; i++) {
                lxw_chart_custom_label *data_label = &series->data_labels[i];
                _populate_range(self, data_label->range);
            }
        }
    }
}

/*
 * Store the image types used in the workbook to update the content types.
 */
STATIC void
_store_image_type(lxw_workbook *self, uint8_t image_type)
{
    if (image_type == LXW_IMAGE_PNG)
        self->has_png = LXW_TRUE;

    if (image_type == LXW_IMAGE_JPEG)
        self->has_jpeg = LXW_TRUE;

    if (image_type == LXW_IMAGE_BMP)
        self->has_bmp = LXW_TRUE;

    if (image_type == LXW_IMAGE_GIF)
        self->has_gif = LXW_TRUE;
}

/*
 * Iterate through the worksheets and set up any chart or image drawings.
 */
STATIC void
_prepare_drawings(lxw_workbook *self)
{
    lxw_sheet *sheet;
    lxw_worksheet *worksheet;
    lxw_object_properties *object_props;
    uint32_t chart_ref_id = 0;
    uint32_t image_ref_id = 0;
    uint32_t ref_id = 0;
    uint32_t drawing_id = 0;
    uint8_t is_chartsheet;
    lxw_image_md5 tmp_image_md5;
    lxw_image_md5 *new_image_md5 = NULL;
    lxw_image_md5 *found_duplicate_image = NULL;
    uint8_t i;

    STAILQ_FOREACH(sheet, self->sheets, list_pointers) {
        if (sheet->is_chartsheet) {
            worksheet = sheet->u.chartsheet->worksheet;
            is_chartsheet = LXW_TRUE;
        }
        else {
            worksheet = sheet->u.worksheet;
            is_chartsheet = LXW_FALSE;
        }

        if (STAILQ_EMPTY(worksheet->image_props)
            && STAILQ_EMPTY(worksheet->chart_data)
            && !worksheet->has_header_vml && !worksheet->has_background_image) {
            continue;
        }

        drawing_id++;

        /* Prepare background images. */
        if (worksheet->has_background_image) {

            object_props = worksheet->background_image;

            _store_image_type(self, object_props->image_type);

            /* Check for duplicate images and only store the first instance. */
            if (object_props->md5) {
                tmp_image_md5.md5 = object_props->md5;
                found_duplicate_image = RB_FIND(lxw_image_md5s,
                                                self->background_md5s,
                                                &tmp_image_md5);
            }

            if (found_duplicate_image) {
                ref_id = found_duplicate_image->id;
                object_props->is_duplicate = LXW_TRUE;
            }
            else {
                image_ref_id++;
                ref_id = image_ref_id;

#ifndef USE_NO_MD5
                new_image_md5 = calloc(1, sizeof(lxw_image_md5));
#endif
                if (new_image_md5 && object_props->md5) {
                    new_image_md5->id = ref_id;
                    new_image_md5->md5 = lxw_strdup(object_props->md5);

                    RB_INSERT(lxw_image_md5s, self->background_md5s,
                              new_image_md5);
                }
            }

            lxw_worksheet_prepare_background(worksheet, ref_id, object_props);
        }

        /* Prepare worksheet images. */
        STAILQ_FOREACH(object_props, worksheet->image_props, list_pointers) {

            /* Ignore background image added above. */
            if (object_props->is_background)
                continue;

            _store_image_type(self, object_props->image_type);

            /* Check for duplicate images and only store the first instance. */
            if (object_props->md5) {
                tmp_image_md5.md5 = object_props->md5;
                found_duplicate_image = RB_FIND(lxw_image_md5s,
                                                self->image_md5s,
                                                &tmp_image_md5);
            }

            if (found_duplicate_image) {
                ref_id = found_duplicate_image->id;
                object_props->is_duplicate = LXW_TRUE;
            }
            else {
                image_ref_id++;
                ref_id = image_ref_id;

#ifndef USE_NO_MD5
                new_image_md5 = calloc(1, sizeof(lxw_image_md5));
#endif
                if (new_image_md5 && object_props->md5) {
                    new_image_md5->id = ref_id;
                    new_image_md5->md5 = lxw_strdup(object_props->md5);

                    RB_INSERT(lxw_image_md5s, self->image_md5s,
                              new_image_md5);
                }
            }

            lxw_worksheet_prepare_image(worksheet, ref_id, drawing_id,
                                        object_props);
        }

        /* Prepare worksheet charts. */
        STAILQ_FOREACH(object_props, worksheet->chart_data, list_pointers) {
            chart_ref_id++;
            lxw_worksheet_prepare_chart(worksheet, chart_ref_id, drawing_id,
                                        object_props, is_chartsheet);
            if (object_props->chart)
                STAILQ_INSERT_TAIL(self->ordered_charts, object_props->chart,
                                   ordered_list_pointers);
        }

        /* Prepare worksheet header/footer images. */
        for (i = 0; i < LXW_HEADER_FOOTER_OBJS_MAX; i++) {

            object_props = *worksheet->header_footer_objs[i];
            if (!object_props)
                continue;

            _store_image_type(self, object_props->image_type);

            /* Check for duplicate images and only store the first instance. */
            if (object_props->md5) {
                tmp_image_md5.md5 = object_props->md5;
                found_duplicate_image = RB_FIND(lxw_image_md5s,
                                                self->header_image_md5s,
                                                &tmp_image_md5);
            }

            if (found_duplicate_image) {
                ref_id = found_duplicate_image->id;
                object_props->is_duplicate = LXW_TRUE;
            }
            else {
                image_ref_id++;
                ref_id = image_ref_id;

#ifndef USE_NO_MD5
                new_image_md5 = calloc(1, sizeof(lxw_image_md5));
#endif
                if (new_image_md5 && object_props->md5) {
                    new_image_md5->id = ref_id;
                    new_image_md5->md5 = lxw_strdup(object_props->md5);

                    RB_INSERT(lxw_image_md5s, self->header_image_md5s,
                              new_image_md5);
                }
            }

            lxw_worksheet_prepare_header_image(worksheet, ref_id,
                                               object_props);
        }

    }

    self->drawing_count = drawing_id;
}

/*
 * Iterate through the worksheets and set up the VML objects.
 */
STATIC void
_prepare_vml(lxw_workbook *self)
{
    lxw_worksheet *worksheet;
    lxw_sheet *sheet;
    uint32_t comment_id = 0;
    uint32_t vml_drawing_id = 0;
    uint32_t vml_data_id = 1;
    uint32_t vml_header_id = 0;
    uint32_t vml_shape_id = 1024;
    uint32_t comment_count = 0;

    STAILQ_FOREACH(sheet, self->sheets, list_pointers) {
        if (sheet->is_chartsheet)
            continue;
        else
            worksheet = sheet->u.worksheet;

        if (!worksheet->has_vml && !worksheet->has_header_vml)
            continue;

        if (worksheet->has_vml) {
            self->has_vml = LXW_TRUE;
            if (worksheet->has_comments) {
                self->comment_count++;
                comment_id++;
                self->has_comments = LXW_TRUE;
            }

            vml_drawing_id++;

            comment_count = lxw_worksheet_prepare_vml_objects(worksheet,
                                                              vml_data_id,
                                                              vml_shape_id,
                                                              vml_drawing_id,
                                                              comment_id);

            /* Each VML should start with a shape id incremented by 1024. */
            vml_data_id += 1 * ((1024 + comment_count) / 1024);
            vml_shape_id += 1024 * ((1024 + comment_count) / 1024);
        }

        if (worksheet->has_header_vml) {
            self->has_vml = LXW_TRUE;
            vml_drawing_id++;
            vml_header_id++;
            lxw_worksheet_prepare_header_vml_objects(worksheet,
                                                     vml_header_id,
                                                     vml_drawing_id);
        }
    }
}

/*
 * Iterate through the worksheets and store any defined names used for print
 * ranges or repeat rows/columns.
 */
STATIC void
_prepare_defined_names(lxw_workbook *self)
{
    lxw_worksheet *worksheet;
    lxw_sheet *sheet;
    char app_name[LXW_DEFINED_NAME_LENGTH];
    char range[LXW_DEFINED_NAME_LENGTH];
    char area[LXW_MAX_CELL_RANGE_LENGTH];
    char first_col[8];
    char last_col[8];

    STAILQ_FOREACH(sheet, self->sheets, list_pointers) {
        if (sheet->is_chartsheet)
            continue;
        else
            worksheet = sheet->u.worksheet;
        /*
         * Check for autofilter settings and store them.
         */
        if (worksheet->autofilter.in_use) {

            lxw_snprintf(app_name, LXW_DEFINED_NAME_LENGTH,
                         "%s!_FilterDatabase", worksheet->quoted_name);

            lxw_rowcol_to_range_abs(area,
                                    worksheet->autofilter.first_row,
                                    worksheet->autofilter.first_col,
                                    worksheet->autofilter.last_row,
                                    worksheet->autofilter.last_col);

            lxw_snprintf(range, LXW_DEFINED_NAME_LENGTH, "%s!%s",
                         worksheet->quoted_name, area);

            /* Autofilters are the only defined name to set the hidden flag. */
            _store_defined_name(self, "_xlnm._FilterDatabase", app_name,
                                range, worksheet->index, LXW_TRUE);
        }

        /*
         * Check for Print Area settings and store them.
         */
        if (worksheet->print_area.in_use) {

            lxw_snprintf(app_name, LXW_DEFINED_NAME_LENGTH,
                         "%s!Print_Area", worksheet->quoted_name);

            /* Check for print area that is the max row range. */
            if (worksheet->print_area.first_row == 0
                && worksheet->print_area.last_row == LXW_ROW_MAX - 1) {

                lxw_col_to_name(first_col,
                                worksheet->print_area.first_col, LXW_FALSE);

                lxw_col_to_name(last_col,
                                worksheet->print_area.last_col, LXW_FALSE);

                lxw_snprintf(area, LXW_MAX_CELL_RANGE_LENGTH - 1, "$%s:$%s",
                             first_col, last_col);

            }
            /* Check for print area that is the max column range. */
            else if (worksheet->print_area.first_col == 0
                     && worksheet->print_area.last_col == LXW_COL_MAX - 1) {

                lxw_snprintf(area, LXW_MAX_CELL_RANGE_LENGTH - 1, "$%d:$%d",
                             worksheet->print_area.first_row + 1,
                             worksheet->print_area.last_row + 1);

            }
            else {
                lxw_rowcol_to_range_abs(area,
                                        worksheet->print_area.first_row,
                                        worksheet->print_area.first_col,
                                        worksheet->print_area.last_row,
                                        worksheet->print_area.last_col);
            }

            lxw_snprintf(range, LXW_DEFINED_NAME_LENGTH, "%s!%s",
                         worksheet->quoted_name, area);

            _store_defined_name(self, "_xlnm.Print_Area", app_name,
                                range, worksheet->index, LXW_FALSE);
        }

        /*
         * Check for repeat rows/cols. aka, Print Titles and store them.
         */
        if (worksheet->repeat_rows.in_use || worksheet->repeat_cols.in_use) {
            if (worksheet->repeat_rows.in_use
                && worksheet->repeat_cols.in_use) {
                lxw_snprintf(app_name, LXW_DEFINED_NAME_LENGTH,
                             "%s!Print_Titles", worksheet->quoted_name);

                lxw_col_to_name(first_col,
                                worksheet->repeat_cols.first_col, LXW_FALSE);

                lxw_col_to_name(last_col,
                                worksheet->repeat_cols.last_col, LXW_FALSE);

                lxw_snprintf(range, LXW_DEFINED_NAME_LENGTH,
                             "%s!$%s:$%s,%s!$%d:$%d",
                             worksheet->quoted_name, first_col,
                             last_col, worksheet->quoted_name,
                             worksheet->repeat_rows.first_row + 1,
                             worksheet->repeat_rows.last_row + 1);

                _store_defined_name(self, "_xlnm.Print_Titles", app_name,
                                    range, worksheet->index, LXW_FALSE);
            }
            else if (worksheet->repeat_rows.in_use) {

                lxw_snprintf(app_name, LXW_DEFINED_NAME_LENGTH,
                             "%s!Print_Titles", worksheet->quoted_name);

                lxw_snprintf(range, LXW_DEFINED_NAME_LENGTH,
                             "%s!$%d:$%d", worksheet->quoted_name,
                             worksheet->repeat_rows.first_row + 1,
                             worksheet->repeat_rows.last_row + 1);

                _store_defined_name(self, "_xlnm.Print_Titles", app_name,
                                    range, worksheet->index, LXW_FALSE);
            }
            else if (worksheet->repeat_cols.in_use) {
                lxw_snprintf(app_name, LXW_DEFINED_NAME_LENGTH,
                             "%s!Print_Titles", worksheet->quoted_name);

                lxw_col_to_name(first_col,
                                worksheet->repeat_cols.first_col, LXW_FALSE);

                lxw_col_to_name(last_col,
                                worksheet->repeat_cols.last_col, LXW_FALSE);

                lxw_snprintf(range, LXW_DEFINED_NAME_LENGTH,
                             "%s!$%s:$%s", worksheet->quoted_name,
                             first_col, last_col);

                _store_defined_name(self, "_xlnm.Print_Titles", app_name,
                                    range, worksheet->index, LXW_FALSE);
            }
        }
    }
}

/*
 * Iterate through the worksheets and set up the table objects.
 */
STATIC void
_prepare_tables(lxw_workbook *self)
{
    lxw_worksheet *worksheet;
    lxw_sheet *sheet;
    uint32_t table_id = 0;
    uint32_t table_count = 0;

    STAILQ_FOREACH(sheet, self->sheets, list_pointers) {
        if (sheet->is_chartsheet)
            continue;
        else
            worksheet = sheet->u.worksheet;

        table_count = worksheet->table_count;

        if (table_count == 0)
            continue;

        lxw_worksheet_prepare_tables(worksheet, table_id + 1);

        table_id += table_count;
    }
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
    lxw_xml_declaration(self->file);
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

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("xmlns", xmlns);
    LXW_PUSH_ATTRIBUTES_STR("xmlns:r", xmlns_r);

    lxw_xml_start_tag(self->file, "workbook", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <fileVersion> element.
 */
STATIC void
_write_file_version(lxw_workbook *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("appName", "xl");
    LXW_PUSH_ATTRIBUTES_STR("lastEdited", "4");
    LXW_PUSH_ATTRIBUTES_STR("lowestEdited", "4");
    LXW_PUSH_ATTRIBUTES_STR("rupBuild", "4505");

    if (self->vba_project)
        LXW_PUSH_ATTRIBUTES_STR("codeName",
                                "{37E998C4-C9E5-D4B9-71C8-EB1FF731991C}");

    lxw_xml_empty_tag(self->file, "fileVersion", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <fileSharing> element.
 */
STATIC void
_workbook_write_file_sharing(lxw_workbook *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    if (self->read_only == 0)
        return;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("readOnlyRecommended", "1");

    lxw_xml_empty_tag(self->file, "fileSharing", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <workbookPr> element.
 */
STATIC void
_write_workbook_pr(lxw_workbook *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    if (self->vba_codename)
        LXW_PUSH_ATTRIBUTES_STR("codeName", self->vba_codename);

    LXW_PUSH_ATTRIBUTES_STR("defaultThemeVersion", "124226");

    lxw_xml_empty_tag(self->file, "workbookPr", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <workbookView> element.
 */
STATIC void
_write_workbook_view(lxw_workbook *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("xWindow", "240");
    LXW_PUSH_ATTRIBUTES_STR("yWindow", "15");
    LXW_PUSH_ATTRIBUTES_STR("windowWidth", "16095");
    LXW_PUSH_ATTRIBUTES_STR("windowHeight", "9660");

    if (self->first_sheet)
        LXW_PUSH_ATTRIBUTES_INT("firstSheet", self->first_sheet);

    if (self->active_sheet)
        LXW_PUSH_ATTRIBUTES_INT("activeTab", self->active_sheet);

    lxw_xml_empty_tag(self->file, "workbookView", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <bookViews> element.
 */
STATIC void
_write_book_views(lxw_workbook *self)
{
    lxw_xml_start_tag(self->file, "bookViews", NULL);

    _write_workbook_view(self);

    lxw_xml_end_tag(self->file, "bookViews");
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
    char r_id[LXW_MAX_ATTRIBUTE_LENGTH] = "rId1";

    lxw_snprintf(r_id, LXW_ATTR_32, "rId%d", sheet_id);

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("name", name);
    LXW_PUSH_ATTRIBUTES_INT("sheetId", sheet_id);

    if (hidden)
        LXW_PUSH_ATTRIBUTES_STR("state", "hidden");

    LXW_PUSH_ATTRIBUTES_STR("r:id", r_id);

    lxw_xml_empty_tag(self->file, "sheet", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <sheets> element.
 */
STATIC void
_write_sheets(lxw_workbook *self)
{
    lxw_sheet *sheet;
    lxw_worksheet *worksheet;
    lxw_chartsheet *chartsheet;

    lxw_xml_start_tag(self->file, "sheets", NULL);

    STAILQ_FOREACH(sheet, self->sheets, list_pointers) {
        if (sheet->is_chartsheet) {
            chartsheet = sheet->u.chartsheet;
            _write_sheet(self, chartsheet->name, chartsheet->index + 1,
                         chartsheet->hidden);
        }
        else {
            worksheet = sheet->u.worksheet;
            _write_sheet(self, worksheet->name, worksheet->index + 1,
                         worksheet->hidden);
        }
    }

    lxw_xml_end_tag(self->file, "sheets");
}

/*
 * Write the <calcPr> element.
 */
STATIC void
_write_calc_pr(lxw_workbook *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("calcId", "124519");
    LXW_PUSH_ATTRIBUTES_STR("fullCalcOnLoad", "1");

    lxw_xml_empty_tag(self->file, "calcPr", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <definedName> element.
 */
STATIC void
_write_defined_name(lxw_workbook *self, lxw_defined_name *defined_name)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("name", defined_name->name);

    if (defined_name->index != -1)
        LXW_PUSH_ATTRIBUTES_INT("localSheetId", defined_name->index);

    if (defined_name->hidden)
        LXW_PUSH_ATTRIBUTES_INT("hidden", 1);

    lxw_xml_data_element(self->file, "definedName", defined_name->formula,
                         &attributes);

    LXW_FREE_ATTRIBUTES();
}

STATIC void
_write_defined_names(lxw_workbook *self)
{
    lxw_defined_name *defined_name;

    if (TAILQ_EMPTY(self->defined_names))
        return;

    lxw_xml_start_tag(self->file, "definedNames", NULL);

    TAILQ_FOREACH(defined_name, self->defined_names, list_pointers) {
        _write_defined_name(self, defined_name);
    }

    lxw_xml_end_tag(self->file, "definedNames");
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
lxw_workbook_assemble_xml_file(lxw_workbook *self)
{
    /* Prepare workbook and sub-objects for writing. */
    _prepare_workbook(self);

    /* Write the XML declaration. */
    _workbook_xml_declaration(self);

    /* Write the root workbook element. */
    _write_workbook(self);

    /* Write the XLSX file version. */
    _write_file_version(self);

    /* Write the fileSharing element. */
    _workbook_write_file_sharing(self);

    /* Write the workbook properties. */
    _write_workbook_pr(self);

    /* Write the workbook view properties. */
    _write_book_views(self);

    /* Write the worksheet names and ids. */
    _write_sheets(self);

    /* Write the workbook defined names. */
    _write_defined_names(self);

    /* Write the workbook calculation properties. */
    _write_calc_pr(self);

    /* Close the workbook tag. */
    lxw_xml_end_tag(self->file, "workbook");
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
workbook_new(const char *filename)
{
    return workbook_new_opt(filename, NULL);
}

/* Deprecated function name for backwards compatibility. */
lxw_workbook *
new_workbook(const char *filename)
{
    return workbook_new_opt(filename, NULL);
}

/* Deprecated function name for backwards compatibility. */
lxw_workbook *
new_workbook_opt(const char *filename, lxw_workbook_options *options)
{
    return workbook_new_opt(filename, options);
}

/*
 * Create a new workbook object with options.
 */
lxw_workbook *
workbook_new_opt(const char *filename, lxw_workbook_options *options)
{
    lxw_format *format;
    lxw_workbook *workbook;

    /* Create the workbook object. */
    workbook = calloc(1, sizeof(lxw_workbook));
    GOTO_LABEL_ON_MEM_ERROR(workbook, mem_error);
    workbook->filename = lxw_strdup(filename);

    /* Add the sheets list. */
    workbook->sheets = calloc(1, sizeof(struct lxw_sheets));
    GOTO_LABEL_ON_MEM_ERROR(workbook->sheets, mem_error);
    STAILQ_INIT(workbook->sheets);

    /* Add the worksheets list. */
    workbook->worksheets = calloc(1, sizeof(struct lxw_worksheets));
    GOTO_LABEL_ON_MEM_ERROR(workbook->worksheets, mem_error);
    STAILQ_INIT(workbook->worksheets);

    /* Add the chartsheets list. */
    workbook->chartsheets = calloc(1, sizeof(struct lxw_chartsheets));
    GOTO_LABEL_ON_MEM_ERROR(workbook->chartsheets, mem_error);
    STAILQ_INIT(workbook->chartsheets);

    /* Add the worksheet names tree. */
    workbook->worksheet_names = calloc(1, sizeof(struct lxw_worksheet_names));
    GOTO_LABEL_ON_MEM_ERROR(workbook->worksheet_names, mem_error);
    RB_INIT(workbook->worksheet_names);

    /* Add the chartsheet names tree. */
    workbook->chartsheet_names = calloc(1,
                                        sizeof(struct lxw_chartsheet_names));
    GOTO_LABEL_ON_MEM_ERROR(workbook->chartsheet_names, mem_error);
    RB_INIT(workbook->chartsheet_names);

    /* Add the image MD5 tree. */
    workbook->image_md5s = calloc(1, sizeof(struct lxw_image_md5s));
    GOTO_LABEL_ON_MEM_ERROR(workbook->image_md5s, mem_error);
    RB_INIT(workbook->image_md5s);

    /* Add the header image MD5 tree. */
    workbook->header_image_md5s = calloc(1, sizeof(struct lxw_image_md5s));
    GOTO_LABEL_ON_MEM_ERROR(workbook->header_image_md5s, mem_error);
    RB_INIT(workbook->header_image_md5s);

    /* Add the background image MD5 tree. */
    workbook->background_md5s = calloc(1, sizeof(struct lxw_image_md5s));
    GOTO_LABEL_ON_MEM_ERROR(workbook->background_md5s, mem_error);
    RB_INIT(workbook->background_md5s);

    /* Add the charts list. */
    workbook->charts = calloc(1, sizeof(struct lxw_charts));
    GOTO_LABEL_ON_MEM_ERROR(workbook->charts, mem_error);
    STAILQ_INIT(workbook->charts);

    /* Add the ordered charts list to track chart insertion order. */
    workbook->ordered_charts = calloc(1, sizeof(struct lxw_charts));
    GOTO_LABEL_ON_MEM_ERROR(workbook->ordered_charts, mem_error);
    STAILQ_INIT(workbook->ordered_charts);

    /* Add the formats list. */
    workbook->formats = calloc(1, sizeof(struct lxw_formats));
    GOTO_LABEL_ON_MEM_ERROR(workbook->formats, mem_error);
    STAILQ_INIT(workbook->formats);

    /* Add the defined_names list. */
    workbook->defined_names = calloc(1, sizeof(struct lxw_defined_names));
    GOTO_LABEL_ON_MEM_ERROR(workbook->defined_names, mem_error);
    TAILQ_INIT(workbook->defined_names);

    /* Add the shared strings table. */
    workbook->sst = lxw_sst_new();
    GOTO_LABEL_ON_MEM_ERROR(workbook->sst, mem_error);

    /* Add the default workbook properties. */
    workbook->properties = calloc(1, sizeof(lxw_doc_properties));
    GOTO_LABEL_ON_MEM_ERROR(workbook->properties, mem_error);

    /* Add a hash table to track format indices. */
    workbook->used_xf_formats = lxw_hash_new(128, 1, 0);
    GOTO_LABEL_ON_MEM_ERROR(workbook->used_xf_formats, mem_error);

    /* Add a hash table to track format indices. */
    workbook->used_dxf_formats = lxw_hash_new(128, 1, 0);
    GOTO_LABEL_ON_MEM_ERROR(workbook->used_dxf_formats, mem_error);

    /* Add the worksheets list. */
    workbook->custom_properties =
        calloc(1, sizeof(struct lxw_custom_properties));
    GOTO_LABEL_ON_MEM_ERROR(workbook->custom_properties, mem_error);
    STAILQ_INIT(workbook->custom_properties);

    /* Add the default cell format. */
    format = workbook_add_format(workbook);
    GOTO_LABEL_ON_MEM_ERROR(format, mem_error);

    /* Initialize its index. */
    lxw_format_get_xf_index(format);

    /* Add the default hyperlink format. */
    format = workbook_add_format(workbook);
    GOTO_LABEL_ON_MEM_ERROR(format, mem_error);
    format_set_hyperlink(format);
    workbook->default_url_format = format;

    if (options) {
        workbook->options.constant_memory = options->constant_memory;
        workbook->options.tmpdir = lxw_strdup(options->tmpdir);
        workbook->options.use_zip64 = options->use_zip64;
        workbook->options.output_buffer = options->output_buffer;
        workbook->options.output_buffer_size = options->output_buffer_size;
    }

    workbook->max_url_length = 2079;

    return workbook;

mem_error:
    lxw_workbook_free(workbook);
    workbook = NULL;
    return NULL;
}

/*
 * Add a new worksheet to the Excel workbook.
 */
lxw_worksheet *
workbook_add_worksheet(lxw_workbook *self, const char *sheetname)
{
    lxw_sheet *sheet = NULL;
    lxw_worksheet *worksheet = NULL;
    lxw_worksheet_name *worksheet_name = NULL;
    lxw_error error;
    lxw_worksheet_init_data init_data = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char *new_name = NULL;

    if (sheetname) {
        /* Use the user supplied name. */
        init_data.name = lxw_strdup(sheetname);
        init_data.quoted_name = lxw_quote_sheetname(sheetname);
    }
    else {
        /* Use the default SheetN name. */
        new_name = malloc(LXW_MAX_SHEETNAME_LENGTH);
        GOTO_LABEL_ON_MEM_ERROR(new_name, mem_error);

        lxw_snprintf(new_name, LXW_MAX_SHEETNAME_LENGTH, "Sheet%d",
                     self->num_worksheets + 1);
        init_data.name = new_name;
        init_data.quoted_name = lxw_strdup(new_name);
    }

    /* Check that the worksheet name is valid. */
    error = workbook_validate_sheet_name(self, init_data.name);
    if (error) {
        LXW_WARN_FORMAT2("workbook_add_worksheet(): worksheet name '%s' has "
                         "error: %s", init_data.name, lxw_strerror(error));
        goto mem_error;
    }

    /* Create a struct to find/store the worksheet name/pointer. */
    worksheet_name = calloc(1, sizeof(struct lxw_worksheet_name));
    GOTO_LABEL_ON_MEM_ERROR(worksheet_name, mem_error);

    /* Initialize the metadata to pass to the worksheet. */
    init_data.hidden = 0;
    init_data.index = self->num_sheets;
    init_data.sst = self->sst;
    init_data.optimize = self->options.constant_memory;
    init_data.active_sheet = &self->active_sheet;
    init_data.first_sheet = &self->first_sheet;
    init_data.tmpdir = self->options.tmpdir;
    init_data.default_url_format = self->default_url_format;
    init_data.max_url_length = self->max_url_length;

    /* Create a new worksheet object. */
    worksheet = lxw_worksheet_new(&init_data);
    GOTO_LABEL_ON_MEM_ERROR(worksheet, mem_error);

    /* Add it to the worksheet list. */
    self->num_worksheets++;
    STAILQ_INSERT_TAIL(self->worksheets, worksheet, list_pointers);

    /* Create a new sheet object. */
    sheet = calloc(1, sizeof(lxw_sheet));
    GOTO_LABEL_ON_MEM_ERROR(sheet, mem_error);
    sheet->u.worksheet = worksheet;

    /* Add it to the worksheet list. */
    self->num_sheets++;
    STAILQ_INSERT_TAIL(self->sheets, sheet, list_pointers);

    /* Store the worksheet so we can look it up by name. */
    worksheet_name->name = init_data.name;
    worksheet_name->worksheet = worksheet;
    RB_INSERT(lxw_worksheet_names, self->worksheet_names, worksheet_name);

    return worksheet;

mem_error:
    free(init_data.name);
    free(init_data.quoted_name);
    free(worksheet_name);
    free(worksheet);
    return NULL;
}

/*
 * Add a new chartsheet to the Excel workbook.
 */
lxw_chartsheet *
workbook_add_chartsheet(lxw_workbook *self, const char *sheetname)
{
    lxw_sheet *sheet = NULL;
    lxw_chartsheet *chartsheet = NULL;
    lxw_chartsheet_name *chartsheet_name = NULL;
    lxw_error error;
    lxw_worksheet_init_data init_data = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char *new_name = NULL;

    if (sheetname) {
        /* Use the user supplied name. */
        init_data.name = lxw_strdup(sheetname);
        init_data.quoted_name = lxw_quote_sheetname(sheetname);
    }
    else {
        /* Use the default SheetN name. */
        new_name = malloc(LXW_MAX_SHEETNAME_LENGTH);
        GOTO_LABEL_ON_MEM_ERROR(new_name, mem_error);

        lxw_snprintf(new_name, LXW_MAX_SHEETNAME_LENGTH, "Chart%d",
                     self->num_chartsheets + 1);
        init_data.name = new_name;
        init_data.quoted_name = lxw_strdup(new_name);
    }

    /* Check that the chartsheet name is valid. */
    error = workbook_validate_sheet_name(self, init_data.name);
    if (error) {
        LXW_WARN_FORMAT2
            ("workbook_add_chartsheet(): chartsheet name '%s' has "
             "error: %s", init_data.name, lxw_strerror(error));
        goto mem_error;
    }

    /* Create a struct to find/store the chartsheet name/pointer. */
    chartsheet_name = calloc(1, sizeof(struct lxw_chartsheet_name));
    GOTO_LABEL_ON_MEM_ERROR(chartsheet_name, mem_error);

    /* Initialize the metadata to pass to the chartsheet. */
    init_data.hidden = 0;
    init_data.index = self->num_sheets;
    init_data.sst = self->sst;
    init_data.optimize = self->options.constant_memory;
    init_data.active_sheet = &self->active_sheet;
    init_data.first_sheet = &self->first_sheet;
    init_data.tmpdir = self->options.tmpdir;

    /* Create a new chartsheet object. */
    chartsheet = lxw_chartsheet_new(&init_data);
    GOTO_LABEL_ON_MEM_ERROR(chartsheet, mem_error);

    /* Add it to the chartsheet list. */
    self->num_chartsheets++;
    STAILQ_INSERT_TAIL(self->chartsheets, chartsheet, list_pointers);

    /* Create a new sheet object. */
    sheet = calloc(1, sizeof(lxw_sheet));
    GOTO_LABEL_ON_MEM_ERROR(sheet, mem_error);
    sheet->is_chartsheet = LXW_TRUE;
    sheet->u.chartsheet = chartsheet;

    /* Add it to the chartsheet list. */
    self->num_sheets++;
    STAILQ_INSERT_TAIL(self->sheets, sheet, list_pointers);

    /* Store the chartsheet so we can look it up by name. */
    chartsheet_name->name = init_data.name;
    chartsheet_name->chartsheet = chartsheet;
    RB_INSERT(lxw_chartsheet_names, self->chartsheet_names, chartsheet_name);

    return chartsheet;

mem_error:
    free(init_data.name);
    free(init_data.quoted_name);
    free(chartsheet_name);
    free(chartsheet);
    return NULL;
}

/*
 * Add a new chart to the Excel workbook.
 */
lxw_chart *
workbook_add_chart(lxw_workbook *self, uint8_t type)
{
    lxw_chart *chart;

    /* Create a new chart object. */
    chart = lxw_chart_new(type);

    if (chart)
        STAILQ_INSERT_TAIL(self->charts, chart, list_pointers);

    return chart;
}

/*
 * Add a new format to the Excel workbook.
 */
lxw_format *
workbook_add_format(lxw_workbook *self)
{
    /* Create a new format object. */
    lxw_format *format = lxw_format_new();
    RETURN_ON_MEM_ERROR(format, NULL);

    format->xf_format_indices = self->used_xf_formats;
    format->dxf_format_indices = self->used_dxf_formats;
    format->num_xf_formats = &self->num_xf_formats;

    STAILQ_INSERT_TAIL(self->formats, format, list_pointers);

    return format;
}

/*
 * Call finalization code and close file.
 */
lxw_error
workbook_close(lxw_workbook *self)
{
    lxw_sheet *sheet = NULL;
    lxw_worksheet *worksheet = NULL;
    lxw_packager *packager = NULL;
    lxw_error error = LXW_NO_ERROR;
    char codename[LXW_MAX_SHEETNAME_LENGTH] = { 0 };

    /* Add a default worksheet if non have been added. */
    if (!self->num_sheets)
        workbook_add_worksheet(self, NULL);

    /* Ensure that at least one worksheet has been selected. */
    if (self->active_sheet == 0) {
        sheet = STAILQ_FIRST(self->sheets);
        if (!sheet->is_chartsheet) {
            worksheet = sheet->u.worksheet;
            worksheet->selected = LXW_TRUE;
            worksheet->hidden = 0;
        }
    }

    /* Set the active sheet and check if a metadata file is needed. */
    STAILQ_FOREACH(sheet, self->sheets, list_pointers) {
        if (sheet->is_chartsheet)
            continue;
        else
            worksheet = sheet->u.worksheet;

        if (worksheet->index == self->active_sheet)
            worksheet->active = LXW_TRUE;

        if (worksheet->has_dynamic_arrays)
            self->has_metadata = LXW_TRUE;
    }

    /* Set workbook and worksheet VBA codenames if a macro has been added. */
    if (self->vba_project) {
        if (!self->vba_codename)
            workbook_set_vba_name(self, "ThisWorkbook");

        STAILQ_FOREACH(sheet, self->sheets, list_pointers) {
            if (sheet->is_chartsheet)
                continue;
            else
                worksheet = sheet->u.worksheet;

            if (!worksheet->vba_codename) {
                lxw_snprintf(codename, LXW_MAX_SHEETNAME_LENGTH, "Sheet%d",
                             worksheet->index + 1);

                worksheet_set_vba_name(worksheet, codename);
            }
        }
    }

    /* Prepare the worksheet VML elements such as comments. */
    _prepare_vml(self);

    /* Set the defined names for the worksheets such as Print Titles. */
    _prepare_defined_names(self);

    /* Prepare the drawings, charts and images. */
    _prepare_drawings(self);

    /* Add cached data to charts. */
    _add_chart_cache_data(self);

    /* Set the table ids for the worksheet tables. */
    _prepare_tables(self);

    /* Create a packager object to assemble sub-elements into a zip file. */
    packager = lxw_packager_new(self->filename,
                                self->options.tmpdir,
                                self->options.use_zip64);

    /* If the packager fails it is generally due to a zip permission error. */
    if (packager == NULL) {
        LXW_PRINTF(LXW_STDERR "[ERROR] workbook_close(): "
                   "Error creating '%s'. "
                   "System error = %s\n", self->filename, strerror(errno));

        error = LXW_ERROR_CREATING_XLSX_FILE;
        goto mem_error;
    }

    /* Set the workbook object in the packager. */
    packager->workbook = self;

    /* Assemble all the sub-files in the xlsx package. */
    error = lxw_create_package(packager);

    if (!self->filename) {
        *self->options.output_buffer = packager->output_buffer;
        *self->options.output_buffer_size = packager->output_buffer_size;
    }

    /* Error and non-error conditions fall through to the cleanup code. */
    if (error == LXW_ERROR_CREATING_TMPFILE) {
        LXW_PRINTF(LXW_STDERR "[ERROR] workbook_close(): "
                   "Error creating tmpfile(s) to assemble '%s'. "
                   "System error = %s\n", self->filename, strerror(errno));
    }

    /* If LXW_ERROR_ZIP_FILE_OPERATION then errno is set by zip. */
    if (error == LXW_ERROR_ZIP_FILE_OPERATION) {
        LXW_PRINTF(LXW_STDERR "[ERROR] workbook_close(): "
                   "Zip ZIP_ERRNO error while creating xlsx file '%s'. "
                   "System error = %s\n", self->filename, strerror(errno));
    }

    /* If LXW_ERROR_ZIP_PARAMETER_ERROR then errno is set by zip. */
    if (error == LXW_ERROR_ZIP_PARAMETER_ERROR) {
        LXW_PRINTF(LXW_STDERR "[ERROR] workbook_close(): "
                   "Zip ZIP_PARAMERROR error while creating xlsx file '%s'. "
                   "System error = %s\n", self->filename, strerror(errno));
    }

    /* If LXW_ERROR_ZIP_BAD_ZIP_FILE then errno is set by zip. */
    if (error == LXW_ERROR_ZIP_BAD_ZIP_FILE) {
        LXW_PRINTF(LXW_STDERR "[ERROR] workbook_close(): "
                   "Zip ZIP_BADZIPFILE error while creating xlsx file '%s'. "
                   "This may require the use_zip64 option for large files. "
                   "System error = %s\n", self->filename, strerror(errno));
    }

    /* If LXW_ERROR_ZIP_INTERNAL_ERROR then errno is set by zip. */
    if (error == LXW_ERROR_ZIP_INTERNAL_ERROR) {
        LXW_PRINTF(LXW_STDERR "[ERROR] workbook_close(): "
                   "Zip ZIP_INTERNALERROR error while creating xlsx file '%s'. "
                   "System error = %s\n", self->filename, strerror(errno));
    }

    /* The next 2 error conditions don't set errno. */
    if (error == LXW_ERROR_ZIP_FILE_ADD) {
        LXW_PRINTF(LXW_STDERR "[ERROR] workbook_close(): "
                   "Zip error adding file to xlsx file '%s'.\n",
                   self->filename);
    }

    if (error == LXW_ERROR_ZIP_CLOSE) {
        LXW_PRINTF(LXW_STDERR "[ERROR] workbook_close(): "
                   "Zip error closing xlsx file '%s'.\n", self->filename);
    }

mem_error:
    lxw_packager_free(packager);
    lxw_workbook_free(self);
    return error;
}

/*
 * Create a defined name in Excel. We handle global/workbook level names and
 * local/worksheet names.
 */
lxw_error
workbook_define_name(lxw_workbook *self, const char *name,
                     const char *formula)
{
    return _store_defined_name(self, name, NULL, formula, -1, LXW_FALSE);
}

/*
 * Set the document properties such as Title, Author etc.
 */
lxw_error
workbook_set_properties(lxw_workbook *self, lxw_doc_properties *user_props)
{
    lxw_doc_properties *doc_props;

    /* Free any existing properties. */
    _free_doc_properties(self->properties);

    doc_props = calloc(1, sizeof(lxw_doc_properties));
    GOTO_LABEL_ON_MEM_ERROR(doc_props, mem_error);

    /* Copy the user properties to an internal structure. */
    if (user_props->title) {
        doc_props->title = lxw_strdup(user_props->title);
        GOTO_LABEL_ON_MEM_ERROR(doc_props->title, mem_error);
    }

    if (user_props->subject) {
        doc_props->subject = lxw_strdup(user_props->subject);
        GOTO_LABEL_ON_MEM_ERROR(doc_props->subject, mem_error);
    }

    if (user_props->author) {
        doc_props->author = lxw_strdup(user_props->author);
        GOTO_LABEL_ON_MEM_ERROR(doc_props->author, mem_error);
    }

    if (user_props->manager) {
        doc_props->manager = lxw_strdup(user_props->manager);
        GOTO_LABEL_ON_MEM_ERROR(doc_props->manager, mem_error);
    }

    if (user_props->company) {
        doc_props->company = lxw_strdup(user_props->company);
        GOTO_LABEL_ON_MEM_ERROR(doc_props->company, mem_error);
    }

    if (user_props->category) {
        doc_props->category = lxw_strdup(user_props->category);
        GOTO_LABEL_ON_MEM_ERROR(doc_props->category, mem_error);
    }

    if (user_props->keywords) {
        doc_props->keywords = lxw_strdup(user_props->keywords);
        GOTO_LABEL_ON_MEM_ERROR(doc_props->keywords, mem_error);
    }

    if (user_props->comments) {
        doc_props->comments = lxw_strdup(user_props->comments);
        GOTO_LABEL_ON_MEM_ERROR(doc_props->comments, mem_error);
    }

    if (user_props->status) {
        doc_props->status = lxw_strdup(user_props->status);
        GOTO_LABEL_ON_MEM_ERROR(doc_props->status, mem_error);
    }

    if (user_props->hyperlink_base) {
        doc_props->hyperlink_base = lxw_strdup(user_props->hyperlink_base);
        GOTO_LABEL_ON_MEM_ERROR(doc_props->hyperlink_base, mem_error);
    }

    doc_props->created = user_props->created;

    self->properties = doc_props;

    return LXW_NO_ERROR;

mem_error:
    _free_doc_properties(doc_props);
    return LXW_ERROR_MEMORY_MALLOC_FAILED;
}

/*
 * Set a string custom document property.
 */
lxw_error
workbook_set_custom_property_string(lxw_workbook *self, const char *name,
                                    const char *value)
{
    lxw_custom_property *custom_property;

    if (!name) {
        LXW_WARN_FORMAT("workbook_set_custom_property_string(): "
                        "parameter 'name' cannot be NULL.");
        return LXW_ERROR_NULL_PARAMETER_IGNORED;
    }

    if (!value) {
        LXW_WARN_FORMAT("workbook_set_custom_property_string(): "
                        "parameter 'value' cannot be NULL.");
        return LXW_ERROR_NULL_PARAMETER_IGNORED;
    }

    if (lxw_utf8_strlen(name) > 255) {
        LXW_WARN_FORMAT("workbook_set_custom_property_string(): parameter "
                        "'name' exceeds Excel length limit of 255.");
        return LXW_ERROR_255_STRING_LENGTH_EXCEEDED;
    }

    if (lxw_utf8_strlen(value) > 255) {
        LXW_WARN_FORMAT("workbook_set_custom_property_string(): parameter "
                        "'value' exceeds Excel length limit of 255.");
        return LXW_ERROR_255_STRING_LENGTH_EXCEEDED;
    }

    /* Create a struct to hold the custom property. */
    custom_property = calloc(1, sizeof(struct lxw_custom_property));
    RETURN_ON_MEM_ERROR(custom_property, LXW_ERROR_MEMORY_MALLOC_FAILED);

    custom_property->name = lxw_strdup(name);
    custom_property->u.string = lxw_strdup(value);
    custom_property->type = LXW_CUSTOM_STRING;

    STAILQ_INSERT_TAIL(self->custom_properties, custom_property,
                       list_pointers);

    return LXW_NO_ERROR;
}

/*
 * Set a double number custom document property.
 */
lxw_error
workbook_set_custom_property_number(lxw_workbook *self, const char *name,
                                    double value)
{
    lxw_custom_property *custom_property;

    if (!name) {
        LXW_WARN_FORMAT("workbook_set_custom_property_number(): parameter "
                        "'name' cannot be NULL.");
        return LXW_ERROR_NULL_PARAMETER_IGNORED;
    }

    if (lxw_utf8_strlen(name) > 255) {
        LXW_WARN_FORMAT("workbook_set_custom_property_number(): parameter "
                        "'name' exceeds Excel length limit of 255.");
        return LXW_ERROR_255_STRING_LENGTH_EXCEEDED;
    }

    /* Create a struct to hold the custom property. */
    custom_property = calloc(1, sizeof(struct lxw_custom_property));
    RETURN_ON_MEM_ERROR(custom_property, LXW_ERROR_MEMORY_MALLOC_FAILED);

    custom_property->name = lxw_strdup(name);
    custom_property->u.number = value;
    custom_property->type = LXW_CUSTOM_DOUBLE;

    STAILQ_INSERT_TAIL(self->custom_properties, custom_property,
                       list_pointers);

    return LXW_NO_ERROR;
}

/*
 * Set a integer number custom document property.
 */
lxw_error
workbook_set_custom_property_integer(lxw_workbook *self, const char *name,
                                     int32_t value)
{
    lxw_custom_property *custom_property;

    if (!name) {
        LXW_WARN_FORMAT("workbook_set_custom_property_integer(): parameter "
                        "'name' cannot be NULL.");
        return LXW_ERROR_NULL_PARAMETER_IGNORED;
    }

    if (strlen(name) > 255) {
        LXW_WARN_FORMAT("workbook_set_custom_property_integer(): parameter "
                        "'name' exceeds Excel length limit of 255.");
        return LXW_ERROR_255_STRING_LENGTH_EXCEEDED;
    }

    /* Create a struct to hold the custom property. */
    custom_property = calloc(1, sizeof(struct lxw_custom_property));
    RETURN_ON_MEM_ERROR(custom_property, LXW_ERROR_MEMORY_MALLOC_FAILED);

    custom_property->name = lxw_strdup(name);
    custom_property->u.integer = value;
    custom_property->type = LXW_CUSTOM_INTEGER;

    STAILQ_INSERT_TAIL(self->custom_properties, custom_property,
                       list_pointers);

    return LXW_NO_ERROR;
}

/*
 * Set a boolean custom document property.
 */
lxw_error
workbook_set_custom_property_boolean(lxw_workbook *self, const char *name,
                                     uint8_t value)
{
    lxw_custom_property *custom_property;

    if (!name) {
        LXW_WARN_FORMAT("workbook_set_custom_property_boolean(): parameter "
                        "'name' cannot be NULL.");
        return LXW_ERROR_NULL_PARAMETER_IGNORED;
    }

    if (lxw_utf8_strlen(name) > 255) {
        LXW_WARN_FORMAT("workbook_set_custom_property_boolean(): parameter "
                        "'name' exceeds Excel length limit of 255.");
        return LXW_ERROR_255_STRING_LENGTH_EXCEEDED;
    }

    /* Create a struct to hold the custom property. */
    custom_property = calloc(1, sizeof(struct lxw_custom_property));
    RETURN_ON_MEM_ERROR(custom_property, LXW_ERROR_MEMORY_MALLOC_FAILED);

    custom_property->name = lxw_strdup(name);
    custom_property->u.boolean = value;
    custom_property->type = LXW_CUSTOM_BOOLEAN;

    STAILQ_INSERT_TAIL(self->custom_properties, custom_property,
                       list_pointers);

    return LXW_NO_ERROR;
}

/*
 * Set a datetime custom document property.
 */
lxw_error
workbook_set_custom_property_datetime(lxw_workbook *self, const char *name,
                                      lxw_datetime *datetime)
{
    lxw_custom_property *custom_property;

    if (!name) {
        LXW_WARN_FORMAT("workbook_set_custom_property_datetime(): parameter "
                        "'name' cannot be NULL.");
        return LXW_ERROR_NULL_PARAMETER_IGNORED;
    }

    if (lxw_utf8_strlen(name) > 255) {
        LXW_WARN_FORMAT("workbook_set_custom_property_datetime(): parameter "
                        "'name' exceeds Excel length limit of 255.");
        return LXW_ERROR_NULL_PARAMETER_IGNORED;
    }

    if (!datetime) {
        LXW_WARN_FORMAT("workbook_set_custom_property_datetime(): parameter "
                        "'datetime' cannot be NULL.");
        return LXW_ERROR_NULL_PARAMETER_IGNORED;
    }

    /* Create a struct to hold the custom property. */
    custom_property = calloc(1, sizeof(struct lxw_custom_property));
    RETURN_ON_MEM_ERROR(custom_property, LXW_ERROR_MEMORY_MALLOC_FAILED);

    custom_property->name = lxw_strdup(name);

    memcpy(&custom_property->u.datetime, datetime, sizeof(lxw_datetime));
    custom_property->type = LXW_CUSTOM_DATETIME;

    STAILQ_INSERT_TAIL(self->custom_properties, custom_property,
                       list_pointers);

    return LXW_NO_ERROR;
}

/*
 * Get a worksheet object from its name.
 */
lxw_worksheet *
workbook_get_worksheet_by_name(lxw_workbook *self, const char *name)
{
    lxw_worksheet_name worksheet_name;
    lxw_worksheet_name *found;

    if (!name)
        return NULL;

    worksheet_name.name = name;
    found = RB_FIND(lxw_worksheet_names,
                    self->worksheet_names, &worksheet_name);

    if (found)
        return found->worksheet;
    else
        return NULL;
}

/*
 * Get a chartsheet object from its name.
 */
lxw_chartsheet *
workbook_get_chartsheet_by_name(lxw_workbook *self, const char *name)
{
    lxw_chartsheet_name chartsheet_name;
    lxw_chartsheet_name *found;

    if (!name)
        return NULL;

    chartsheet_name.name = name;
    found = RB_FIND(lxw_chartsheet_names,
                    self->chartsheet_names, &chartsheet_name);

    if (found)
        return found->chartsheet;
    else
        return NULL;
}

/*
 * Get the default URL format.
 */
lxw_format *
workbook_get_default_url_format(lxw_workbook *self)
{
    return self->default_url_format;
}

/*
 * Unset the default URL format.
 */
void
workbook_unset_default_url_format(lxw_workbook *self)
{
    self->default_url_format->hyperlink = LXW_FALSE;
    self->default_url_format->xf_id = 0;
    self->default_url_format->underline = LXW_UNDERLINE_NONE;
    self->default_url_format->theme = 0;
}

/*
 * Validate the worksheet name based on Excel's rules.
 */
lxw_error
workbook_validate_sheet_name(lxw_workbook *self, const char *sheetname)
{
    /* Check the UTF-8 length of the worksheet name. */
    if (lxw_utf8_strlen(sheetname) > LXW_SHEETNAME_MAX)
        return LXW_ERROR_SHEETNAME_LENGTH_EXCEEDED;

    /* Check that the worksheet name doesn't contain invalid characters. */
    if (strpbrk(sheetname, "[]:*?/\\"))
        return LXW_ERROR_INVALID_SHEETNAME_CHARACTER;

    /* Check that the worksheet doesn't start or end with an apostrophe. */
    if (sheetname[0] == '\'' || sheetname[strlen(sheetname) - 1] == '\'')
        return LXW_ERROR_SHEETNAME_START_END_APOSTROPHE;

    /* Check if the worksheet name is already in use. */
    if (workbook_get_worksheet_by_name(self, sheetname))
        return LXW_ERROR_SHEETNAME_ALREADY_USED;

    /* Check if the chartsheet name is already in use. */
    if (workbook_get_chartsheet_by_name(self, sheetname))
        return LXW_ERROR_SHEETNAME_ALREADY_USED;

    return LXW_NO_ERROR;
}

/*
 * Add a vbaProject binary to the Excel workbook.
 */
lxw_error
workbook_add_vba_project(lxw_workbook *self, const char *filename)
{
    FILE *filehandle;

    if (!filename) {
        LXW_WARN("workbook_add_vba_project(): "
                 "filename must be specified.");
        return LXW_ERROR_NULL_PARAMETER_IGNORED;
    }

    /* Check that the vbaProject file exists and can be opened. */
    filehandle = lxw_fopen(filename, "rb");
    if (!filehandle) {
        LXW_WARN_FORMAT1("workbook_add_vba_project(): "
                         "file doesn't exist or can't be opened: %s.",
                         filename);
        return LXW_ERROR_PARAMETER_VALIDATION;
    }
    fclose(filehandle);

    self->vba_project = lxw_strdup(filename);

    return LXW_NO_ERROR;
}

/*
 * Set the VBA name for the workbook.
 */
lxw_error
workbook_set_vba_name(lxw_workbook *self, const char *name)
{
    if (!name) {
        LXW_WARN("workbook_set_vba_name(): " "name must be specified.");
        return LXW_ERROR_NULL_PARAMETER_IGNORED;
    }

    self->vba_codename = lxw_strdup(name);

    return LXW_NO_ERROR;
}

/*
 * Set the Excel "Read-only recommended" save option.
 */
void
workbook_read_only_recommended(lxw_workbook *self)
{
    self->read_only = 2;
}
