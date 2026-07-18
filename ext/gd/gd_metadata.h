#ifndef PHP_GD_METADATA_H
#define PHP_GD_METADATA_H

#include "php.h"

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
#else
# include <gd.h>
#endif

extern zend_class_entry *php_gd_metadata_ce;

void php_gd_metadata_minit(void);
gdImageMetadata *php_gd_metadata_from_zval(zval *zv);
gdImageMetadata *php_gd_metadata_copy(const gdImageMetadata *metadata);
void php_gd_metadata_create_zval(zval *zv, gdImageMetadata *metadata);

#endif
