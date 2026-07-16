/*
   +----------------------------------------------------------------------+
   | Copyright © The PHP Group and Contributors.                          |
   +----------------------------------------------------------------------+
   | This source file is subject to the Modified BSD License that is      |
   | bundled with this package in the file LICENSE, and is available      |
   | through the World Wide Web at https://www.php.net/license/bsd-3-clause. |
   +----------------------------------------------------------------------+
*/

#ifndef GD_CODEC_WRITE_H
#define GD_CODEC_WRITE_H

#include "php.h"

typedef struct _php_gd_codec_write_entry php_gd_codec_write_entry;

void php_gd_codec_write_minit(zend_class_entry *write_options_ce, zend_class_entry *codec_exception_ce);
void php_gd_codec_write_mshutdown(void);
zend_class_entry *php_gd_get_codec_write_options_ce(void);
void php_gd_register_codec_write(zend_class_entry *options_ce, zend_class_entry *codec_ce);
void php_gd_register_codec_format(const char *format_case_name, zend_class_entry *options_ce);
void php_gd_register_codec_extension(const char *extension, zend_class_entry *options_ce);
php_gd_codec_write_entry *php_gd_find_codec_write_entry(zend_class_entry *options_ce);
zend_class_entry *php_gd_find_write_options_ce_by_format(zval *format_zv);
zend_class_entry *php_gd_infer_write_options_ce(zend_string *path);
bool php_gd_create_default_write_options(zval *options_zv, zend_class_entry *options_ce);
void php_gd_codec_write_to_file(php_gd_codec_write_entry *entry, zval *return_value, zval *image, zend_string *path, zval *options);
void php_gd_codec_write_to_stream(php_gd_codec_write_entry *entry, zval *return_value, zval *image, zval *stream, zval *options);
void php_gd_codec_write_to_string(php_gd_codec_write_entry *entry, zval *return_value, zval *image, zval *options);

#endif /* GD_CODEC_WRITE_H */
