/*
 * libxlsxwriter
 *
 * Copyright 2014-2016, John McNamara, jmcnamara@cpan.org. See LICENSE.txt.
 *
 * packager - A libxlsxwriter library for creating Excel XLSX packager files.
 *
 */
#ifndef __LXW_PACKAGER_H__
#define __LXW_PACKAGER_H__

#include <stdint.h>
#include "xlsxwriter/third_party/minizip/zip.h"

#include "common.h"
#include "workbook.h"
#include "worksheet.h"
#include "shared_strings.h"
#include "app.h"
#include "core.h"
#include "theme.h"
#include "styles.h"
#include "format.h"
#include "content_types.h"
#include "relationships.h"

#define LXW_ZIP_BUFFER_SIZE (16384)

/*
 * Struct to represent a packager.
 */
typedef struct lxw_packager {

    FILE *file;
    lxw_workbook *workbook;

    size_t buffer_size;
    zipFile zipfile;
    zip_fileinfo zipfile_info;
    char *filename;
    char *buffer;

    uint16_t drawing_count;

} lxw_packager;


/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

lxw_packager *lxw_packager_new(const char *filename);
void lxw_packager_free(lxw_packager *packager);
uint8_t lxw_create_package(lxw_packager *self);

/* Declarations required for unit testing. */
#ifdef TESTING

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_PACKAGER_H__ */
