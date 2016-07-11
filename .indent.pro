/*
 * Indent rules for libxlsxwriter.
 *
 * The rules for user defined typedefs can be update as follows:
 *
   perl -i -pe 'print and last if /[l]ibxlsxwriter typedefs/' .indent.pro
   ack -h typedef include/xlsxwriter/*.h src/*.c | perl -lne 'print "-T $1" if /\w+\s+\w+\s+(\w+)/' | sort >> .indent.pro
 *
 */

/* Command line options used with GNU indent 2.2.10 */
--braces-on-if-line
--braces-on-struct-decl-line
--case-indentation 4
--continue-at-parentheses
--declaration-comment-column 0
--format-first-column-comments
--honour-newlines
--ignore-profile
--indent-label 0
--indent-level 4
--no-space-after-function-call-names
--no-tabs
--swallow-optional-blank-lines

/* Typedefs used in the code. */
-T int8_t
-T int16_t
-T int32_t
-T int64_t
-T uint8_t
-T uint16_t
-T uint32_t
-T uint64_t
-T ssize_t
-T size_t
-T time_t

-T LIST_ENTRY
-T RB_ENTRY
-T SLIST_ENTRY
-T STAILQ_ENTRY
-T TAILQ_ENTRY

/* libxlsxwriter typedefs. */
-T lxw_app
-T lxw_autofilter
-T lxw_border
-T lxw_cell
-T lxw_chart
-T lxw_chart_axis
-T lxw_chart_font
-T lxw_chart_series
-T lxw_chart_title
-T lxw_chart_types
-T lxw_col_options
-T lxw_col_t
-T lxw_color_t
-T lxw_content_types
-T lxw_core
-T lxw_custom
-T lxw_custom_property
-T lxw_datetime
-T lxw_defined_name
-T lxw_doc_properties
-T lxw_drawing
-T lxw_drawing_coords
-T lxw_drawing_object
-T lxw_error
-T lxw_fill
-T lxw_font
-T lxw_format
-T lxw_hash_element
-T lxw_hash_table
-T lxw_header_footer_options
-T lxw_heading_pair
-T lxw_image_options
-T lxw_merged_range
-T lxw_packager
-T lxw_panes
-T lxw_part_name
-T lxw_print_area
-T lxw_protection
-T lxw_rel_tuple
-T lxw_relationships
-T lxw_repeat_cols
-T lxw_repeat_rows
-T lxw_row
-T lxw_row_col_options
-T lxw_row_t
-T lxw_selection
-T lxw_series_data_point
-T lxw_series_range
-T lxw_sst
-T lxw_styles
-T lxw_theme
-T lxw_tuple
-T lxw_workbook
-T lxw_workbook_options
-T lxw_worksheet
-T lxw_worksheet_init_data
-T lxw_worksheet_name
