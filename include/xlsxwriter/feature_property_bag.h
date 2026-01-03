/*
 * libxlsxwriter
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 * feature_property_bag - A libxlsxwriter library for creating Excel XLSX
 *                        featurePropertyBag files.
 *
 */
#ifndef __LXW_FEATURE_PROPERTY_BAG_H__
#define __LXW_FEATURE_PROPERTY_BAG_H__

#include <stdint.h>

#include "common.h"

/* Feature property bag types. */
#define LXW_FEATURE_BAG_NONE           0
#define LXW_FEATURE_BAG_XF_COMPLEMENTS  1
#define LXW_FEATURE_BAG_DXF_COMPLEMENTS 2

/*
 * Struct to represent a feature_property_bag.
 */
typedef struct lxw_feature_property_bag {

    FILE *file;
    uint8_t has_xf_complements;
    uint8_t has_dxf_complements;

} lxw_feature_property_bag;


/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C" {
#endif
/* *INDENT-ON* */

lxw_feature_property_bag *lxw_feature_property_bag_new(void);
void lxw_feature_property_bag_free(lxw_feature_property_bag
                                   *feature_property_bag);
void lxw_feature_property_bag_assemble_xml_file(lxw_feature_property_bag
                                                *self);

/* Declarations required for unit testing. */
#ifdef TESTING

#endif /* TESTING */

/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */

#endif /* __LXW_FEATURE_PROPERTY_BAG_H__ */
