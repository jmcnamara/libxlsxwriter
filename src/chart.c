/*****************************************************************************
 * chart - A library for creating Excel XLSX chart files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2017, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/chart.h"
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
 * Free a series range object.
 */
STATIC void
_chart_free_range(lxw_series_range *range)
{
    struct lxw_series_data_point *data_point;

    if (!range)
        return;

    while (!STAILQ_EMPTY(range->data_cache)) {
        data_point = STAILQ_FIRST(range->data_cache);
        free(data_point->string);
        STAILQ_REMOVE_HEAD(range->data_cache, list_pointers);

        free(data_point);
    }

    free(range->data_cache);
    free(range->formula);
    free(range->sheetname);
    free(range);
}

/*
 * Free a series object.
 */
STATIC void
_chart_series_free(lxw_chart_series *series)
{
    if (!series)
        return;

    free(series->title.name);
    free(series->line);
    free(series->fill);
    free(series->pattern);

    if (series->marker) {
        free(series->marker->line);
        free(series->marker->fill);
        free(series->marker->pattern);
        free(series->marker);
    }

    _chart_free_range(series->categories);
    _chart_free_range(series->values);
    _chart_free_range(series->title.range);

    free(series);
}

/*
 * Initialize the data cache in a range object.
 */
STATIC int
_chart_init_data_cache(lxw_series_range *range)
{
    /* Initialize the series range data cache. */
    range->data_cache = calloc(1, sizeof(struct lxw_series_data_points));
    RETURN_ON_MEM_ERROR(range->data_cache, -1);
    STAILQ_INIT(range->data_cache);

    return 0;
}

/*
 * Free a chart font object.
 */
STATIC void
_chart_free_font(lxw_chart_font *font)
{
    if (!font)
        return;

    free(font->name);
    free(font);
}

/*
 * Free a chart object.
 */
void
lxw_chart_free(lxw_chart *chart)
{
    lxw_chart_series *series;

    if (!chart)
        return;

    /* Chart series. */
    if (chart->series_list) {
        while (!STAILQ_EMPTY(chart->series_list)) {
            series = STAILQ_FIRST(chart->series_list);
            STAILQ_REMOVE_HEAD(chart->series_list, list_pointers);

            _chart_series_free(series);
        }

        free(chart->series_list);
    }

    /* X Axis. */
    if (chart->x_axis) {
        _chart_free_font(chart->x_axis->title.font);
        _chart_free_font(chart->x_axis->num_font);
        _chart_free_range(chart->x_axis->title.range);
        free(chart->x_axis->title.name);
        free(chart->x_axis->line);
        free(chart->x_axis->fill);
        free(chart->x_axis->pattern);
        free(chart->x_axis->major_gridlines.line);
        free(chart->x_axis->minor_gridlines.line);
        free(chart->x_axis);
    }

    /* Y Axis. */
    if (chart->y_axis) {
        _chart_free_font(chart->y_axis->title.font);
        _chart_free_font(chart->y_axis->num_font);
        _chart_free_range(chart->y_axis->title.range);
        free(chart->y_axis->title.name);
        free(chart->y_axis->line);
        free(chart->y_axis->fill);
        free(chart->y_axis->pattern);
        free(chart->y_axis->major_gridlines.line);
        free(chart->y_axis->minor_gridlines.line);
        free(chart->y_axis);
    }

    /* Chart title. */
    _chart_free_font(chart->title.font);
    _chart_free_range(chart->title.range);
    free(chart->title.name);

    /* Chart legend. */
    _chart_free_font(chart->legend.font);
    free(chart->delete_series);

    free(chart->default_marker);

    free(chart->chartarea_line);
    free(chart->chartarea_fill);
    free(chart->chartarea_pattern);
    free(chart->plotarea_line);
    free(chart->plotarea_fill);
    free(chart->plotarea_pattern);

    free(chart);
}

/*
 * Create a new chart object.
 */
lxw_chart *
lxw_chart_new(uint8_t type)
{
    lxw_chart *chart = calloc(1, sizeof(lxw_chart));
    GOTO_LABEL_ON_MEM_ERROR(chart, mem_error);

    chart->series_list = calloc(1, sizeof(struct lxw_chart_series_list));
    GOTO_LABEL_ON_MEM_ERROR(chart->series_list, mem_error);
    STAILQ_INIT(chart->series_list);

    chart->x_axis = calloc(1, sizeof(struct lxw_chart_axis));
    GOTO_LABEL_ON_MEM_ERROR(chart->x_axis, mem_error);

    chart->y_axis = calloc(1, sizeof(struct lxw_chart_axis));
    GOTO_LABEL_ON_MEM_ERROR(chart->y_axis, mem_error);

    chart->title.range = calloc(1, sizeof(lxw_series_range));
    GOTO_LABEL_ON_MEM_ERROR(chart->title.range, mem_error);

    chart->x_axis->title.range = calloc(1, sizeof(lxw_series_range));
    GOTO_LABEL_ON_MEM_ERROR(chart->x_axis->title.range, mem_error);

    chart->y_axis->title.range = calloc(1, sizeof(lxw_series_range));
    GOTO_LABEL_ON_MEM_ERROR(chart->y_axis->title.range, mem_error);

    /* Initialize the ranges in the chart titles. */
    if (_chart_init_data_cache(chart->title.range) != LXW_NO_ERROR)
        goto mem_error;

    if (_chart_init_data_cache(chart->x_axis->title.range) != LXW_NO_ERROR)
        goto mem_error;

    if (_chart_init_data_cache(chart->y_axis->title.range) != LXW_NO_ERROR)
        goto mem_error;

    chart->type = type;
    chart->style_id = 2;
    chart->hole_size = 50;

    /* Set the default axis positions. */
    chart->cat_axis_position = LXW_CHART_AXIS_BOTTOM;
    chart->val_axis_position = LXW_CHART_AXIS_LEFT;

    lxw_strcpy(chart->x_axis->default_num_format, "General");
    lxw_strcpy(chart->y_axis->default_num_format, "General");

    chart->x_axis->major_gridlines.visible = LXW_FALSE;
    chart->y_axis->major_gridlines.visible = LXW_TRUE;

    chart->series_overlap_1 = 100;

    chart->has_horiz_cat_axis = LXW_FALSE;
    chart->has_horiz_val_axis = LXW_TRUE;

    chart->legend.position = LXW_CHART_LEGEND_RIGHT;

    return chart;

mem_error:
    lxw_chart_free(chart);
    return NULL;
}

/*
 * Create a copy of a user supplied font.
 */
STATIC lxw_chart_font *
_chart_convert_font_args(lxw_chart_font *user_font)
{
    lxw_chart_font *font;

    if (!user_font)
        return NULL;

    font = calloc(1, sizeof(struct lxw_chart_font));
    RETURN_ON_MEM_ERROR(font, NULL);

    memcpy(font, user_font, sizeof(lxw_chart_font));

    font->name = lxw_strdup(user_font->name);

    /* Convert font size units. */
    if (font->size)
        font->size = font->size * 100;

    /* Convert rotation into 60,000ths of a degree. */
    if (font->rotation)
        font->rotation = font->rotation * 60000;

    if (font->color) {
        font->color = lxw_format_check_color(font->color);
        font->has_color = LXW_TRUE;
    }

    return font;
}

/*
 * Create a copy of a user supplied line.
 */
STATIC lxw_chart_line *
_chart_convert_line_args(lxw_chart_line *user_line)
{
    lxw_chart_line *line;

    if (!user_line)
        return NULL;

    line = calloc(1, sizeof(struct lxw_chart_line));
    RETURN_ON_MEM_ERROR(line, NULL);

    memcpy(line, user_line, sizeof(lxw_chart_line));

    if (line->color) {
        line->color = lxw_format_check_color(line->color);
        line->has_color = LXW_TRUE;
    }

    if (line->transparency > 100)
        line->transparency = 0;

    return line;
}

/*
 * Create a copy of a user supplied fill.
 */
STATIC lxw_chart_fill *
_chart_convert_fill_args(lxw_chart_fill *user_fill)
{
    lxw_chart_fill *fill;

    if (!user_fill)
        return NULL;

    fill = calloc(1, sizeof(struct lxw_chart_fill));
    RETURN_ON_MEM_ERROR(fill, NULL);

    memcpy(fill, user_fill, sizeof(lxw_chart_fill));

    if (fill->color) {
        fill->color = lxw_format_check_color(fill->color);
        fill->has_color = LXW_TRUE;
    }

    if (fill->transparency > 100)
        fill->transparency = 0;

    return fill;
}

/*
 * Create a copy of a user supplied pattern.
 */
STATIC lxw_chart_pattern *
_chart_convert_pattern_args(lxw_chart_pattern *user_pattern)
{
    lxw_chart_pattern *pattern;

    if (!user_pattern)
        return NULL;

    if (!user_pattern->type) {
        LXW_WARN("chart_xxx_set_pattern: 'type' must be specified");
        return NULL;
    }

    if (!user_pattern->fg_color) {
        LXW_WARN("chart_xxx_set_pattern: 'fg_color' must be specified");
        return NULL;
    }

    pattern = calloc(1, sizeof(struct lxw_chart_pattern));
    RETURN_ON_MEM_ERROR(pattern, NULL);

    memcpy(pattern, user_pattern, sizeof(lxw_chart_pattern));

    pattern->fg_color = lxw_format_check_color(pattern->fg_color);
    pattern->has_fg_color = LXW_TRUE;

    if (pattern->bg_color) {
        pattern->bg_color = lxw_format_check_color(pattern->bg_color);
        pattern->has_bg_color = LXW_TRUE;
    }
    else {
        /* Default background color in Excel is white, when unspecified. */
        pattern->bg_color = LXW_COLOR_WHITE;
        pattern->has_bg_color = LXW_TRUE;
    }

    pattern->type = user_pattern->type;

    return pattern;
}

/*
 * Set a marker type for a series.
 */
STATIC void
_chart_set_default_marker_type(lxw_chart *self, uint8_t type)
{
    if (!self->default_marker) {
        lxw_chart_marker *marker = calloc(1, sizeof(struct lxw_chart_marker));
        RETURN_VOID_ON_MEM_ERROR(marker);
        self->default_marker = marker;
    }

    self->default_marker->type = type;
}

/*
 * Add unique ids for primary or secondary axes.
 */
STATIC void
_chart_add_axis_ids(lxw_chart *self)
{
    uint32_t chart_id = 50010000 + self->id;
    uint32_t axis_count = 1;

    self->axis_id_1 = chart_id + axis_count;
    self->axis_id_2 = self->axis_id_1 + 1;
}

/*
 * Utility function to set a chart range.
 */
STATIC void
_chart_set_range(lxw_series_range *range, const char *sheetname,
                 lxw_row_t first_row, lxw_col_t first_col,
                 lxw_row_t last_row, lxw_col_t last_col)
{
    char formula[LXW_MAX_FORMULA_RANGE_LENGTH] = { 0 };

    /* Set the range properties. */
    range->sheetname = lxw_strdup(sheetname);
    range->first_row = first_row;
    range->first_col = first_col;
    range->last_row = last_row;
    range->last_col = last_col;

    /* Free any existing range. */
    free(range->formula);

    /* Convert the range properties to a formula like: Sheet1!$A$1:$A$5. */
    lxw_rowcol_to_formula_abs(formula, sheetname,
                              first_row, first_col, last_row, last_col);

    range->formula = lxw_strdup(formula);
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
_chart_xml_declaration(lxw_chart *self)
{
    lxw_xml_declaration(self->file);
}

/*
 * Write the <c:chartSpace> element.
 */
STATIC void
_chart_write_chart_space(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char xmlns_c[] = LXW_SCHEMA_DRAWING "/chart";
    char xmlns_a[] = LXW_SCHEMA_DRAWING "/main";
    char xmlns_r[] = LXW_SCHEMA_OFFICEDOC "/relationships";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("xmlns:c", xmlns_c);
    LXW_PUSH_ATTRIBUTES_STR("xmlns:a", xmlns_a);
    LXW_PUSH_ATTRIBUTES_STR("xmlns:r", xmlns_r);

    lxw_xml_start_tag(self->file, "c:chartSpace", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:lang> element.
 */
STATIC void
_chart_write_lang(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", "en-US");

    lxw_xml_empty_tag(self->file, "c:lang", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:style> element.
 */
STATIC void
_chart_write_style(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    /* Don"t write an element for the default style, 2. */
    if (self->style_id == 2)
        return;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", self->style_id);

    lxw_xml_empty_tag(self->file, "c:style", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:layout> element.
 */
STATIC void
_chart_write_layout(lxw_chart *self)
{
    lxw_xml_empty_tag(self->file, "c:layout", NULL);
}

/*
 * Write the <c:grouping> element.
 */
STATIC void
_chart_write_grouping(lxw_chart *self, uint8_t grouping)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    if (grouping == LXW_GROUPING_STANDARD)
        LXW_PUSH_ATTRIBUTES_STR("val", "standard");
    else if (grouping == LXW_GROUPING_PERCENTSTACKED)
        LXW_PUSH_ATTRIBUTES_STR("val", "percentStacked");
    else if (grouping == LXW_GROUPING_STACKED)
        LXW_PUSH_ATTRIBUTES_STR("val", "stacked");
    else
        LXW_PUSH_ATTRIBUTES_STR("val", "clustered");

    lxw_xml_empty_tag(self->file, "c:grouping", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:radarStyle> element.
 */
STATIC void
_chart_write_radar_style(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    if (self->type == LXW_CHART_RADAR_FILLED)
        LXW_PUSH_ATTRIBUTES_STR("val", "filled");
    else
        LXW_PUSH_ATTRIBUTES_STR("val", "marker");

    lxw_xml_empty_tag(self->file, "c:radarStyle", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:varyColors> element.
 */
STATIC void
_chart_write_vary_colors(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", "1");

    lxw_xml_empty_tag(self->file, "c:varyColors", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:firstSliceAng> element.
 */
STATIC void
_chart_write_first_slice_ang(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", self->rotation);

    lxw_xml_empty_tag(self->file, "c:firstSliceAng", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:holeSize> element.
 */
STATIC void
_chart_write_hole_size(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", self->hole_size);

    lxw_xml_empty_tag(self->file, "c:holeSize", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <a:alpha> element.
 */
STATIC void
_chart_write_a_alpha(lxw_chart *self, uint8_t transparency)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    uint32_t val;

    LXW_INIT_ATTRIBUTES();

    val = (100 - transparency) * 1000;

    LXW_PUSH_ATTRIBUTES_INT("val", val);

    lxw_xml_empty_tag(self->file, "a:alpha", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <a:srgbClr> element.
 */
STATIC void
_chart_write_a_srgb_clr(lxw_chart *self, lxw_color_t color,
                        uint8_t transparency)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char rgb_str[LXW_ATTR_32];

    LXW_INIT_ATTRIBUTES();

    lxw_snprintf(rgb_str, LXW_ATTR_32, "%06X", color & LXW_COLOR_MASK);
    LXW_PUSH_ATTRIBUTES_STR("val", rgb_str);

    if (transparency) {
        lxw_xml_start_tag(self->file, "a:srgbClr", &attributes);

        /* Write the a:alpha element. */
        _chart_write_a_alpha(self, transparency);

        lxw_xml_end_tag(self->file, "a:srgbClr");
    }
    else {
        lxw_xml_empty_tag(self->file, "a:srgbClr", &attributes);
    }

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <a:solidFill> element.
 */
STATIC void
_chart_write_a_solid_fill(lxw_chart *self, lxw_color_t color,
                          uint8_t transparency)
{

    lxw_xml_start_tag(self->file, "a:solidFill", NULL);

    /* Write the a:srgbClr element. */
    _chart_write_a_srgb_clr(self, color, transparency);

    lxw_xml_end_tag(self->file, "a:solidFill");
}

/*
 * Write the <a:t> element.
 */
STATIC void
_chart_write_a_t(lxw_chart *self, char *name)
{
    lxw_xml_data_element(self->file, "a:t", name, NULL);
}

/*
 * Write the <a:endParaRPr> element.
 */
STATIC void
_chart_write_a_end_para_rpr(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("lang", "en-US");

    lxw_xml_empty_tag(self->file, "a:endParaRPr", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <a:defRPr> element.
 */
STATIC void
_chart_write_a_def_rpr(lxw_chart *self, lxw_chart_font *font)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    uint8_t has_color = LXW_FALSE;
    uint8_t has_latin = LXW_FALSE;
    uint8_t use_font_default = LXW_FALSE;

    LXW_INIT_ATTRIBUTES();

    if (font) {
        has_color = font->color || font->has_color;
        has_latin = font->name || font->pitch_family || font->charset;
        use_font_default = !(has_color || has_latin || font->baseline == -1);

        /* Set the font attributes. */
        if (font->size)
            LXW_PUSH_ATTRIBUTES_INT("sz", font->size);

        if (use_font_default || font->bold)
            LXW_PUSH_ATTRIBUTES_INT("b", font->bold & 0x1);

        if (use_font_default || font->italic)
            LXW_PUSH_ATTRIBUTES_INT("i", font->italic & 0x1);

        if (font->underline)
            LXW_PUSH_ATTRIBUTES_STR("u", "sng");

        if (font->baseline != -1)
            LXW_PUSH_ATTRIBUTES_INT("baseline", font->baseline);
    }

    /* There are sub-elements if the font name or color have changed. */
    if (has_latin || has_color) {

        lxw_xml_start_tag(self->file, "a:defRPr", &attributes);

        if (has_color) {
            _chart_write_a_solid_fill(self, font->color, LXW_FALSE);
        }

        if (has_latin) {
            /* Free and reuse the attribute list for the latin attributes. */
            LXW_FREE_ATTRIBUTES();

            if (font->name)
                LXW_PUSH_ATTRIBUTES_STR("typeface", font->name);

            if (font->pitch_family)
                LXW_PUSH_ATTRIBUTES_INT("pitchFamily", font->pitch_family);

            if (font->pitch_family || font->charset)
                LXW_PUSH_ATTRIBUTES_INT("charset", font->charset);

            /* Write the <a:latin> element. */
            lxw_xml_empty_tag(self->file, "a:latin", &attributes);
        }

        lxw_xml_end_tag(self->file, "a:defRPr");
    }
    else {
        lxw_xml_empty_tag(self->file, "a:defRPr", &attributes);
    }

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <a:rPr> element.
 */
STATIC void
_chart_write_a_r_pr(lxw_chart *self, lxw_chart_font *font)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    uint8_t has_color = LXW_FALSE;
    uint8_t has_latin = LXW_FALSE;
    uint8_t use_font_default = LXW_FALSE;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("lang", "en-US");

    if (font) {
        has_color = font->color || font->has_color;
        has_latin = font->name || font->pitch_family || font->charset;
        use_font_default = !(has_color || has_latin || font->baseline == -1);

        /* Set the font attributes. */
        if (font->size)
            LXW_PUSH_ATTRIBUTES_INT("sz", font->size);

        if (use_font_default || font->bold)
            LXW_PUSH_ATTRIBUTES_INT("b", font->bold & 0x1);

        if (use_font_default || font->italic)
            LXW_PUSH_ATTRIBUTES_INT("i", font->italic & 0x1);

        if (font->underline)
            LXW_PUSH_ATTRIBUTES_STR("u", "sng");

        if (font->baseline != -1)
            LXW_PUSH_ATTRIBUTES_INT("baseline", font->baseline);
    }

    /* There are sub-elements if the font name or color have changed. */
    if (has_latin || has_color) {

        lxw_xml_start_tag(self->file, "a:rPr", &attributes);

        if (has_color) {
            _chart_write_a_solid_fill(self, font->color, LXW_FALSE);
        }

        if (has_latin) {
            /* Free and reuse the attribute list for the latin attributes. */
            LXW_FREE_ATTRIBUTES();

            if (font->name)
                LXW_PUSH_ATTRIBUTES_STR("typeface", font->name);

            if (font->pitch_family)
                LXW_PUSH_ATTRIBUTES_INT("pitchFamily", font->pitch_family);

            if (font->pitch_family || font->charset)
                LXW_PUSH_ATTRIBUTES_INT("charset", font->charset);

            /* Write the <a:latin> element. */
            lxw_xml_empty_tag(self->file, "a:latin", &attributes);
        }

        lxw_xml_end_tag(self->file, "a:rPr");
    }
    else {
        lxw_xml_empty_tag(self->file, "a:rPr", &attributes);
    }

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <a:r> element.
 */
STATIC void
_chart_write_a_r(lxw_chart *self, char *name, lxw_chart_font *font)
{
    lxw_xml_start_tag(self->file, "a:r", NULL);

    /* Write the a:rPr element. */
    _chart_write_a_r_pr(self, font);

    /* Write the a:t element. */
    _chart_write_a_t(self, name);

    lxw_xml_end_tag(self->file, "a:r");
}

/*
 * Write the <a:pPr> element.
 */
STATIC void
_chart_write_a_p_pr_formula(lxw_chart *self, lxw_chart_font *font)
{
    lxw_xml_start_tag(self->file, "a:pPr", NULL);

    /* Write the a:defRPr element. */
    _chart_write_a_def_rpr(self, font);

    lxw_xml_end_tag(self->file, "a:pPr");
}

/*
 * Write the <a:pPr> element for pie chart legends.
 */
STATIC void
_chart_write_a_p_pr_pie(lxw_chart *self, lxw_chart_font *font)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("rtl", "0");

    lxw_xml_start_tag(self->file, "a:pPr", &attributes);

    /* Write the a:defRPr element. */
    _chart_write_a_def_rpr(self, font);

    lxw_xml_end_tag(self->file, "a:pPr");

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <a:pPr> element.
 */
STATIC void
_chart_write_a_p_pr_rich(lxw_chart *self, lxw_chart_font *font)
{
    lxw_xml_start_tag(self->file, "a:pPr", NULL);

    /* Write the a:defRPr element. */
    _chart_write_a_def_rpr(self, font);

    lxw_xml_end_tag(self->file, "a:pPr");
}

/*
 * Write the <a:p> element.
 */
STATIC void
_chart_write_a_p_formula(lxw_chart *self, lxw_chart_font *font)
{
    lxw_xml_start_tag(self->file, "a:p", NULL);

    /* Write the a:pPr element. */
    _chart_write_a_p_pr_formula(self, font);

    /* Write the a:endParaRPr element. */
    _chart_write_a_end_para_rpr(self);

    lxw_xml_end_tag(self->file, "a:p");
}

/*
 * Write the <a:p> element for pie chart legends.
 */
STATIC void
_chart_write_a_p_pie(lxw_chart *self, lxw_chart_font *font)
{
    lxw_xml_start_tag(self->file, "a:p", NULL);

    /* Write the a:pPr element. */
    _chart_write_a_p_pr_pie(self, font);

    /* Write the a:endParaRPr element. */
    _chart_write_a_end_para_rpr(self);

    lxw_xml_end_tag(self->file, "a:p");
}

/*
 * Write the <a:p> element.
 */
STATIC void
_chart_write_a_p_rich(lxw_chart *self, char *name, lxw_chart_font *font)
{
    lxw_xml_start_tag(self->file, "a:p", NULL);

    /* Write the a:pPr element. */
    _chart_write_a_p_pr_rich(self, font);

    /* Write the a:r element. */
    _chart_write_a_r(self, name, font);

    lxw_xml_end_tag(self->file, "a:p");
}

/*
 * Write the <a:lstStyle> element.
 */
STATIC void
_chart_write_a_lst_style(lxw_chart *self)
{
    lxw_xml_empty_tag(self->file, "a:lstStyle", NULL);
}

/*
 * Write the <a:bodyPr> element.
 */
STATIC void
_chart_write_a_body_pr(lxw_chart *self, int32_t rotation,
                       uint8_t is_horizontal)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    if (rotation == 0 && is_horizontal)
        rotation = -5400000;

    if (rotation)
        LXW_PUSH_ATTRIBUTES_INT("rot", rotation);

    if (is_horizontal)
        LXW_PUSH_ATTRIBUTES_STR("vert", "horz");

    lxw_xml_empty_tag(self->file, "a:bodyPr", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:ptCount> element.
 */
STATIC void
_chart_write_pt_count(lxw_chart *self, uint16_t num_data_points)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", num_data_points);

    lxw_xml_empty_tag(self->file, "c:ptCount", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:v> element.
 */
STATIC void
_chart_write_v_num(lxw_chart *self, double number)
{
    char data[LXW_ATTR_32];

    lxw_snprintf(data, LXW_ATTR_32, "%.16g", number);

    lxw_xml_data_element(self->file, "c:v", data, NULL);
}

/*
 * Write the <c:v> element.
 */
STATIC void
_chart_write_v_str(lxw_chart *self, char *str)
{
    lxw_xml_data_element(self->file, "c:v", str, NULL);
}

/*
 * Write the <c:f> element.
 */
STATIC void
_chart_write_f(lxw_chart *self, char *formula)
{
    lxw_xml_data_element(self->file, "c:f", formula, NULL);
}

/*
 * Write the <c:pt> element.
 */
STATIC void
_chart_write_pt(lxw_chart *self, uint16_t index,
                lxw_series_data_point *data_point)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    /* Ignore chart points that have no data. */
    if (data_point->no_data)
        return;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("idx", index);

    lxw_xml_start_tag(self->file, "c:pt", &attributes);

    if (data_point->is_string && data_point->string)
        _chart_write_v_str(self, data_point->string);
    else
        _chart_write_v_num(self, data_point->number);

    lxw_xml_end_tag(self->file, "c:pt");

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:pt> element.
 */
STATIC void
_chart_write_num_pt(lxw_chart *self, uint16_t index,
                    lxw_series_data_point *data_point)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    /* Ignore chart points that have no data. */
    if (data_point->no_data)
        return;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("idx", index);

    lxw_xml_start_tag(self->file, "c:pt", &attributes);

    _chart_write_v_num(self, data_point->number);

    lxw_xml_end_tag(self->file, "c:pt");

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:formatCode> element.
 */
STATIC void
_chart_write_format_code(lxw_chart *self)
{
    lxw_xml_data_element(self->file, "c:formatCode", "General", NULL);
}

/*
 * Write the <c:numCache> element.
 */
STATIC void
_chart_write_num_cache(lxw_chart *self, lxw_series_range *range)
{
    lxw_series_data_point *data_point;
    uint16_t index = 0;

    lxw_xml_start_tag(self->file, "c:numCache", NULL);

    /* Write the c:formatCode element. */
    _chart_write_format_code(self);

    /* Write the c:ptCount element. */
    _chart_write_pt_count(self, range->num_data_points);

    STAILQ_FOREACH(data_point, range->data_cache, list_pointers) {
        /* Write the c:pt element. */
        _chart_write_num_pt(self, index, data_point);
        index++;
    }

    lxw_xml_end_tag(self->file, "c:numCache");
}

/*
 * Write the <c:strCache> element.
 */
STATIC void
_chart_write_str_cache(lxw_chart *self, lxw_series_range *range)
{
    lxw_series_data_point *data_point;
    uint16_t index = 0;

    lxw_xml_start_tag(self->file, "c:strCache", NULL);

    /* Write the c:ptCount element. */
    _chart_write_pt_count(self, range->num_data_points);

    STAILQ_FOREACH(data_point, range->data_cache, list_pointers) {
        /* Write the c:pt element. */
        _chart_write_pt(self, index, data_point);
        index++;
    }

    lxw_xml_end_tag(self->file, "c:strCache");
}

/*
 * Write the <c:numRef> element.
 */
STATIC void
_chart_write_num_ref(lxw_chart *self, lxw_series_range *range)
{
    lxw_xml_start_tag(self->file, "c:numRef", NULL);

    /* Write the c:f element. */
    _chart_write_f(self, range->formula);

    if (!STAILQ_EMPTY(range->data_cache)) {
        /* Write the c:numCache element. */
        _chart_write_num_cache(self, range);
    }

    lxw_xml_end_tag(self->file, "c:numRef");
}

/*
 * Write the <c:strRef> element.
 */
STATIC void
_chart_write_str_ref(lxw_chart *self, lxw_series_range *range)
{
    lxw_xml_start_tag(self->file, "c:strRef", NULL);

    /* Write the c:f element. */
    _chart_write_f(self, range->formula);

    if (!STAILQ_EMPTY(range->data_cache)) {
        /* Write the c:strCache element. */
        _chart_write_str_cache(self, range);
    }

    lxw_xml_end_tag(self->file, "c:strRef");
}

/*
 * Write the cached data elements.
 */
STATIC void
_chart_write_data_cache(lxw_chart *self, lxw_series_range *range,
                        uint8_t has_string_cache)
{
    if (has_string_cache) {
        /* Write the c:strRef element. */
        _chart_write_str_ref(self, range);
    }
    else {
        /* Write the c:numRef element. */
        _chart_write_num_ref(self, range);
    }
}

/*
 * Write the <c:tx> element with a simple value such as for series names.
 */
STATIC void
_chart_write_tx_value(lxw_chart *self, char *name)
{
    lxw_xml_start_tag(self->file, "c:tx", NULL);

    /* Write the c:v element. */
    _chart_write_v_str(self, name);

    lxw_xml_end_tag(self->file, "c:tx");
}

/*
 * Write the <c:tx> element with a simple value such as for series names.
 */
STATIC void
_chart_write_tx_formula(lxw_chart *self, lxw_chart_title *title)
{
    lxw_xml_start_tag(self->file, "c:tx", NULL);

    _chart_write_str_ref(self, title->range);

    lxw_xml_end_tag(self->file, "c:tx");
}

/*
 * Write the <c:txPr> element.
 */
STATIC void
_chart_write_tx_pr(lxw_chart *self, uint8_t is_horizontal,
                   lxw_chart_font *font)
{
    int32_t rotation = 0;

    if (font)
        rotation = font->rotation;

    lxw_xml_start_tag(self->file, "c:txPr", NULL);

    /* Write the a:bodyPr element. */
    _chart_write_a_body_pr(self, rotation, is_horizontal);

    /* Write the a:lstStyle element. */
    _chart_write_a_lst_style(self);

    /* Write the a:p element. */
    _chart_write_a_p_formula(self, font);

    lxw_xml_end_tag(self->file, "c:txPr");
}

/*
 * Write the <c:txPr> element for pie chart legends.
 */
STATIC void
_chart_write_tx_pr_pie(lxw_chart *self, uint8_t is_horizontal,
                       lxw_chart_font *font)
{
    int32_t rotation = 0;

    if (font)
        rotation = font->rotation;

    lxw_xml_start_tag(self->file, "c:txPr", NULL);

    /* Write the a:bodyPr element. */
    _chart_write_a_body_pr(self, rotation, is_horizontal);

    /* Write the a:lstStyle element. */
    _chart_write_a_lst_style(self);

    /* Write the a:p element. */
    _chart_write_a_p_pie(self, font);

    lxw_xml_end_tag(self->file, "c:txPr");
}

/*
 * Write the <c:txPr> element.
 */
STATIC void
_chart_write_axis_font(lxw_chart *self, lxw_chart_font *font)
{
    if (!font)
        return;

    lxw_xml_start_tag(self->file, "c:txPr", NULL);

    /* Write the a:bodyPr element. */
    _chart_write_a_body_pr(self, font->rotation, LXW_FALSE);

    /* Write the a:lstStyle element. */
    _chart_write_a_lst_style(self);

    lxw_xml_start_tag(self->file, "a:p", NULL);

    /* Write the a:pPr element. */
    _chart_write_a_p_pr_rich(self, font);

    /* Write the a:endParaRPr element. */
    _chart_write_a_end_para_rpr(self);

    lxw_xml_end_tag(self->file, "a:p");
    lxw_xml_end_tag(self->file, "c:txPr");
}

/*
 * Write the <c:rich> element.
 */
STATIC void
_chart_write_rich(lxw_chart *self, char *name, uint8_t is_horizontal,
                  lxw_chart_font *font)
{
    int32_t rotation = 0;

    if (font)
        rotation = font->rotation;

    lxw_xml_start_tag(self->file, "c:rich", NULL);

    /* Write the a:bodyPr element. */
    _chart_write_a_body_pr(self, rotation, is_horizontal);

    /* Write the a:lstStyle element. */
    _chart_write_a_lst_style(self);

    /* Write the a:p element. */
    _chart_write_a_p_rich(self, name, font);

    lxw_xml_end_tag(self->file, "c:rich");
}

/*
 * Write the <c:tx> element.
 */
STATIC void
_chart_write_tx_rich(lxw_chart *self, char *name, uint8_t is_horizontal,
                     lxw_chart_font *font)
{

    lxw_xml_start_tag(self->file, "c:tx", NULL);

    /* Write the c:rich element. */
    _chart_write_rich(self, name, is_horizontal, font);

    lxw_xml_end_tag(self->file, "c:tx");
}

/*
 * Write the <c:title> element for rich strings.
 */
STATIC void
_chart_write_title_rich(lxw_chart *self, lxw_chart_title *title)
{
    lxw_xml_start_tag(self->file, "c:title", NULL);

    /* Write the c:tx element. */
    _chart_write_tx_rich(self, title->name, title->is_horizontal,
                         title->font);

    /* Write the c:layout element. */
    _chart_write_layout(self);

    lxw_xml_end_tag(self->file, "c:title");
}

/*
 * Write the <c:title> element for a formula style title
 */
STATIC void
_chart_write_title_formula(lxw_chart *self, lxw_chart_title *title)
{
    lxw_xml_start_tag(self->file, "c:title", NULL);

    /* Write the c:tx element. */
    _chart_write_tx_formula(self, title);

    /* Write the c:layout element. */
    _chart_write_layout(self);

    /* Write the c:txPr element. */
    _chart_write_tx_pr(self, title->is_horizontal, title->font);

    lxw_xml_end_tag(self->file, "c:title");
}

/*
 * Write the <c:delete> element.
 */
STATIC void
_chart_write_delete(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", "1");

    lxw_xml_empty_tag(self->file, "c:delete", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:autoTitleDeleted> element.
 */
STATIC void
_chart_write_auto_title_deleted(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", "1");

    lxw_xml_empty_tag(self->file, "c:autoTitleDeleted", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:idx> element.
 */
STATIC void
_chart_write_idx(lxw_chart *self, uint16_t index)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", index);

    lxw_xml_empty_tag(self->file, "c:idx", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <a:prstDash> element.
 */
STATIC void
_chart_write_a_prst_dash(lxw_chart *self, uint8_t dash_type)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    if (dash_type == LXW_CHART_LINE_DASH_ROUND_DOT)
        LXW_PUSH_ATTRIBUTES_STR("val", "sysDot");
    else if (dash_type == LXW_CHART_LINE_DASH_SQUARE_DOT)
        LXW_PUSH_ATTRIBUTES_STR("val", "sysDash");
    else if (dash_type == LXW_CHART_LINE_DASH_DASH_DOT)
        LXW_PUSH_ATTRIBUTES_STR("val", "dashDot");
    else if (dash_type == LXW_CHART_LINE_DASH_LONG_DASH)
        LXW_PUSH_ATTRIBUTES_STR("val", "lgDash");
    else if (dash_type == LXW_CHART_LINE_DASH_LONG_DASH_DOT)
        LXW_PUSH_ATTRIBUTES_STR("val", "lgDashDot");
    else if (dash_type == LXW_CHART_LINE_DASH_LONG_DASH_DOT_DOT)
        LXW_PUSH_ATTRIBUTES_STR("val", "lgDashDotDot");
    else if (dash_type == LXW_CHART_LINE_DASH_DOT)
        LXW_PUSH_ATTRIBUTES_STR("val", "dot");
    else if (dash_type == LXW_CHART_LINE_DASH_SYSTEM_DASH_DOT)
        LXW_PUSH_ATTRIBUTES_STR("val", "sysDashDot");
    else if (dash_type == LXW_CHART_LINE_DASH_SYSTEM_DASH_DOT_DOT)
        LXW_PUSH_ATTRIBUTES_STR("val", "sysDashDotDot");
    else
        LXW_PUSH_ATTRIBUTES_STR("val", "dash");

    lxw_xml_empty_tag(self->file, "a:prstDash", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <a:noFill> element.
 */
STATIC void
_chart_write_a_no_fill(lxw_chart *self)
{
    lxw_xml_empty_tag(self->file, "a:noFill", NULL);
}

/*
 * Write the <a:ln> element.
 */
STATIC void
_chart_write_a_ln(lxw_chart *self, lxw_chart_line *line)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    float width_flt;
    uint32_t width_int;

    LXW_INIT_ATTRIBUTES();

    /* Round width to nearest 0.25, like Excel. */
    width_flt = (float) (uint32_t) ((line->width + 0.125) * 4.0) / 4.0;

    /* Convert to internal units. */
    width_int = (uint32_t) (0.5 + (12700.0 * width_flt));

    if (width_int)
        LXW_PUSH_ATTRIBUTES_INT("w", width_int);

    lxw_xml_start_tag(self->file, "a:ln", &attributes);

    /* Write the line fill. */
    if (line->none) {
        /* Write the a:noFill element. */
        _chart_write_a_no_fill(self);
    }
    else if (line->has_color) {
        /* Write the a:solidFill element. */
        _chart_write_a_solid_fill(self, line->color, line->transparency);
    }

    /* Write the line/dash type. */
    if (line->dash_type) {
        /* Write the a:prstDash element. */
        _chart_write_a_prst_dash(self, line->dash_type);
    }

    lxw_xml_end_tag(self->file, "a:ln");

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <a:fgClr> element.
 */
STATIC void
_chart_write_a_fg_clr(lxw_chart *self, lxw_color_t color)
{
    lxw_xml_start_tag(self->file, "a:fgClr", NULL);

    _chart_write_a_srgb_clr(self, color, LXW_FALSE);

    lxw_xml_end_tag(self->file, "a:fgClr");
}

/*
 * Write the <a:bgClr> element.
 */
STATIC void
_chart_write_a_bg_clr(lxw_chart *self, lxw_color_t color)
{
    lxw_xml_start_tag(self->file, "a:bgClr", NULL);

    _chart_write_a_srgb_clr(self, color, LXW_FALSE);

    lxw_xml_end_tag(self->file, "a:bgClr");
}

/*
 * Write the <a:pattFill> element.
 */
STATIC void
_chart_write_a_patt_fill(lxw_chart *self, lxw_chart_pattern *pattern)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    if (pattern->type == LXW_CHART_PATTERN_NONE)
        LXW_PUSH_ATTRIBUTES_STR("prst", "none");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_5)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct5");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_10)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct10");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_20)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct20");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_25)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct25");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_30)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct30");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_40)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct40");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_50)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct50");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_60)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct60");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_70)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct70");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_75)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct75");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_80)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct80");
    else if (pattern->type == LXW_CHART_PATTERN_PERCENT_90)
        LXW_PUSH_ATTRIBUTES_STR("prst", "pct90");
    else if (pattern->type == LXW_CHART_PATTERN_LIGHT_DOWNWARD_DIAGONAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "ltDnDiag");
    else if (pattern->type == LXW_CHART_PATTERN_LIGHT_UPWARD_DIAGONAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "ltUpDiag");
    else if (pattern->type == LXW_CHART_PATTERN_DARK_DOWNWARD_DIAGONAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "dkDnDiag");
    else if (pattern->type == LXW_CHART_PATTERN_DARK_UPWARD_DIAGONAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "dkUpDiag");
    else if (pattern->type == LXW_CHART_PATTERN_WIDE_DOWNWARD_DIAGONAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "wdDnDiag");
    else if (pattern->type == LXW_CHART_PATTERN_WIDE_UPWARD_DIAGONAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "wdUpDiag");
    else if (pattern->type == LXW_CHART_PATTERN_LIGHT_VERTICAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "ltVert");
    else if (pattern->type == LXW_CHART_PATTERN_LIGHT_HORIZONTAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "ltHorz");
    else if (pattern->type == LXW_CHART_PATTERN_NARROW_VERTICAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "narVert");
    else if (pattern->type == LXW_CHART_PATTERN_NARROW_HORIZONTAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "narHorz");
    else if (pattern->type == LXW_CHART_PATTERN_DARK_VERTICAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "dkVert");
    else if (pattern->type == LXW_CHART_PATTERN_DARK_HORIZONTAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "dkHorz");
    else if (pattern->type == LXW_CHART_PATTERN_DASHED_DOWNWARD_DIAGONAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "dashDnDiag");
    else if (pattern->type == LXW_CHART_PATTERN_DASHED_UPWARD_DIAGONAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "dashUpDiag");
    else if (pattern->type == LXW_CHART_PATTERN_DASHED_HORIZONTAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "dashHorz");
    else if (pattern->type == LXW_CHART_PATTERN_DASHED_VERTICAL)
        LXW_PUSH_ATTRIBUTES_STR("prst", "dashVert");
    else if (pattern->type == LXW_CHART_PATTERN_SMALL_CONFETTI)
        LXW_PUSH_ATTRIBUTES_STR("prst", "smConfetti");
    else if (pattern->type == LXW_CHART_PATTERN_LARGE_CONFETTI)
        LXW_PUSH_ATTRIBUTES_STR("prst", "lgConfetti");
    else if (pattern->type == LXW_CHART_PATTERN_ZIGZAG)
        LXW_PUSH_ATTRIBUTES_STR("prst", "zigZag");
    else if (pattern->type == LXW_CHART_PATTERN_WAVE)
        LXW_PUSH_ATTRIBUTES_STR("prst", "wave");
    else if (pattern->type == LXW_CHART_PATTERN_DIAGONAL_BRICK)
        LXW_PUSH_ATTRIBUTES_STR("prst", "diagBrick");
    else if (pattern->type == LXW_CHART_PATTERN_HORIZONTAL_BRICK)
        LXW_PUSH_ATTRIBUTES_STR("prst", "horzBrick");
    else if (pattern->type == LXW_CHART_PATTERN_WEAVE)
        LXW_PUSH_ATTRIBUTES_STR("prst", "weave");
    else if (pattern->type == LXW_CHART_PATTERN_PLAID)
        LXW_PUSH_ATTRIBUTES_STR("prst", "plaid");
    else if (pattern->type == LXW_CHART_PATTERN_DIVOT)
        LXW_PUSH_ATTRIBUTES_STR("prst", "divot");
    else if (pattern->type == LXW_CHART_PATTERN_DOTTED_GRID)
        LXW_PUSH_ATTRIBUTES_STR("prst", "dotGrid");
    else if (pattern->type == LXW_CHART_PATTERN_DOTTED_DIAMOND)
        LXW_PUSH_ATTRIBUTES_STR("prst", "dotDmnd");
    else if (pattern->type == LXW_CHART_PATTERN_SHINGLE)
        LXW_PUSH_ATTRIBUTES_STR("prst", "shingle");
    else if (pattern->type == LXW_CHART_PATTERN_TRELLIS)
        LXW_PUSH_ATTRIBUTES_STR("prst", "trellis");
    else if (pattern->type == LXW_CHART_PATTERN_SPHERE)
        LXW_PUSH_ATTRIBUTES_STR("prst", "sphere");
    else if (pattern->type == LXW_CHART_PATTERN_SMALL_GRID)
        LXW_PUSH_ATTRIBUTES_STR("prst", "smGrid");
    else if (pattern->type == LXW_CHART_PATTERN_LARGE_GRID)
        LXW_PUSH_ATTRIBUTES_STR("prst", "lgGrid");
    else if (pattern->type == LXW_CHART_PATTERN_SMALL_CHECK)
        LXW_PUSH_ATTRIBUTES_STR("prst", "smCheck");
    else if (pattern->type == LXW_CHART_PATTERN_LARGE_CHECK)
        LXW_PUSH_ATTRIBUTES_STR("prst", "lgCheck");
    else if (pattern->type == LXW_CHART_PATTERN_OUTLINED_DIAMOND)
        LXW_PUSH_ATTRIBUTES_STR("prst", "openDmnd");
    else if (pattern->type == LXW_CHART_PATTERN_SOLID_DIAMOND)
        LXW_PUSH_ATTRIBUTES_STR("prst", "solidDmnd");
    else
        LXW_PUSH_ATTRIBUTES_STR("prst", "percent_50");

    lxw_xml_start_tag(self->file, "a:pattFill", &attributes);

    if (pattern->has_fg_color)
        _chart_write_a_fg_clr(self, pattern->fg_color);

    if (pattern->has_bg_color)
        _chart_write_a_bg_clr(self, pattern->bg_color);

    lxw_xml_end_tag(self->file, "a:pattFill");

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:spPr> element.
 */
STATIC void
_chart_write_sp_pr(lxw_chart *self, lxw_chart_line *line,
                   lxw_chart_fill *fill, lxw_chart_pattern *pattern)
{
    if (!line && !fill && !pattern)
        return;

    lxw_xml_start_tag(self->file, "c:spPr", NULL);

    /* Write the series fill. Note: a pattern fill overrides a solid fill. */
    if (fill && !pattern) {
        if (fill->none) {
            /* Write the a:noFill element. */
            _chart_write_a_no_fill(self);
        }
        else {
            /* Write the a:solidFill element. */
            _chart_write_a_solid_fill(self, fill->color, fill->transparency);
        }
    }

    if (pattern) {
        /* Write the a:pattFill element. */
        _chart_write_a_patt_fill(self, pattern);
    }

    if (line) {
        /* Write the a:ln element. */
        _chart_write_a_ln(self, line);
    }

    lxw_xml_end_tag(self->file, "c:spPr");
}

/*
 * Write the <c:order> element.
 */
STATIC void
_chart_write_order(lxw_chart *self, uint16_t index)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", index);

    lxw_xml_empty_tag(self->file, "c:order", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:axId> element.
 */
STATIC void
_chart_write_axis_id(lxw_chart *self, uint32_t axis_id)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", axis_id);

    lxw_xml_empty_tag(self->file, "c:axId", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:axId> element.
 */
STATIC void
_chart_write_axis_ids(lxw_chart *self)
{
    if (!self->axis_id_1)
        _chart_add_axis_ids(self);

    _chart_write_axis_id(self, self->axis_id_1);
    _chart_write_axis_id(self, self->axis_id_2);
}

/*
 * Write the series name.
 */
STATIC void
_chart_write_series_name(lxw_chart *self, lxw_chart_series *series)
{
    if (series->title.name) {
        /* Write the c:tx element. */
        _chart_write_tx_value(self, series->title.name);
    }
    else if (series->title.range->formula) {
        /* Write the c:tx element. */
        _chart_write_tx_formula(self, &series->title);

    }
}

/*
 * Write the <c:majorTickMark> element.
 */
STATIC void
_chart_write_major_tick_mark(lxw_chart *self, lxw_chart_axis *axis)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    if (!axis->major_tick_mark)
        return;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", "cross");

    lxw_xml_empty_tag(self->file, "c:majorTickMark", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:symbol> element.
 */
STATIC void
_chart_write_symbol(lxw_chart *self, uint8_t type)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    if (type == LXW_CHART_MARKER_SQUARE)
        LXW_PUSH_ATTRIBUTES_STR("val", "square");
    else if (type == LXW_CHART_MARKER_DIAMOND)
        LXW_PUSH_ATTRIBUTES_STR("val", "diamond");
    else if (type == LXW_CHART_MARKER_TRIANGLE)
        LXW_PUSH_ATTRIBUTES_STR("val", "triangle");
    else if (type == LXW_CHART_MARKER_X)
        LXW_PUSH_ATTRIBUTES_STR("val", "x");
    else if (type == LXW_CHART_MARKER_STAR)
        LXW_PUSH_ATTRIBUTES_STR("val", "star");
    else if (type == LXW_CHART_MARKER_SHORT_DASH)
        LXW_PUSH_ATTRIBUTES_STR("val", "short_dash");
    else if (type == LXW_CHART_MARKER_LONG_DASH)
        LXW_PUSH_ATTRIBUTES_STR("val", "long_dash");
    else if (type == LXW_CHART_MARKER_CIRCLE)
        LXW_PUSH_ATTRIBUTES_STR("val", "circle");
    else if (type == LXW_CHART_MARKER_PLUS)
        LXW_PUSH_ATTRIBUTES_STR("val", "plus");
    else
        LXW_PUSH_ATTRIBUTES_STR("val", "none");

    lxw_xml_empty_tag(self->file, "c:symbol", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:size> element.
 */
STATIC void
_chart_write_marker_size(lxw_chart *self, uint8_t size)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", size);

    lxw_xml_empty_tag(self->file, "c:size", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:marker> element.
 */
STATIC void
_chart_write_marker(lxw_chart *self, lxw_chart_marker *marker)
{
    /* If there isn't a user defined marker use the default, if this chart
     * type one. The default usually turns the marker off. */
    if (!marker)
        marker = self->default_marker;

    if (!marker)
        return;

    if (marker->type == LXW_CHART_MARKER_AUTOMATIC)
        return;

    lxw_xml_start_tag(self->file, "c:marker", NULL);

    /* Write the c:symbol element. */
    _chart_write_symbol(self, marker->type);

    /* Write the c:size element. */
    if (marker->size)
        _chart_write_marker_size(self, marker->size);

    /* Write the c:spPr element. */
    _chart_write_sp_pr(self, marker->line, marker->fill, marker->pattern);

    lxw_xml_end_tag(self->file, "c:marker");
}

/*
 * Write the <c:marker> element.
 */
STATIC void
_chart_write_marker_value(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char val[] = "1";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", val);

    lxw_xml_empty_tag(self->file, "c:marker", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:smooth> element.
 */
STATIC void
_chart_write_smooth(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char val[] = "1";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", val);

    lxw_xml_empty_tag(self->file, "c:smooth", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:scatterStyle> element.
 */
STATIC void
_chart_write_scatter_style(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    if (self->type == LXW_CHART_SCATTER_SMOOTH
        || self->type == LXW_CHART_SCATTER_SMOOTH_WITH_MARKERS)
        LXW_PUSH_ATTRIBUTES_STR("val", "smoothMarker");
    else
        LXW_PUSH_ATTRIBUTES_STR("val", "lineMarker");

    lxw_xml_empty_tag(self->file, "c:scatterStyle", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:cat> element.
 */
STATIC void
_chart_write_cat(lxw_chart *self, lxw_chart_series *series)
{
    uint8_t has_string_cache = series->categories->has_string_cache;

    /* Ignore <c:cat> elements for charts without category values. */
    if (!series->categories->formula)
        return;

    self->cat_has_num_fmt = !has_string_cache;

    lxw_xml_start_tag(self->file, "c:cat", NULL);

    /* Write the c:numRef element. */
    _chart_write_data_cache(self, series->categories, has_string_cache);

    lxw_xml_end_tag(self->file, "c:cat");
}

/*
 * Write the <c:xVal> element.
 */
STATIC void
_chart_write_x_val(lxw_chart *self, lxw_chart_series *series)
{
    uint8_t has_string_cache = series->categories->has_string_cache;

    lxw_xml_start_tag(self->file, "c:xVal", NULL);

    /* Write the data cache elements. */
    _chart_write_data_cache(self, series->categories, has_string_cache);

    lxw_xml_end_tag(self->file, "c:xVal");
}

/*
 * Write the <c:val> element.
 */
STATIC void
_chart_write_val(lxw_chart *self, lxw_chart_series *series)
{
    lxw_xml_start_tag(self->file, "c:val", NULL);

    /* Write the data cache elements. The string_cache is set to false since
     * this should always be a number series. */
    _chart_write_data_cache(self, series->values, LXW_FALSE);

    lxw_xml_end_tag(self->file, "c:val");
}

/*
 * Write the <c:yVal> element.
 */
STATIC void
_chart_write_y_val(lxw_chart *self, lxw_chart_series *series)
{
    lxw_xml_start_tag(self->file, "c:yVal", NULL);

    /* Write the data cache elements. The string_cache is set to false since
     * this should always be a number series. */
    _chart_write_data_cache(self, series->values, LXW_FALSE);

    lxw_xml_end_tag(self->file, "c:yVal");
}

/*
 * Write the <c:ser> element.
 */
STATIC void
_chart_write_ser(lxw_chart *self, lxw_chart_series *series)
{
    uint16_t index = self->series_index++;

    lxw_xml_start_tag(self->file, "c:ser", NULL);

    /* Write the c:idx element. */
    _chart_write_idx(self, index);

    /* Write the c:order element. */
    _chart_write_order(self, index);

    /* Write the series name. */
    _chart_write_series_name(self, series);

    /* Write the c:spPr element. */
    _chart_write_sp_pr(self, series->line, series->fill, series->pattern);

    /* Write the c:marker element. */
    _chart_write_marker(self, series->marker);

    /* Write the c:cat element. */
    _chart_write_cat(self, series);

    /* Write the c:val element. */
    _chart_write_val(self, series);

    lxw_xml_end_tag(self->file, "c:ser");
}

/*
 * Write the <c:ser> element but with c:xVal/c:yVal instead of c:cat/c:val
 * elements.
 */
STATIC void
_chart_write_xval_ser(lxw_chart *self, lxw_chart_series *series)
{
    uint16_t index = self->series_index++;

    lxw_xml_start_tag(self->file, "c:ser", NULL);

    /* Write the c:idx element. */
    _chart_write_idx(self, index);

    /* Write the c:order element. */
    _chart_write_order(self, index);

    /* Write the series name. */
    _chart_write_series_name(self, series);

    /* Write the c:spPr element. */
    _chart_write_sp_pr(self, series->line, series->fill, series->pattern);

    /* Write the c:marker element. */
    _chart_write_marker(self, series->marker);

    /* Write the c:xVal element. */
    _chart_write_x_val(self, series);

    /* Write the yVal element. */
    _chart_write_y_val(self, series);

    if (self->type == LXW_CHART_SCATTER_SMOOTH
        || self->type == LXW_CHART_SCATTER_SMOOTH_WITH_MARKERS) {
        /* Write the c:smooth element. */
        _chart_write_smooth(self);
    }

    lxw_xml_end_tag(self->file, "c:ser");
}

/*
 * Write the <c:orientation> element.
 */
STATIC void
_chart_write_orientation(lxw_chart *self, uint8_t reverse)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    if (reverse)
        LXW_PUSH_ATTRIBUTES_STR("val", "maxMin");
    else
        LXW_PUSH_ATTRIBUTES_STR("val", "minMax");

    lxw_xml_empty_tag(self->file, "c:orientation", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:max> element.
 */
STATIC void
_chart_write_max(lxw_chart *self, double max)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_DBL("val", max);

    lxw_xml_empty_tag(self->file, "c:max", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:min> element.
 */
STATIC void
_chart_write_min(lxw_chart *self, double min)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_DBL("val", min);

    lxw_xml_empty_tag(self->file, "c:min", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:logBase> element.
 */
STATIC void
_chart_write_log_base(lxw_chart *self, uint16_t log_base)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    if (!log_base)
        return;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", log_base);

    lxw_xml_empty_tag(self->file, "c:logBase", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:scaling> element.
 */
STATIC void
_chart_write_scaling(lxw_chart *self, uint8_t reverse,
                     uint8_t has_min, double min,
                     uint8_t has_max, double max, uint16_t log_base)
{
    lxw_xml_start_tag(self->file, "c:scaling", NULL);

    /* Write the c:logBase element. */
    _chart_write_log_base(self, log_base);

    /* Write the c:orientation element. */
    _chart_write_orientation(self, reverse);

    if (has_max) {
        /* Write the c:max element. */
        _chart_write_max(self, max);
    }

    if (has_min) {
        /* Write the c:min element. */
        _chart_write_min(self, min);
    }

    lxw_xml_end_tag(self->file, "c:scaling");
}

/*
 * Write the <c:axPos> element.
 */
STATIC void
_chart_write_axis_pos(lxw_chart *self, uint8_t position, uint8_t reverse)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    /* Reverse the axis direction if required. */
    position ^= reverse;

    if (position == LXW_CHART_AXIS_RIGHT)
        LXW_PUSH_ATTRIBUTES_STR("val", "r");
    else if (position == LXW_CHART_AXIS_LEFT)
        LXW_PUSH_ATTRIBUTES_STR("val", "l");
    else if (position == LXW_CHART_AXIS_TOP)
        LXW_PUSH_ATTRIBUTES_STR("val", "t");
    else if (position == LXW_CHART_AXIS_BOTTOM)
        LXW_PUSH_ATTRIBUTES_STR("val", "b");

    lxw_xml_empty_tag(self->file, "c:axPos", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:tickLblPos> element.
 */
STATIC void
_chart_write_tick_lbl_pos(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char val[] = "nextTo";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", val);

    lxw_xml_empty_tag(self->file, "c:tickLblPos", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:crossAx> element.
 */
STATIC void
_chart_write_cross_axis(lxw_chart *self, uint32_t axis_id)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", axis_id);

    lxw_xml_empty_tag(self->file, "c:crossAx", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:crosses> element.
 */
STATIC void
_chart_write_crosses(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char val[] = "autoZero";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", val);

    lxw_xml_empty_tag(self->file, "c:crosses", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:auto> element.
 */
STATIC void
_chart_write_auto(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char val[] = "1";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", val);

    lxw_xml_empty_tag(self->file, "c:auto", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:lblAlgn> element.
 */
STATIC void
_chart_write_lbl_algn(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char val[] = "ctr";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", val);

    lxw_xml_empty_tag(self->file, "c:lblAlgn", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:lblOffset> element.
 */
STATIC void
_chart_write_lbl_offset(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char val[] = "100";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", val);

    lxw_xml_empty_tag(self->file, "c:lblOffset", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:majorGridlines> element.
 */
STATIC void
_chart_write_major_gridlines(lxw_chart *self, lxw_chart_axis *axis)
{
    if (!axis->major_gridlines.visible)
        return;

    if (axis->major_gridlines.line) {
        lxw_xml_start_tag(self->file, "c:majorGridlines", NULL);

        /* Write the c:spPr element for the axis line. */
        _chart_write_sp_pr(self, axis->major_gridlines.line, NULL, NULL);

        lxw_xml_end_tag(self->file, "c:majorGridlines");
    }
    else {
        lxw_xml_empty_tag(self->file, "c:majorGridlines", NULL);
    }
}

/*
 * Write the <c:minorGridlines> element.
 */
STATIC void
_chart_write_minor_gridlines(lxw_chart *self, lxw_chart_axis *axis)
{
    if (!axis->minor_gridlines.visible)
        return;

    if (axis->minor_gridlines.line) {
        lxw_xml_start_tag(self->file, "c:minorGridlines", NULL);

        /* Write the c:spPr element for the axis line. */
        _chart_write_sp_pr(self, axis->minor_gridlines.line, NULL, NULL);

        lxw_xml_end_tag(self->file, "c:minorGridlines");

    }
    else {
        lxw_xml_empty_tag(self->file, "c:minorGridlines", NULL);
    }
}

/*
 * Write the <c:numFmt> element.
 */
STATIC void
_chart_write_number_format(lxw_chart *self, lxw_chart_axis *axis)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char source_linked[] = "1";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("formatCode", axis->default_num_format);
    LXW_PUSH_ATTRIBUTES_STR("sourceLinked", source_linked);

    lxw_xml_empty_tag(self->file, "c:numFmt", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:crossBetween> element.
 */
STATIC void
_chart_write_cross_between(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    if (self->cross_between)
        LXW_PUSH_ATTRIBUTES_STR("val", "midCat");
    else
        LXW_PUSH_ATTRIBUTES_STR("val", "between");

    lxw_xml_empty_tag(self->file, "c:crossBetween", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:overlay> element.
 */
STATIC void
_chart_write_overlay(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", "1");

    lxw_xml_empty_tag(self->file, "c:overlay", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:legendPos> element.
 */
STATIC void
_chart_write_legend_pos(lxw_chart *self, char *position)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();

    LXW_PUSH_ATTRIBUTES_STR("val", position);

    lxw_xml_empty_tag(self->file, "c:legendPos", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:legendEntry> element.
 */
STATIC void
_chart_write_legend_entry(lxw_chart *self, uint16_t index)
{
    lxw_xml_start_tag(self->file, "c:legendEntry", NULL);

    /* Write the c:idx element. */
    _chart_write_idx(self, self->delete_series[index]);

    /* Write the c:delete element. */
    _chart_write_delete(self);

    lxw_xml_end_tag(self->file, "c:legendEntry");
}

/*
 * Write the <c:legend> element.
 */
STATIC void
_chart_write_legend(lxw_chart *self)
{
    uint8_t has_overlay = LXW_FALSE;
    uint16_t index;

    if (self->legend.position == LXW_CHART_LEGEND_NONE)
        return;

    lxw_xml_start_tag(self->file, "c:legend", NULL);

    /* Write the c:legendPos element. */
    switch (self->legend.position) {
        case LXW_CHART_LEGEND_LEFT:
            _chart_write_legend_pos(self, "l");
            break;
        case LXW_CHART_LEGEND_TOP:
            _chart_write_legend_pos(self, "t");
            break;
        case LXW_CHART_LEGEND_BOTTOM:
            _chart_write_legend_pos(self, "b");
            break;
        case LXW_CHART_LEGEND_OVERLAY_RIGHT:
            _chart_write_legend_pos(self, "r");
            has_overlay = LXW_TRUE;
            break;
        case LXW_CHART_LEGEND_OVERLAY_LEFT:
            _chart_write_legend_pos(self, "l");
            has_overlay = LXW_TRUE;
            break;
        default:
            _chart_write_legend_pos(self, "r");
    }

    /* Remove series labels from the legend. */
    for (index = 0; index < self->delete_series_count; index++) {
        /* Write the c:legendEntry element. */
        _chart_write_legend_entry(self, index);
    }

    /* Write the c:layout element. */
    _chart_write_layout(self);

    if (self->type == LXW_CHART_PIE || self->type == LXW_CHART_DOUGHNUT) {
        /* Write the c:overlay element. */
        if (has_overlay)
            _chart_write_overlay(self);

        /* Write the c:txPr element for Pie/Doughnut charts. */
        _chart_write_tx_pr_pie(self, LXW_FALSE, self->legend.font);
    }
    else {
        /* Write the c:txPr element for all other charts. */
        if (self->legend.font)
            _chart_write_tx_pr(self, LXW_FALSE, self->legend.font);

        /* Write the c:overlay element. */
        if (has_overlay)
            _chart_write_overlay(self);
    }

    lxw_xml_end_tag(self->file, "c:legend");
}

/*
 * Write the <c:plotVisOnly> element.
 */
STATIC void
_chart_write_plot_vis_only(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", "1");

    lxw_xml_empty_tag(self->file, "c:plotVisOnly", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:headerFooter> element.
 */
STATIC void
_chart_write_header_footer(lxw_chart *self)
{
    lxw_xml_empty_tag(self->file, "c:headerFooter", NULL);
}

/*
 * Write the <c:pageMargins> element.
 */
STATIC void
_chart_write_page_margins(lxw_chart *self)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;
    char b[] = "0.75";
    char l[] = "0.7";
    char r[] = "0.7";
    char t[] = "0.75";
    char header[] = "0.3";
    char footer[] = "0.3";

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("b", b);
    LXW_PUSH_ATTRIBUTES_STR("l", l);
    LXW_PUSH_ATTRIBUTES_STR("r", r);
    LXW_PUSH_ATTRIBUTES_STR("t", t);
    LXW_PUSH_ATTRIBUTES_STR("header", header);
    LXW_PUSH_ATTRIBUTES_STR("footer", footer);

    lxw_xml_empty_tag(self->file, "c:pageMargins", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:pageSetup> element.
 */
STATIC void
_chart_write_page_setup(lxw_chart *self)
{
    lxw_xml_empty_tag(self->file, "c:pageSetup", NULL);
}

/*
 * Write the <c:printSettings> element.
 */
STATIC void
_chart_write_print_settings(lxw_chart *self)
{
    lxw_xml_start_tag(self->file, "c:printSettings", NULL);

    /* Write the c:headerFooter element. */
    _chart_write_header_footer(self);

    /* Write the c:pageMargins element. */
    _chart_write_page_margins(self);

    /* Write the c:pageSetup element. */
    _chart_write_page_setup(self);

    lxw_xml_end_tag(self->file, "c:printSettings");
}

/*
 * Write the <c:overlap> element.
 */
STATIC void
_chart_write_overlap(lxw_chart *self, int overlap)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_INT("val", overlap);

    lxw_xml_empty_tag(self->file, "c:overlap", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write the <c:title> element.
 */
STATIC void
_chart_write_title(lxw_chart *self, lxw_chart_title *title)
{
    if (title->name) {
        /* Write the c:title element. */
        _chart_write_title_rich(self, title);
    }
    else if (title->range->formula) {
        /* Write the c:title element. */
        _chart_write_title_formula(self, title);
    }
}

/*
 * Write the <c:title> element.
 */
STATIC void
_chart_write_chart_title(lxw_chart *self)
{
    if (self->title.off) {
        /* Write the c:autoTitleDeleted element. */
        _chart_write_auto_title_deleted(self);
    }
    else {
        /* Write the c:title element. */
        _chart_write_title(self, &self->title);
    }
}

/*
 * Write the <c:catAx> element. Usually the X axis.
 */
STATIC void
_chart_write_cat_axis(lxw_chart *self)
{
    uint8_t position = self->cat_axis_position;

    lxw_xml_start_tag(self->file, "c:catAx", NULL);

    _chart_write_axis_id(self, self->axis_id_1);

    /* Write the c:scaling element. Note we can't set max, min, or log base
     * for a Category axis in Excel.*/
    _chart_write_scaling(self,
                         self->x_axis->reverse,
                         LXW_FALSE, 0.0, LXW_FALSE, 0.0, 0);

    /* Write the c:delete element to hide axis. */
    if (self->x_axis->hidden)
        _chart_write_delete(self);

    /* Write the c:axPos element. */
    _chart_write_axis_pos(self, position, self->y_axis->reverse);

    /* Write the c:majorGridlines element. */
    _chart_write_major_gridlines(self, self->x_axis);

    /* Write the c:minorGridlines element. */
    _chart_write_minor_gridlines(self, self->x_axis);

    /* Write the axis title elements. */
    self->x_axis->title.is_horizontal = self->has_horiz_cat_axis;
    _chart_write_title(self, &self->x_axis->title);

    /* Write the c:numFmt element. */
    if (self->cat_has_num_fmt)
        _chart_write_number_format(self, self->x_axis);

    /* Write the c:majorTickMark element. */
    _chart_write_major_tick_mark(self, self->x_axis);

    /* Write the c:tickLblPos element. */
    _chart_write_tick_lbl_pos(self);

    /* Write the c:spPr element for the axis line. */
    _chart_write_sp_pr(self, self->x_axis->line, self->x_axis->fill,
                       self->x_axis->pattern);

    /* Write the axis font elements. */
    _chart_write_axis_font(self, self->x_axis->num_font);

    /* Write the c:crossAx element. */
    _chart_write_cross_axis(self, self->axis_id_2);

    /* Write the c:crosses element. */
    _chart_write_crosses(self);

    /* Write the c:auto element. */
    _chart_write_auto(self);

    /* Write the c:lblAlgn element. */
    _chart_write_lbl_algn(self);

    /* Write the c:lblOffset element. */
    _chart_write_lbl_offset(self);

    lxw_xml_end_tag(self->file, "c:catAx");
}

/*
 * Write the <c:valAx> element.
 */
STATIC void
_chart_write_val_axis(lxw_chart *self)
{
    uint8_t position = self->val_axis_position;

    lxw_xml_start_tag(self->file, "c:valAx", NULL);

    _chart_write_axis_id(self, self->axis_id_2);

    /* Write the c:scaling element. */
    _chart_write_scaling(self,
                         self->y_axis->reverse,
                         self->y_axis->has_min, self->y_axis->min,
                         self->y_axis->has_max, self->y_axis->max,
                         self->y_axis->log_base);

    /* Write the c:delete element to hide axis. */
    if (self->y_axis->hidden)
        _chart_write_delete(self);

    /* Write the c:axPos element. */
    _chart_write_axis_pos(self, position, self->x_axis->reverse);

    /* Write the c:majorGridlines element. */
    _chart_write_major_gridlines(self, self->y_axis);

    /* Write the c:minorGridlines element. */
    _chart_write_minor_gridlines(self, self->y_axis);

    /* Write the axis title elements. */
    self->y_axis->title.is_horizontal = self->has_horiz_val_axis;
    _chart_write_title(self, &self->y_axis->title);

    /* Write the c:numFmt element. */
    _chart_write_number_format(self, self->y_axis);

    /* Write the c:majorTickMark element. */
    _chart_write_major_tick_mark(self, self->y_axis);

    /* Write the c:tickLblPos element. */
    _chart_write_tick_lbl_pos(self);

    /* Write the c:spPr element for the axis line. */
    _chart_write_sp_pr(self, self->y_axis->line, self->y_axis->fill,
                       self->y_axis->pattern);

    /* Write the axis font elements. */
    _chart_write_axis_font(self, self->y_axis->num_font);

    /* Write the c:crossAx element. */
    _chart_write_cross_axis(self, self->axis_id_1);

    /* Write the c:crosses element. */
    _chart_write_crosses(self);

    /* Write the c:crossBetween element. */
    _chart_write_cross_between(self);

    lxw_xml_end_tag(self->file, "c:valAx");
}

/*
 * Write the <c:valAx> element. This is for the second valAx in scatter plots.
 */
STATIC void
_chart_write_cat_val_axis(lxw_chart *self)
{
    uint8_t position = self->cat_axis_position;

    lxw_xml_start_tag(self->file, "c:valAx", NULL);

    _chart_write_axis_id(self, self->axis_id_1);

    /* Write the c:scaling element. */
    _chart_write_scaling(self,
                         self->x_axis->reverse,
                         self->x_axis->has_min, self->x_axis->min,
                         self->x_axis->has_max, self->x_axis->max,
                         self->x_axis->log_base);

    /* Write the c:delete element to hide axis. */
    if (self->x_axis->hidden)
        _chart_write_delete(self);

    /* Write the c:axPos element. */
    _chart_write_axis_pos(self, position, self->y_axis->reverse);

    /* Write the c:majorGridlines element. */
    _chart_write_major_gridlines(self, self->x_axis);

    /* Write the c:minorGridlines element. */
    _chart_write_minor_gridlines(self, self->x_axis);

    /* Write the axis title elements. */
    self->x_axis->title.is_horizontal = self->has_horiz_val_axis;
    _chart_write_title(self, &self->x_axis->title);

    /* Write the c:numFmt element. */
    _chart_write_number_format(self, self->x_axis);

    /* Write the c:majorTickMark element. */
    _chart_write_major_tick_mark(self, self->x_axis);

    /* Write the c:tickLblPos element. */
    _chart_write_tick_lbl_pos(self);

    /* Write the c:spPr element for the axis line. */
    _chart_write_sp_pr(self, self->x_axis->line, self->x_axis->fill,
                       self->x_axis->pattern);

    /* Write the axis font elements. */
    _chart_write_axis_font(self, self->x_axis->num_font);

    /* Write the c:crossAx element. */
    _chart_write_cross_axis(self, self->axis_id_2);

    /* Write the c:crosses element. */
    _chart_write_crosses(self);

    /* Write the c:crossBetween element. */
    _chart_write_cross_between(self);

    lxw_xml_end_tag(self->file, "c:valAx");
}

/*
 * Write the <c:barDir> element.
 */
STATIC void
_chart_write_bar_dir(lxw_chart *self, char *type)
{
    struct xml_attribute_list attributes;
    struct xml_attribute *attribute;

    LXW_INIT_ATTRIBUTES();
    LXW_PUSH_ATTRIBUTES_STR("val", type);

    lxw_xml_empty_tag(self->file, "c:barDir", &attributes);

    LXW_FREE_ATTRIBUTES();
}

/*
 * Write a area chart.
 */
STATIC void
_chart_write_area_chart(lxw_chart *self)
{
    lxw_chart_series *series;

    lxw_xml_start_tag(self->file, "c:areaChart", NULL);

    /* Write the c:grouping element. */
    _chart_write_grouping(self, self->grouping);

    STAILQ_FOREACH(series, self->series_list, list_pointers) {
        /* Write the c:ser element. */
        _chart_write_ser(self, series);
    }

    if (self->has_overlap) {
        /* Write the c:overlap element. */
        _chart_write_overlap(self, self->series_overlap_1);
    }

    /* Write the c:axId elements. */
    _chart_write_axis_ids(self);

    lxw_xml_end_tag(self->file, "c:areaChart");
}

/*
 * Write a bar chart.
 */
STATIC void
_chart_write_bar_chart(lxw_chart *self)
{
    lxw_chart_series *series;

    lxw_xml_start_tag(self->file, "c:barChart", NULL);

    /* Write the c:barDir element. */
    _chart_write_bar_dir(self, "bar");

    /* Write the c:grouping element. */
    _chart_write_grouping(self, self->grouping);

    STAILQ_FOREACH(series, self->series_list, list_pointers) {
        /* Write the c:ser element. */
        _chart_write_ser(self, series);
    }

    if (self->has_overlap) {
        /* Write the c:overlap element. */
        _chart_write_overlap(self, self->series_overlap_1);
    }

    /* Write the c:axId elements. */
    _chart_write_axis_ids(self);

    lxw_xml_end_tag(self->file, "c:barChart");
}

/*
 * Write a column chart.
 */
STATIC void
_chart_write_column_chart(lxw_chart *self)
{
    lxw_chart_series *series;

    lxw_xml_start_tag(self->file, "c:barChart", NULL);

    /* Write the c:barDir element. */
    _chart_write_bar_dir(self, "col");

    /* Write the c:grouping element. */
    _chart_write_grouping(self, self->grouping);

    STAILQ_FOREACH(series, self->series_list, list_pointers) {
        /* Write the c:ser element. */
        _chart_write_ser(self, series);
    }

    if (self->has_overlap) {
        /* Write the c:overlap element. */
        _chart_write_overlap(self, self->series_overlap_1);
    }

    /* Write the c:axId elements. */
    _chart_write_axis_ids(self);

    lxw_xml_end_tag(self->file, "c:barChart");
}

/*
 * Write a doughnut chart.
 */
STATIC void
_chart_write_doughnut_chart(lxw_chart *self)
{
    lxw_chart_series *series;

    lxw_xml_start_tag(self->file, "c:doughnutChart", NULL);

    /* Write the c:varyColors element. */
    _chart_write_vary_colors(self);

    STAILQ_FOREACH(series, self->series_list, list_pointers) {
        /* Write the c:ser element. */
        _chart_write_ser(self, series);
    }

    /* Write the c:firstSliceAng element. */
    _chart_write_first_slice_ang(self);

    /* Write the c:holeSize element. */
    _chart_write_hole_size(self);

    lxw_xml_end_tag(self->file, "c:doughnutChart");
}

/*
 * Write a line chart.
 */
STATIC void
_chart_write_line_chart(lxw_chart *self)
{
    lxw_chart_series *series;

    lxw_xml_start_tag(self->file, "c:lineChart", NULL);

    /* Write the c:grouping element. */
    _chart_write_grouping(self, self->grouping);

    STAILQ_FOREACH(series, self->series_list, list_pointers) {
        /* Write the c:ser element. */
        _chart_write_ser(self, series);
    }

    /* Write the c:marker element. */
    _chart_write_marker_value(self);

    /* Write the c:axId elements. */
    _chart_write_axis_ids(self);

    lxw_xml_end_tag(self->file, "c:lineChart");
}

/*
 * Write a pie chart.
 */
STATIC void
_chart_write_pie_chart(lxw_chart *self)
{
    lxw_chart_series *series;

    lxw_xml_start_tag(self->file, "c:pieChart", NULL);

    /* Write the c:varyColors element. */
    _chart_write_vary_colors(self);

    STAILQ_FOREACH(series, self->series_list, list_pointers) {
        /* Write the c:ser element. */
        _chart_write_ser(self, series);
    }

    /* Write the c:firstSliceAng element. */
    _chart_write_first_slice_ang(self);

    lxw_xml_end_tag(self->file, "c:pieChart");
}

/*
 * Write a scatter chart.
 */
STATIC void
_chart_write_scatter_chart(lxw_chart *self)
{
    lxw_chart_series *series;

    lxw_xml_start_tag(self->file, "c:scatterChart", NULL);

    /* Write the c:scatterStyle element. */
    _chart_write_scatter_style(self);

    STAILQ_FOREACH(series, self->series_list, list_pointers) {

        /* Add default scatter chart formatting to the series data unless
         * it has already been specified by the user.*/
        if (self->type == LXW_CHART_SCATTER) {
            if (!series->line) {
                lxw_chart_line line = {
                    0x000000,
                    LXW_TRUE,
                    2.25,
                    LXW_CHART_LINE_DASH_SOLID,
                    0,
                    LXW_FALSE
                };
                series->line = _chart_convert_line_args(&line);
            }
        }

        /* Write the c:ser element. */
        _chart_write_xval_ser(self, series);
    }

    /* Write the c:axId elements. */
    _chart_write_axis_ids(self);

    lxw_xml_end_tag(self->file, "c:scatterChart");
}

/*
 * Write a radar chart.
 */
STATIC void
_chart_write_radar_chart(lxw_chart *self)
{
    lxw_chart_series *series;

    lxw_xml_start_tag(self->file, "c:radarChart", NULL);

    /* Write the c:radarStyle element. */
    _chart_write_radar_style(self);

    STAILQ_FOREACH(series, self->series_list, list_pointers) {
        /* Write the c:ser element. */
        _chart_write_ser(self, series);
    }

    if (self->has_overlap) {
        /* Write the c:overlap element. */
        _chart_write_overlap(self, self->series_overlap_1);
    }

    /* Write the c:axId elements. */
    _chart_write_axis_ids(self);

    lxw_xml_end_tag(self->file, "c:radarChart");
}

/*
 * Write the <c:plotArea> element.
 */
STATIC void
_chart_write_scatter_plot_area(lxw_chart *self)
{
    lxw_xml_start_tag(self->file, "c:plotArea", NULL);

    /* Write the c:layout element. */
    _chart_write_layout(self);

    /* Write subclass chart type elements for primary and secondary axes. */
    self->write_chart_type(self);

    /* Write the c:catAx element. */
    _chart_write_cat_val_axis(self);

    self->has_horiz_val_axis = LXW_TRUE;

    /* Write the c:valAx element. */
    _chart_write_val_axis(self);

    /* Write the c:spPr element for the plotarea formatting. */
    _chart_write_sp_pr(self, self->plotarea_line, self->plotarea_fill,
                       self->plotarea_pattern);

    lxw_xml_end_tag(self->file, "c:plotArea");
}

/*
 * Write the <c:plotArea> element. Special handling for pie/doughnut.
 */
STATIC void
_chart_write_pie_plot_area(lxw_chart *self)
{
    lxw_xml_start_tag(self->file, "c:plotArea", NULL);

    /* Write the c:layout element. */
    _chart_write_layout(self);

    /* Write subclass chart type elements for primary and secondary axes. */
    self->write_chart_type(self);

    lxw_xml_end_tag(self->file, "c:plotArea");
}

/*
 * Write the <c:plotArea> element.
 */
STATIC void
_chart_write_plot_area(lxw_chart *self)
{
    lxw_xml_start_tag(self->file, "c:plotArea", NULL);

    /* Write the c:layout element. */
    _chart_write_layout(self);

    /* Write subclass chart type elements for primary and secondary axes. */
    self->write_chart_type(self);

    /* Write the c:catAx element. */
    _chart_write_cat_axis(self);

    /* Write the c:valAx element. */
    _chart_write_val_axis(self);

    /* Write the c:spPr element for the plotarea formatting. */
    _chart_write_sp_pr(self, self->plotarea_line, self->plotarea_fill,
                       self->plotarea_pattern);

    lxw_xml_end_tag(self->file, "c:plotArea");
}

/*
 * Write the <c:chart> element.
 */
STATIC void
_chart_write_chart(lxw_chart *self)
{
    lxw_xml_start_tag(self->file, "c:chart", NULL);

    /* Write the c:title element. */
    _chart_write_chart_title(self);

    /* Write the c:plotArea element. */
    self->write_plot_area(self);

    /* Write the c:legend element. */
    _chart_write_legend(self);

    /* Write the c:plotVisOnly element. */
    _chart_write_plot_vis_only(self);

    lxw_xml_end_tag(self->file, "c:chart");
}

/*
 * Initialize a area chart.
 */
STATIC void
_chart_initialize_area_chart(lxw_chart *self, uint8_t type)
{
    self->grouping = LXW_GROUPING_STANDARD;
    self->cross_between = LXW_CHART_AXIS_POSITION_ON_TICK;

    if (type == LXW_CHART_AREA_STACKED) {
        self->grouping = LXW_GROUPING_STACKED;
        self->subtype = LXW_CHART_SUBTYPE_STACKED;
    }

    if (type == LXW_CHART_AREA_STACKED_PERCENT) {
        self->grouping = LXW_GROUPING_PERCENTSTACKED;
        lxw_strcpy((self->y_axis)->default_num_format, "0%");
        self->subtype = LXW_CHART_SUBTYPE_STACKED;
    }

    /* Initialize the function pointers for this chart type. */
    self->write_chart_type = _chart_write_area_chart;
    self->write_plot_area = _chart_write_plot_area;
}

/*
 * Initialize a bar chart.
 */
STATIC void
_chart_initialize_bar_chart(lxw_chart *self, uint8_t type)
{
    lxw_chart_axis *tmp;

    /* Reverse the X and Y axes for Bar charts. */
    tmp = self->x_axis;
    self->x_axis = self->y_axis;
    self->y_axis = tmp;

    /*Also reverse some of the defaults. */
    self->x_axis->major_gridlines.visible = LXW_FALSE;
    self->y_axis->major_gridlines.visible = LXW_TRUE;

    self->has_horiz_cat_axis = LXW_TRUE;
    self->has_horiz_val_axis = LXW_FALSE;

    if (type == LXW_CHART_BAR_STACKED) {
        self->grouping = LXW_GROUPING_STACKED;
        self->has_overlap = LXW_TRUE;
        self->subtype = LXW_CHART_SUBTYPE_STACKED;
    }

    if (type == LXW_CHART_BAR_STACKED_PERCENT) {
        self->grouping = LXW_GROUPING_PERCENTSTACKED;
        lxw_strcpy((self->y_axis)->default_num_format, "0%");
        self->has_overlap = LXW_TRUE;
        self->subtype = LXW_CHART_SUBTYPE_STACKED;
    }

    /* Override the default axis positions for a bar chart. */
    self->cat_axis_position = LXW_CHART_AXIS_LEFT;
    self->val_axis_position = LXW_CHART_AXIS_BOTTOM;

    /* Initialize the function pointers for this chart type. */
    self->write_chart_type = _chart_write_bar_chart;
    self->write_plot_area = _chart_write_plot_area;
}

/*
 * Initialize a column chart.
 */
STATIC void
_chart_initialize_column_chart(lxw_chart *self, uint8_t type)
{
    self->has_horiz_val_axis = LXW_FALSE;

    if (type == LXW_CHART_COLUMN_STACKED) {
        self->grouping = LXW_GROUPING_STACKED;
        self->has_overlap = LXW_TRUE;
        self->subtype = LXW_CHART_SUBTYPE_STACKED;
    }

    if (type == LXW_CHART_COLUMN_STACKED_PERCENT) {
        self->grouping = LXW_GROUPING_PERCENTSTACKED;
        lxw_strcpy((self->y_axis)->default_num_format, "0%");
        self->has_overlap = LXW_TRUE;
        self->subtype = LXW_CHART_SUBTYPE_STACKED;
    }

    /* Initialize the function pointers for this chart type. */
    self->write_chart_type = _chart_write_column_chart;
    self->write_plot_area = _chart_write_plot_area;
}

/*
 * Initialize a doughnut chart.
 */
STATIC void
_chart_initialize_doughnut_chart(lxw_chart *self)
{
    /* Initialize the function pointers for this chart type. */
    self->write_chart_type = _chart_write_doughnut_chart;
    self->write_plot_area = _chart_write_pie_plot_area;
}

/*
 * Initialize a line chart.
 */
STATIC void
_chart_initialize_line_chart(lxw_chart *self)
{
    _chart_set_default_marker_type(self, LXW_CHART_MARKER_NONE);
    self->grouping = LXW_GROUPING_STANDARD;

    /* Initialize the function pointers for this chart type. */
    self->write_chart_type = _chart_write_line_chart;
    self->write_plot_area = _chart_write_plot_area;
}

/*
 * Initialize a pie chart.
 */
STATIC void
_chart_initialize_pie_chart(lxw_chart *self)
{
    /* Initialize the function pointers for this chart type. */
    self->write_chart_type = _chart_write_pie_chart;
    self->write_plot_area = _chart_write_pie_plot_area;
}

/*
 * Initialize a scatter chart.
 */
STATIC void
_chart_initialize_scatter_chart(lxw_chart *self)
{
    self->has_horiz_val_axis = LXW_FALSE;
    self->cross_between = LXW_CHART_AXIS_POSITION_ON_TICK;
    self->is_scatter_chart = LXW_TRUE;

    if (self->type == LXW_CHART_SCATTER_STRAIGHT
        || self->type == LXW_CHART_SCATTER_SMOOTH) {

        _chart_set_default_marker_type(self, LXW_CHART_MARKER_NONE);
    }

    /* Initialize the function pointers for this chart type. */
    self->write_chart_type = _chart_write_scatter_chart;
    self->write_plot_area = _chart_write_scatter_plot_area;
}

/*
 * Initialize a radar chart.
 */
STATIC void
_chart_initialize_radar_chart(lxw_chart *self, uint8_t type)
{
    if (type == LXW_CHART_RADAR)
        _chart_set_default_marker_type(self, LXW_CHART_MARKER_NONE);

    self->x_axis->major_gridlines.visible = LXW_TRUE;

    self->y_axis->major_tick_mark = LXW_TRUE;

    /* Initialize the function pointers for this chart type. */
    self->write_chart_type = _chart_write_radar_chart;
    self->write_plot_area = _chart_write_plot_area;
}

/*
 * Initialize the chart specific properties.
 */
STATIC void
_chart_initialize(lxw_chart *self, uint8_t type)
{
    switch (type) {

        case LXW_CHART_AREA:
        case LXW_CHART_AREA_STACKED:
        case LXW_CHART_AREA_STACKED_PERCENT:
            _chart_initialize_area_chart(self, type);
            break;

        case LXW_CHART_BAR:
        case LXW_CHART_BAR_STACKED:
        case LXW_CHART_BAR_STACKED_PERCENT:
            _chart_initialize_bar_chart(self, type);
            break;

        case LXW_CHART_COLUMN:
        case LXW_CHART_COLUMN_STACKED:
        case LXW_CHART_COLUMN_STACKED_PERCENT:
            _chart_initialize_column_chart(self, type);
            break;

        case LXW_CHART_DOUGHNUT:
            _chart_initialize_doughnut_chart(self);
            break;

        case LXW_CHART_LINE:
            _chart_initialize_line_chart(self);
            break;

        case LXW_CHART_PIE:
            _chart_initialize_pie_chart(self);
            break;

        case LXW_CHART_SCATTER:
        case LXW_CHART_SCATTER_STRAIGHT:
        case LXW_CHART_SCATTER_STRAIGHT_WITH_MARKERS:
        case LXW_CHART_SCATTER_SMOOTH:
        case LXW_CHART_SCATTER_SMOOTH_WITH_MARKERS:
            _chart_initialize_scatter_chart(self);
            break;

        case LXW_CHART_RADAR:
        case LXW_CHART_RADAR_WITH_MARKERS:
        case LXW_CHART_RADAR_FILLED:
            _chart_initialize_radar_chart(self, type);
            break;

        default:
            LXW_WARN_FORMAT1("workbook_add_chart(): "
                             "unhandled chart type '%d'", type);
    }
}

/*
 * Assemble and write the XML file.
 */
void
lxw_chart_assemble_xml_file(lxw_chart *self)
{
    /* Initialize the chart specific properties. */
    _chart_initialize(self, self->type);

    /* Write the XML declaration. */
    _chart_xml_declaration(self);

    /* Write the c:chartSpace element. */
    _chart_write_chart_space(self);

    /* Write the c:lang element. */
    _chart_write_lang(self);

    /* Write the c:style element. */
    _chart_write_style(self);

    /* Write the c:chart element. */
    _chart_write_chart(self);

    /* Write the c:spPr element for the chartarea formatting. */
    _chart_write_sp_pr(self, self->chartarea_line, self->chartarea_fill,
                       self->chartarea_pattern);

    /* Write the c:printSettings element. */
    _chart_write_print_settings(self);

    lxw_xml_end_tag(self->file, "c:chartSpace");
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/

/*
 * Add data to a data cache in a range object, for testing only.
 */
int
lxw_chart_add_data_cache(lxw_series_range *range, uint8_t *data,
                         uint16_t rows, uint8_t cols, uint8_t col)
{
    struct lxw_series_data_point *data_point;
    uint16_t i;

    range->ignore_cache = LXW_TRUE;
    range->num_data_points = rows;

    /* Initialize the series range data cache. */
    for (i = 0; i < rows; i++) {
        data_point = calloc(1, sizeof(struct lxw_series_data_point));
        STAILQ_INSERT_TAIL(range->data_cache, data_point, list_pointers);
        data_point->number = data[i * cols + col];
    }

    return 0;
}

/*
 * Insert an image into the worksheet.
 */
lxw_chart_series *
chart_add_series(lxw_chart *self, const char *categories, const char *values)
{
    lxw_chart_series *series;

    /* Create a new object to hold the series. */
    series = calloc(1, sizeof(lxw_chart_series));
    GOTO_LABEL_ON_MEM_ERROR(series, mem_error);

    series->categories = calloc(1, sizeof(lxw_series_range));
    GOTO_LABEL_ON_MEM_ERROR(series->categories, mem_error);

    series->values = calloc(1, sizeof(lxw_series_range));
    GOTO_LABEL_ON_MEM_ERROR(series->values, mem_error);

    series->title.range = calloc(1, sizeof(lxw_series_range));
    GOTO_LABEL_ON_MEM_ERROR(series->title.range, mem_error);

    if (categories) {
        if (categories[0] == '=')
            series->categories->formula = lxw_strdup(categories + 1);
        else
            series->categories->formula = lxw_strdup(categories);
    }

    if (values) {
        if (values[0] == '=')
            series->values->formula = lxw_strdup(values + 1);
        else
            series->values->formula = lxw_strdup(values);
    }

    if (_chart_init_data_cache(series->categories) != LXW_NO_ERROR)
        goto mem_error;

    if (_chart_init_data_cache(series->values) != LXW_NO_ERROR)
        goto mem_error;

    if (_chart_init_data_cache(series->title.range) != LXW_NO_ERROR)
        goto mem_error;

    STAILQ_INSERT_TAIL(self->series_list, series, list_pointers);

    return series;

mem_error:
    _chart_series_free(series);
    return NULL;
}

/*
 * Set on of the 48 built-in Excel chart styles.
 */
void
chart_set_style(lxw_chart *self, uint8_t style_id)
{
    /* The default style is 2. The range is 1 - 48 */
    if (style_id < 1 || style_id > 48)
        style_id = 2;

    self->style_id = style_id;
}

/*
 * Set a user defined name for a series.
 */
void
chart_series_set_name(lxw_chart_series *series, const char *name)
{
    if (!name)
        return;

    if (name[0] == '=')
        series->title.range->formula = lxw_strdup(name + 1);
    else
        series->title.name = lxw_strdup(name);
}

/*
 * Set an axis caption, with a range instead or a formula..
 */
void
chart_series_set_name_range(lxw_chart_series *series, const char *sheetname,
                            lxw_row_t row, lxw_col_t col)
{
    if (!sheetname) {
        LXW_WARN("chart_series_set_name_range(): "
                 "sheetname must be specified");
        return;
    }

    /* Start and end row, col are the same for single cell range. */
    _chart_set_range(series->title.range, sheetname, row, col, row, col);
}

/*
 * Set the categories range for a series.
 */
void
chart_series_set_categories(lxw_chart_series *series, const char *sheetname,
                            lxw_row_t first_row, lxw_col_t first_col,
                            lxw_row_t last_row, lxw_col_t last_col)
{
    if (!sheetname) {
        LXW_WARN("chart_series_set_categories(): "
                 "sheetname must be specified");
        return;
    }

    _chart_set_range(series->categories, sheetname,
                     first_row, first_col, last_row, last_col);
}

/*
 * Set the values range for a series.
 */
void
chart_series_set_values(lxw_chart_series *series, const char *sheetname,
                        lxw_row_t first_row, lxw_col_t first_col,
                        lxw_row_t last_row, lxw_col_t last_col)
{
    if (!sheetname) {
        LXW_WARN("chart_series_set_values(): sheetname must be specified");
        return;
    }

    _chart_set_range(series->values, sheetname,
                     first_row, first_col, last_row, last_col);
}

/*
 * Set a line type for a series.
 */
void
chart_series_set_line(lxw_chart_series *series, lxw_chart_line *line)
{
    if (!line)
        return;

    /* Free any previously allocated resource. */
    free(series->line);

    series->line = _chart_convert_line_args(line);
}

/*
 * Set a fill type for a series.
 */
void
chart_series_set_fill(lxw_chart_series *series, lxw_chart_fill *fill)
{
    if (!fill)
        return;

    /* Free any previously allocated resource. */
    free(series->fill);

    series->fill = _chart_convert_fill_args(fill);
}

/*
 * Set a pattern type for a series.
 */
void
chart_series_set_pattern(lxw_chart_series *series, lxw_chart_pattern *pattern)
{
    if (!pattern)
        return;

    /* Free any previously allocated resource. */
    free(series->pattern);

    series->pattern = _chart_convert_pattern_args(pattern);
}

/*
 * Set a marker type for a series.
 */
void
chart_series_set_marker_type(lxw_chart_series *series, uint8_t type)
{
    if (!series->marker) {
        lxw_chart_marker *marker = calloc(1, sizeof(struct lxw_chart_marker));
        RETURN_VOID_ON_MEM_ERROR(marker);
        series->marker = marker;
    }

    series->marker->type = type;
}

/*
 * Set a marker size for a series.
 */
void
chart_series_set_marker_size(lxw_chart_series *series, uint8_t size)
{
    if (!series->marker) {
        lxw_chart_marker *marker = calloc(1, sizeof(struct lxw_chart_marker));
        RETURN_VOID_ON_MEM_ERROR(marker);
        series->marker = marker;
    }

    series->marker->size = size;
}

/*
 * Set a line type for a series marker.
 */
void
chart_series_set_marker_line(lxw_chart_series *series, lxw_chart_line *line)
{
    if (!line)
        return;

    if (!series->marker) {
        lxw_chart_marker *marker = calloc(1, sizeof(struct lxw_chart_marker));
        RETURN_VOID_ON_MEM_ERROR(marker);
        series->marker = marker;
    }

    /* Free any previously allocated resource. */
    free(series->marker->line);

    series->marker->line = _chart_convert_line_args(line);
}

/*
 * Set a fill type for a series marker.
 */
void
chart_series_set_marker_fill(lxw_chart_series *series, lxw_chart_fill *fill)
{
    if (!fill)
        return;

    if (!series->marker) {
        lxw_chart_marker *marker = calloc(1, sizeof(struct lxw_chart_marker));
        RETURN_VOID_ON_MEM_ERROR(marker);
        series->marker = marker;
    }

    /* Free any previously allocated resource. */
    free(series->marker->fill);

    series->marker->fill = _chart_convert_fill_args(fill);
}

/*
 * Set a pattern type for a series.
 */
void
chart_series_set_marker_pattern(lxw_chart_series *series,
                                lxw_chart_pattern *pattern)
{
    if (!pattern)
        return;

    if (!series->marker) {
        lxw_chart_marker *marker = calloc(1, sizeof(struct lxw_chart_marker));
        RETURN_VOID_ON_MEM_ERROR(marker);
        series->marker = marker;
    }

    /* Free any previously allocated resource. */
    free(series->marker->pattern);

    series->marker->pattern = _chart_convert_pattern_args(pattern);
}

/*
 * Set an axis caption.
 */
void
chart_axis_set_name(lxw_chart_axis *axis, const char *name)
{
    if (!name)
        return;

    if (name[0] == '=')
        axis->title.range->formula = lxw_strdup(name + 1);
    else
        axis->title.name = lxw_strdup(name);
}

/*
 * Set an axis caption, with a range instead or a formula..
 */
void
chart_axis_set_name_range(lxw_chart_axis *axis, const char *sheetname,
                          lxw_row_t row, lxw_col_t col)
{
    if (!sheetname) {
        LXW_WARN("chart_axis_set_name_range(): sheetname must be specified");
        return;
    }

    /* Start and end row, col are the same for single cell range. */
    _chart_set_range(axis->title.range, sheetname, row, col, row, col);
}

/*
 * Set an axis title/name font.
 */
void
chart_axis_set_name_font(lxw_chart_axis *axis, lxw_chart_font *font)
{
    /* Free any previously allocated resource. */
    free(axis->title.font);

    axis->title.font = _chart_convert_font_args(font);
}

/*
 * Set an axis number font.
 */
void
chart_axis_set_num_font(lxw_chart_axis *axis, lxw_chart_font *font)
{
    /* Free any previously allocated resource. */
    free(axis->num_font);

    axis->num_font = _chart_convert_font_args(font);
}

/*
 * Set a line type for an axis.
 */
void
chart_axis_set_line(lxw_chart_axis *axis, lxw_chart_line *line)
{
    if (!line)
        return;

    /* Free any previously allocated resource. */
    free(axis->line);

    axis->line = _chart_convert_line_args(line);
}

/*
 * Set a fill type for an axis.
 */
void
chart_axis_set_fill(lxw_chart_axis *axis, lxw_chart_fill *fill)
{
    if (!fill)
        return;

    /* Free any previously allocated resource. */
    free(axis->fill);

    axis->fill = _chart_convert_fill_args(fill);
}

/*
 * Set a pattern type for an axis.
 */
void
chart_axis_set_pattern(lxw_chart_axis *axis, lxw_chart_pattern *pattern)
{
    if (!pattern)
        return;

    /* Free any previously allocated resource. */
    free(axis->pattern);

    axis->pattern = _chart_convert_pattern_args(pattern);
}

/*
 * Reverse the direction of an axis.
 */
void
chart_axis_set_reverse(lxw_chart_axis *axis)
{
    axis->reverse = LXW_TRUE;
}

/*
 * Turn off/hide the axis.
 */
void
chart_axis_off(lxw_chart_axis *axis)
{
    axis->hidden = LXW_TRUE;
}

/*
 * Set the minimum value for an axis.
 */
void
chart_axis_set_min(lxw_chart_axis *axis, double min)
{
    axis->min = min;
    axis->has_min = LXW_TRUE;
}

/*
 * Set the maximum value for an axis.
 */
void
chart_axis_set_max(lxw_chart_axis *axis, double max)
{
    axis->max = max;
    axis->has_max = LXW_TRUE;
}

/*
 * Set the log base for an axis.
 */
void
chart_axis_set_log_base(lxw_chart_axis *axis, uint16_t log_base)
{
    /* Excel log range is 2-1000. */
    if (log_base >= 2 && log_base <= 1000)
        axis->log_base = log_base;
}

/*
 * Set the axis major gridlines on/off.
 */
void
chart_axis_major_gridlines_set_visible(lxw_chart_axis *axis, uint8_t visible)
{
    axis->major_gridlines.visible = visible;
}

/*
 * Set a line type for the major gridlines.
 */
void
chart_axis_major_gridlines_set_line(lxw_chart_axis *axis,
                                    lxw_chart_line *line)
{
    if (!line)
        return;

    /* Free any previously allocated resource. */
    free(axis->major_gridlines.line);

    axis->major_gridlines.line = _chart_convert_line_args(line);

    /* If the gridline has a format it should also be visible. */
    if (axis->major_gridlines.line)
        axis->major_gridlines.visible = LXW_TRUE;
}

/*
 * Set the axis minor gridlines on/off.
 */
void
chart_axis_minor_gridlines_set_visible(lxw_chart_axis *axis, uint8_t visible)
{
    axis->minor_gridlines.visible = visible;
}

/*
 * Set a line type for the minor gridlines.
 */
void
chart_axis_minor_gridlines_set_line(lxw_chart_axis *axis,
                                    lxw_chart_line *line)
{
    if (!line)
        return;

    /* Free any previously allocated resource. */
    free(axis->minor_gridlines.line);

    axis->minor_gridlines.line = _chart_convert_line_args(line);

    /* If the gridline has a format it should also be visible. */
    if (axis->minor_gridlines.line)
        axis->minor_gridlines.visible = LXW_TRUE;
}

/*
 * Set the chart title.
 */
void
chart_title_set_name(lxw_chart *self, const char *name)
{
    if (!name)
        return;

    if (name[0] == '=')
        self->title.range->formula = lxw_strdup(name + 1);
    else
        self->title.name = lxw_strdup(name);
}

/*
 * Set the chart title, with a range instead or a formula.
 */
void
chart_title_set_name_range(lxw_chart *self, const char *sheetname,
                           lxw_row_t row, lxw_col_t col)
{
    if (!sheetname) {
        LXW_WARN("chart_title_set_name_range(): sheetname must be specified");
        return;
    }

    /* Start and end row, col are the same for single cell range. */
    _chart_set_range(self->title.range, sheetname, row, col, row, col);
}

/*
 * Set the chart title font.
 */
void
chart_title_set_name_font(lxw_chart *self, lxw_chart_font *font)
{
    /* Free any previously allocated resource. */
    free(self->title.font);

    self->title.font = _chart_convert_font_args(font);
}

/*
 * Turn off the chart title.
 */
void
chart_title_off(lxw_chart *self)
{
    self->title.off = LXW_TRUE;
}

/*
 * Set the chart legend position.
 */
void
chart_legend_set_position(lxw_chart *self, uint8_t position)
{
    self->legend.position = position;
}

/*
 * Set the legend font.
 */
void
chart_legend_set_font(lxw_chart *self, lxw_chart_font *font)
{
    /* Free any previously allocated resource. */
    free(self->legend.font);

    self->legend.font = _chart_convert_font_args(font);
}

/*
 * Remove one or more series from the the legend.
 */
lxw_error
chart_legend_delete_series(lxw_chart *self, int16_t delete_series[])
{
    uint16_t count = 0;

    if (delete_series == NULL)
        return LXW_ERROR_NULL_PARAMETER_IGNORED;

    while (delete_series[count] > 0)
        count++;

    /* The maximum number of series in a chart is 255. */
    if (count > 255)
        count = 255;

    self->delete_series = calloc(count, sizeof(int16_t));
    RETURN_ON_MEM_ERROR(self->delete_series, LXW_ERROR_MEMORY_MALLOC_FAILED);
    memcpy(self->delete_series, delete_series, count * sizeof(int16_t));
    self->delete_series_count = count;

    return LXW_NO_ERROR;
}

/*
 * Set a line type for the chartarea.
 */
void
chart_chartarea_set_line(lxw_chart *self, lxw_chart_line *line)
{
    if (!line)
        return;

    /* Free any previously allocated resource. */
    free(self->chartarea_line);

    self->chartarea_line = _chart_convert_line_args(line);
}

/*
 * Set a fill type for the chartarea.
 */
void
chart_chartarea_set_fill(lxw_chart *self, lxw_chart_fill *fill)
{
    if (!fill)
        return;

    /* Free any previously allocated resource. */
    free(self->chartarea_fill);

    self->chartarea_fill = _chart_convert_fill_args(fill);
}

/*
 * Set a pattern type for the chartarea.
 */
void
chart_chartarea_set_pattern(lxw_chart *self, lxw_chart_pattern *pattern)
{
    if (!pattern)
        return;

    /* Free any previously allocated resource. */
    free(self->chartarea_pattern);

    self->chartarea_pattern = _chart_convert_pattern_args(pattern);
}

/*
 * Set a line type for the plotarea.
 */
void
chart_plotarea_set_line(lxw_chart *self, lxw_chart_line *line)
{
    if (!line)
        return;

    /* Free any previously allocated resource. */
    free(self->plotarea_line);

    self->plotarea_line = _chart_convert_line_args(line);
}

/*
 * Set a fill type for the plotarea.
 */
void
chart_plotarea_set_fill(lxw_chart *self, lxw_chart_fill *fill)
{
    if (!fill)
        return;

    /* Free any previously allocated resource. */
    free(self->plotarea_fill);

    self->plotarea_fill = _chart_convert_fill_args(fill);
}

/*
 * Set a pattern type for the plotarea.
 */
void
chart_plotarea_set_pattern(lxw_chart *self, lxw_chart_pattern *pattern)
{
    if (!pattern)
        return;

    /* Free any previously allocated resource. */
    free(self->plotarea_pattern);

    self->plotarea_pattern = _chart_convert_pattern_args(pattern);
}

/*
 * Set the Pie/Doughnut chart rotation: the angle of the first slice.
 */
void
chart_set_rotation(lxw_chart *self, uint16_t rotation)
{
    if (rotation <= 360)
        self->rotation = rotation;
    else
        LXW_WARN_FORMAT1("chart_set_rotation(): Chart rotation '%d' outside "
                         "range: 0 <= rotation <= 360", rotation);
}

/*
 * Set the Doughnut chart hole size.
 */
void
chart_set_hole_size(lxw_chart *self, uint8_t size)
{
    if (size >= 10 && size <= 90)
        self->hole_size = size;
    else
        LXW_WARN_FORMAT1("chart_set_hole_size(): Hole size '%d' outside "
                         "Excel range: 10 <= size <= 90", size);
}
