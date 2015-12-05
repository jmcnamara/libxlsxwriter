/*****************************************************************************
 * packager - A library for creating Excel XLSX packager files.
 *
 * Used in conjunction with the libxlsxwriter library.
 *
 * Copyright 2014-2015, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 */

#include "xlsxwriter/xmlwriter.h"
#include "xlsxwriter/packager.h"
#include "xlsxwriter/hash_table.h"
#include "xlsxwriter/utility.h"

int16_t _add_file_to_zip(lxw_packager *self, FILE * file,
                         const char *filename);

/*
 * Forward declarations.
 */

/*****************************************************************************
 *
 * Private functions.
 *
 ****************************************************************************/
#ifdef _WIN32

#include <windows.h>
#include "../third_party/minizip/iowin32.h"

zipFile
_open_zipfile_win32(const char *filename)
{
    int n;
    wchar_t wide_filename[_MAX_PATH + 1] = L"";

    /* Build a UTF-16 filename for Win32. */
    n = MultiByteToWideChar(CP_UTF8, 0, filename, strlen(filename),
                            wide_filename, _MAX_PATH);

    if (n == 0) {
        LXW_ERROR("MultiByteToWideChar error");
        return NULL;
    }

    /* Use the native Win32 file handling functions with minizip. */
    zlib_filefunc64_def filefunc;
    fill_win32_filefunc64(&filefunc);

    return zipOpen2_64(wide_filename, 0, NULL, &filefunc);
}

#endif

/*
 * Create a new packager object.
 */
lxw_packager *
_new_packager(const char *filename)
{
    struct tm *file_date;
    time_t now = time(NULL);
    lxw_packager *packager = calloc(1, sizeof(lxw_packager));
    GOTO_LABEL_ON_MEM_ERROR(packager, mem_error);

    packager->buffer = calloc(1, LXW_ZIP_BUFFER_SIZE);
    GOTO_LABEL_ON_MEM_ERROR(packager->buffer, mem_error);

    packager->filename = lxw_strdup(filename);
    GOTO_LABEL_ON_MEM_ERROR(packager->filename, mem_error);

    packager->buffer_size = LXW_ZIP_BUFFER_SIZE;

    /* Initialise the zip_fileinfo struct. */
    file_date = localtime(&now);
    packager->zipfile_info.tmz_date.tm_sec = file_date->tm_sec;
    packager->zipfile_info.tmz_date.tm_min = file_date->tm_min;
    packager->zipfile_info.tmz_date.tm_hour = file_date->tm_hour;
    packager->zipfile_info.tmz_date.tm_mday = file_date->tm_mday;
    packager->zipfile_info.tmz_date.tm_mon = file_date->tm_mon;
    packager->zipfile_info.tmz_date.tm_year = file_date->tm_year;
    packager->zipfile_info.dosDate = 0;
    packager->zipfile_info.internal_fa = 0;
    packager->zipfile_info.external_fa = 0;

    /* Create a zip container for the xlsx file. */
#ifdef _WIN32
    packager->zipfile = _open_zipfile_win32(packager->filename);
#else
    packager->zipfile = zipOpen(packager->filename, 0);
#endif

    if (packager->zipfile == NULL)
        goto mem_error;

    return packager;

mem_error:
    _free_packager(packager);
    return NULL;
}

/*
 * Free a packager object.
 */
void
_free_packager(lxw_packager *packager)
{
    if (!packager)
        return;

    free(packager->buffer);
    free(packager->filename);
    free(packager);
}

/*****************************************************************************
 *
 * File assembly functions.
 *
 ****************************************************************************/
/*
 * Write the workbook.xml file.
 */
STATIC uint8_t
_write_workbook_file(lxw_packager *self)
{
    lxw_workbook *workbook = self->workbook;

    workbook->file = lxw_tmpfile();

    _workbook_assemble_xml_file(workbook);

    _add_file_to_zip(self, workbook->file, "xl/workbook.xml");

    fclose(workbook->file);

    return 0;
}

/*
 * Write the worksheet files.
 */
STATIC uint8_t
_write_worksheet_files(lxw_packager *self)
{
    lxw_workbook *workbook = self->workbook;
    lxw_worksheet *worksheet;
    char sheetname[FILENAME_LEN] = { 0 };
    uint16_t index = 1;

    STAILQ_FOREACH(worksheet, workbook->worksheets, list_pointers) {
        lxw_snprintf(sheetname, FILENAME_LEN,
                     "xl/worksheets/sheet%d.xml", index++);

        if (worksheet->optimize_row)
            _worksheet_write_single_row(worksheet);

        worksheet->file = lxw_tmpfile();

        _worksheet_assemble_xml_file(worksheet);

        _add_file_to_zip(self, worksheet->file, sheetname);

        fclose(worksheet->file);
    }

    return 0;
}

/*
 * Write the sharedStrings.xml file.
 */
STATIC uint8_t
_write_shared_strings_file(lxw_packager *self)
{
    lxw_sst *sst = self->workbook->sst;

    /* Skip the sharedStrings file if there are no shared strings. */
    if (!sst->string_count)
        return 0;

    sst->file = lxw_tmpfile();

    _sst_assemble_xml_file(sst);

    _add_file_to_zip(self, sst->file, "xl/sharedStrings.xml");

    fclose(sst->file);

    return 0;
}

/*
 * Write the app.xml file.
 */
STATIC uint8_t
_write_app_file(lxw_packager *self)
{
    lxw_workbook *workbook = self->workbook;
    lxw_worksheet *worksheet;
    lxw_defined_name *defined_name;
    lxw_app *app = _new_app();
    uint16_t named_range_count = 0;
    char *autofilter;
    char *has_range;
    char number[ATTR_32] = { 0 };

    app->file = lxw_tmpfile();

    lxw_snprintf(number, ATTR_32, "%d", self->workbook->num_sheets);

    _add_heading_pair(app, "Worksheets", number);

    STAILQ_FOREACH(worksheet, workbook->worksheets, list_pointers) {
        _add_part_name(app, worksheet->name);
    }

    /* Add the Named Ranges parts. */
    TAILQ_FOREACH(defined_name, workbook->defined_names, list_pointers) {

        has_range = strchr(defined_name->formula, '!');
        autofilter = strstr(defined_name->app_name, "_FilterDatabase");

        /* Only store defined names with ranges (except for autofilters). */
        if (has_range && !autofilter) {
            _add_part_name(app, defined_name->app_name);
            named_range_count++;
        }
    }

    /* Add the Named Range heading pairs. */
    if (named_range_count) {
        lxw_snprintf(number, ATTR_32, "%d", named_range_count);
        _add_heading_pair(app, "Named Ranges", number);
    }

    _app_assemble_xml_file(app);

    _add_file_to_zip(self, app->file, "docProps/app.xml");

    fclose(app->file);

    _free_app(app);

    return 0;
}

/*
 * Write the core.xml file.
 */
STATIC uint8_t
_write_core_file(lxw_packager *self)
{
    lxw_core *core = _new_core();

    core->file = lxw_tmpfile();

    core->properties = self->workbook->properties;

    _core_assemble_xml_file(core);

    _add_file_to_zip(self, core->file, "docProps/core.xml");

    fclose(core->file);

    _free_core(core);

    return 0;
}

/*
 * Write the theme.xml file.
 */
STATIC uint8_t
_write_theme_file(lxw_packager *self)
{
    lxw_theme *theme = _new_theme();

    theme->file = lxw_tmpfile();

    _theme_assemble_xml_file(theme);

    _add_file_to_zip(self, theme->file, "xl/theme/theme1.xml");

    fclose(theme->file);

    _free_theme(theme);

    return 0;
}

/*
 * Write the styles.xml file.
 */
STATIC uint8_t
_write_styles_file(lxw_packager *self)
{
    lxw_styles *styles = _new_styles();
    lxw_hash_element *hash_element;

    /* Copy the unique and in-use formats from the workbook to the styles
     * xf_format list. */
    LXW_FOREACH_ORDERED(hash_element, self->workbook->used_xf_formats) {
        lxw_format *workbook_format = (lxw_format *) hash_element->value;
        lxw_format *style_format = _new_format();
        memcpy(style_format, workbook_format, sizeof(lxw_format));
        STAILQ_INSERT_TAIL(styles->xf_formats, style_format, list_pointers);
    }

    styles->font_count = self->workbook->font_count;
    styles->border_count = self->workbook->border_count;
    styles->fill_count = self->workbook->fill_count;
    styles->num_format_count = self->workbook->num_format_count;
    styles->xf_count = self->workbook->used_xf_formats->unique_count;

    styles->file = lxw_tmpfile();

    _styles_assemble_xml_file(styles);

    _add_file_to_zip(self, styles->file, "xl/styles.xml");

    fclose(styles->file);

    _free_styles(styles);

    return 0;
}

/*
 * Write the ContentTypes.xml file.
 */
STATIC uint8_t
_write_content_types_file(lxw_packager *self)
{
    lxw_content_types *content_types = _new_content_types();
    lxw_workbook *workbook = self->workbook;
    lxw_worksheet *worksheet;
    char sheetname[FILENAME_LEN] = { 0 };
    uint16_t index = 1;

    content_types->file = lxw_tmpfile();

    STAILQ_FOREACH(worksheet, workbook->worksheets, list_pointers) {
        lxw_snprintf(sheetname, FILENAME_LEN, "sheet%d", index++);
        _ct_add_worksheet_name(content_types, sheetname);
    }

    if (workbook->sst->string_count)
        _ct_add_shared_strings(content_types);

    _content_types_assemble_xml_file(content_types);

    _add_file_to_zip(self, content_types->file, "[Content_Types].xml");

    fclose(content_types->file);

    _free_content_types(content_types);

    return 0;
}

/*
 * Write the workbook .rels xml file.
 */
STATIC uint8_t
_write_workbook_rels_file(lxw_packager *self)
{
    lxw_relationships *rels = _new_relationships();
    lxw_workbook *workbook = self->workbook;
    lxw_worksheet *worksheet;
    char sheetname[FILENAME_LEN] = { 0 };
    uint16_t index = 1;

    rels->file = lxw_tmpfile();

    STAILQ_FOREACH(worksheet, workbook->worksheets, list_pointers) {
        lxw_snprintf(sheetname, FILENAME_LEN, "worksheets/sheet%d.xml",
                     index++);
        _add_document_relationship(rels, "/worksheet", sheetname);
    }

    _add_document_relationship(rels, "/theme", "theme/theme1.xml");
    _add_document_relationship(rels, "/styles", "styles.xml");

    if (workbook->sst->string_count)
        _add_document_relationship(rels, "/sharedStrings",
                                   "sharedStrings.xml");

    _relationships_assemble_xml_file(rels);

    _add_file_to_zip(self, rels->file, "xl/_rels/workbook.xml.rels");

    fclose(rels->file);
    _free_relationships(rels);

    return 0;
}

/*
 * Write the worksheet .rels files for worksheets that contain links to
 * external data such as hyperlinks or drawings.
 */
STATIC uint8_t
_write_worksheet_rels_file(lxw_packager *self)
{
    lxw_relationships *rels;
    lxw_rel_tuple *hlink;
    lxw_workbook *workbook = self->workbook;
    lxw_worksheet *worksheet;
    char sheetname[FILENAME_LEN] = { 0 };
    uint16_t index = 1;

    STAILQ_FOREACH(worksheet, workbook->worksheets, list_pointers) {

        if (STAILQ_EMPTY(worksheet->external_hyperlinks))
            continue;

        rels = _new_relationships();
        rels->file = lxw_tmpfile();

        STAILQ_FOREACH(hlink, worksheet->external_hyperlinks, list_pointers) {
            _add_worksheet_relationship(rels, hlink->type, hlink->target,
                                        hlink->target_mode);

        }

        lxw_snprintf(sheetname, FILENAME_LEN,
                     "xl/worksheets/_rels/sheet%d.xml.rels", index++);

        _relationships_assemble_xml_file(rels);

        _add_file_to_zip(self, rels->file, sheetname);

        fclose(rels->file);
        _free_relationships(rels);
    }

    return 0;
}

/*
 * Write the _rels/.rels xml file.
 */
STATIC uint8_t
_write_root_rels_file(lxw_packager *self)
{
    lxw_relationships *rels = _new_relationships();

    rels->file = lxw_tmpfile();

    _add_document_relationship(rels, "/officeDocument", "xl/workbook.xml");
    _add_package_relationship(rels, "/metadata/core-properties",
                              "docProps/core.xml");
    _add_document_relationship(rels, "/extended-properties",
                               "docProps/app.xml");

    _relationships_assemble_xml_file(rels);

    _add_file_to_zip(self, rels->file, "_rels/.rels");

    fclose(rels->file);

    _free_relationships(rels);

    return 0;
}

/*****************************************************************************
 *
 * Public functions.
 *
 ****************************************************************************/

int16_t
_add_file_to_zip(lxw_packager *self, FILE * file, const char *filename)
{
    int16_t error = ZIP_OK;
    size_t size_read;

    error = zipOpenNewFileInZip4_64(self->zipfile,
                                    filename,
                                    &self->zipfile_info,
                                    NULL, 0, NULL, 0, NULL,
                                    Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0,
                                    -MAX_WBITS, DEF_MEM_LEVEL,
                                    Z_DEFAULT_STRATEGY, NULL, 0, 0, 0, 0);

    if (error != ZIP_OK) {
        LXW_ERROR("Error adding member to zipfile");
        return error;
    }

    fflush(file);
    rewind(file);

    size_read = fread(self->buffer, 1, self->buffer_size, file);

    while (size_read) {

        if (size_read < self->buffer_size) {
            if (feof(file) == 0) {
                LXW_ERROR("Error reading member file data");
                return ZIP_ERRNO;
            }
        }

        error = zipWriteInFileInZip(self->zipfile,
                                    self->buffer, (unsigned int) size_read);

        if (error < 0) {
            LXW_ERROR("Error in writing member in the zipfile");
            return error;
        }

        size_read = fread(self->buffer, 1, self->buffer_size, file);
    }

    if (error < 0) {
        return ZIP_ERRNO;
    }
    else {
        error = zipCloseFileInZip(self->zipfile);
        if (error != ZIP_OK) {
            LXW_ERROR("Error in closing member in the zipfile");
            return error;
        }
    }

    return ZIP_OK;
}

/*
 * Write the xml files that make up the XLXS OPC package.
 */
uint8_t
_create_package(lxw_packager *self)
{

    _write_worksheet_files(self);
    _write_workbook_file(self);
    _write_shared_strings_file(self);
    _write_app_file(self);
    _write_core_file(self);
    _write_theme_file(self);
    _write_styles_file(self);
    _write_content_types_file(self);
    _write_workbook_rels_file(self);
    _write_worksheet_rels_file(self);
    _write_root_rels_file(self);

    zipClose(self->zipfile, NULL);

    return 0;
}
