/*
 * Tests for the libxlsxwriter library.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2026, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "../../../include/xlsxwriter/workbook.h"

/* Helper function to open a test image file. The unit tests are normally run
 * from the test/unit directory but may also be run from an individual test
 * directory such as test/unit/workbook, or from the project root (as with
 * CMake/ctest), so try each relative path. */
static FILE *
_open_test_image(const char *filename)
{
    const char *dirs[] = {
        "../functional/src/images",
        "../../functional/src/images",
        "test/functional/src/images",
    };
    char path[256];
    FILE *stream;
    size_t i;

    for (i = 0; i < sizeof(dirs) / sizeof(dirs[0]); i++) {
        lxw_snprintf(path, sizeof(path), "%s/%s", dirs[i], filename);
        stream = lxw_fopen(path, "rb");
        if (stream)
            return stream;
    }

    return NULL;
}


/* Test the _get_image_properties() function for a range of image files and
 * confirm the extracted width, height, dpi and type. Port of the test_images()
 * test in the rust_xlsxwriter image unit tests. */
CTEST(workbook, get_image_properties) {

    size_t i;

    struct image_test {
        const char *filename;
        double width;
        double height;
        double x_dpi;
        double y_dpi;
        const char *extension;
    };

    /* Filename, width, height, x_dpi, y_dpi, type. */
    struct image_test image_test_data[] = {
        {"black_150.jpg",          64,   64,  150.0,      150.0,      "jpeg"},
        {"black_300.jpg",          64,   64,  300.0,      300.0,      "jpeg"},
        {"black_300.png",          64,   64,  299.9994,   299.9994,   "png"},
        {"black_300e.png",         64,   64,  299.9994,   299.9994,   "png"},
        {"black_40000x45000.gif",  40000, 45000, 96.0,    96.0,       "gif"},
        {"black_72.jpg",           64,   64,  72.0,       72.0,       "jpeg"},
        {"black_72.png",           64,   64,  72.009,     72.009,     "png"},
        {"black_72e.png",          64,   64,  72.009,     72.009,     "png"},
        {"black_96.jpg",           64,   64,  96.0,       96.0,       "jpeg"},
        {"black_96.png",           64,   64,  96.012,     96.012,     "png"},
        {"blue.jpg",               23,   23,  96.0,       96.0,       "jpeg"},
        {"blue.png",               23,   23,  96.0,       96.0,       "png"},
        {"grey.jpg",               99,   69,  96.0,       96.0,       "jpeg"},
        {"grey.png",               99,   69,  96.0,       96.0,       "png"},
        {"happy.jpg",              423,  563, 96.0,       96.0,       "jpeg"},
        {"issue32.png",            115,  115, 96.0,       96.0,       "png"},
        {"logo.gif",               200,  80,  96.0,       96.0,       "gif"},
        {"logo.jpg",               200,  80,  96.0,       96.0,       "jpeg"},
        {"logo.png",               200,  80,  96.0,       96.0,       "png"},
        {"mylogo.png",             215,  36,  95.9866,    95.9866,    "png"},
        {"red.bmp",                32,   32,  96.0,       96.0,       "bmp"},
        {"red.gif",                32,   32,  96.0,       96.0,       "gif"},
        {"red.jpg",                32,   32,  96.0,       96.0,       "jpeg"},
        {"red.png",                32,   32,  96.0,       96.0,       "png"},
        {"red2.png",               32,   32,  96.0,       96.0,       "png"},
        {"red_208.png",            208,  49,  96.0,       96.0,       "png"},
        {"red_64x20.png",          64,   20,  96.0,       96.0,       "png"},
        {"red_readonly.png",       32,   32,  96.0,       96.0,       "png"},
        {"red_topdown_20x12.bmp",  20,   12,  96.0,       96.0,       "bmp"},
        {"red_topdown_32x32.bmp",  32,   32,  96.0,       96.0,       "bmp"},
        {"train.jpg",              640,  480, 96.0,       96.0,       "jpeg"},
        {"watermark.png",          1778, 1003, 329.9968,  329.9968,   "png"},
        {"yellow.jpg",             72,   72,  96.0,       96.0,       "jpeg"},
        {"yellow.png",             72,   72,  96.0,       96.0,       "png"},
        {"zero_dpi.jpg",           11,   16,  96.0,       96.0,       "jpeg"},
        {"black_150.png",          64,   64,  150.0124,   150.0124,   "png"},
        {"black_150e.png",         64,   64,  150.0124,   150.0124,   "png"},
    };

    size_t num_tests = sizeof(image_test_data) / sizeof(image_test_data[0]);

    for (i = 0; i < num_tests; i++) {
        lxw_object_properties *object_props;
        lxw_error err;
        struct image_test *test_data = &image_test_data[i];

        object_props = calloc(1, sizeof(lxw_object_properties));
        ASSERT_NOT_NULL(object_props);

        object_props->filename = lxw_strdup(test_data->filename);
        object_props->stream = _open_test_image(test_data->filename);
        ASSERT_NOT_NULL(object_props->stream);

        err = _get_image_properties(object_props);
        ASSERT_EQUAL(LXW_NO_ERROR, err);

        ASSERT_DBL_NEAR_TOL(test_data->width,  object_props->width,  0.0001);
        ASSERT_DBL_NEAR_TOL(test_data->height, object_props->height, 0.0001);
        ASSERT_DBL_NEAR_TOL(test_data->x_dpi,  object_props->x_dpi,  0.0001);
        ASSERT_DBL_NEAR_TOL(test_data->y_dpi,  object_props->y_dpi,  0.0001);
        ASSERT_STR(test_data->extension, object_props->extension);

        fclose(object_props->stream);
        _free_object_properties(object_props);
    }
}
