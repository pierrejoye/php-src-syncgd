/*
   +----------------------------------------------------------------------+
   | Copyright © The PHP Group and Contributors.                          |
   +----------------------------------------------------------------------+
   | This source file is subject to the Modified BSD License that is      |
   | bundled with this package in the file LICENSE, and is available      |
   | through the World Wide Web at https://www.php.net/license/bsd-3-clause. |
   +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_enum.h"
#include "zend_exceptions.h"
#include "gd_codec_write.h"

struct _php_gd_codec_write_entry {
	zend_class_entry *options_ce;
	zend_class_entry *codec_ce;
	zend_function *to_file;
	zend_function *to_stream;
	zend_function *to_string;
};

static zend_class_entry *php_gd_codec_exception_ce;
static zend_class_entry *php_gd_codec_write_options_ce;
static HashTable php_gd_codec_write_registry;
static HashTable php_gd_codec_write_format_registry;
static HashTable php_gd_codec_write_extension_registry;
static bool php_gd_codec_write_registry_initialized;

static void php_gd_codec_write_entry_dtor(zval *entry_zv)
{
	pefree(Z_PTR_P(entry_zv), true);
}

static zend_function *php_gd_find_codec_method(zend_class_entry *codec_ce, const char *name, size_t name_len)
{
	zend_string *lcname = zend_string_alloc(name_len, true);
	zend_str_tolower_copy(ZSTR_VAL(lcname), name, name_len);
	zend_function *function = zend_hash_find_ptr(&codec_ce->function_table, lcname);
	zend_string_release_ex(lcname, true);
	return function;
}

void php_gd_codec_write_minit(zend_class_entry *write_options_ce, zend_class_entry *codec_exception_ce)
{
	zend_hash_init(&php_gd_codec_write_registry, 0, NULL, php_gd_codec_write_entry_dtor, true);
	zend_hash_init(&php_gd_codec_write_format_registry, 0, NULL, NULL, true);
	zend_hash_init(&php_gd_codec_write_extension_registry, 0, NULL, NULL, true);
	php_gd_codec_write_registry_initialized = true;
	php_gd_codec_write_options_ce = write_options_ce;
	php_gd_codec_exception_ce = codec_exception_ce;
}

void php_gd_codec_write_mshutdown(void)
{
	if (!php_gd_codec_write_registry_initialized) {
		return;
	}

	zend_hash_destroy(&php_gd_codec_write_registry);
	zend_hash_destroy(&php_gd_codec_write_format_registry);
	zend_hash_destroy(&php_gd_codec_write_extension_registry);
	php_gd_codec_write_registry_initialized = false;
	php_gd_codec_write_options_ce = NULL;
	php_gd_codec_exception_ce = NULL;
}

zend_class_entry *php_gd_get_codec_write_options_ce(void)
{
	ZEND_ASSERT(php_gd_codec_write_options_ce != NULL);
	return php_gd_codec_write_options_ce;
}

void php_gd_register_codec_write(zend_class_entry *options_ce, zend_class_entry *codec_ce)
{
	php_gd_codec_write_entry *entry;

	ZEND_ASSERT(php_gd_codec_write_registry_initialized);

	entry = pemalloc(sizeof(*entry), true);
	entry->options_ce = options_ce;
	entry->codec_ce = codec_ce;
	entry->to_file = php_gd_find_codec_method(codec_ce, ZEND_STRL("toFile"));
	entry->to_stream = php_gd_find_codec_method(codec_ce, ZEND_STRL("toStream"));
	entry->to_string = php_gd_find_codec_method(codec_ce, ZEND_STRL("toString"));

	ZEND_ASSERT(entry->to_file != NULL);
	ZEND_ASSERT(entry->to_stream != NULL);
	ZEND_ASSERT(entry->to_string != NULL);

	zend_hash_index_update_ptr(&php_gd_codec_write_registry, (zend_ulong) (uintptr_t) options_ce, entry);
}

void php_gd_register_codec_format(const char *format_case_name, zend_class_entry *options_ce)
{
	ZEND_ASSERT(php_gd_codec_write_registry_initialized);

	zend_hash_str_update_ptr(&php_gd_codec_write_format_registry, format_case_name, strlen(format_case_name), options_ce);
}

void php_gd_register_codec_extension(const char *extension, zend_class_entry *options_ce)
{
	ZEND_ASSERT(php_gd_codec_write_registry_initialized);

	zend_hash_str_update_ptr(&php_gd_codec_write_extension_registry, extension, strlen(extension), options_ce);
}

php_gd_codec_write_entry *php_gd_find_codec_write_entry(zend_class_entry *options_ce)
{
	return zend_hash_index_find_ptr(&php_gd_codec_write_registry, (zend_ulong) (uintptr_t) options_ce);
}

zend_class_entry *php_gd_find_write_options_ce_by_format(zval *format_zv)
{
	zval *case_name = zend_enum_fetch_case_name(Z_OBJ_P(format_zv));

	return zend_hash_find_ptr(&php_gd_codec_write_format_registry, Z_STR_P(case_name));
}

bool php_gd_create_default_write_options(zval *options_zv, zend_class_entry *options_ce)
{
	object_init_ex(options_zv, options_ce);
	if (options_ce->constructor != NULL) {
		zend_call_known_function(options_ce->constructor, Z_OBJ_P(options_zv), options_ce, NULL, 0, NULL, NULL);
		if (EG(exception)) {
			zval_ptr_dtor(options_zv);
			ZVAL_UNDEF(options_zv);
			return false;
		}
	}

	return true;
}

zend_class_entry *php_gd_infer_write_options_ce(zend_string *path)
{
	const char *filename = ZSTR_VAL(path);
	size_t filename_len = ZSTR_LEN(path);
	const char *end = filename + filename_len;
	const char *extension = end;
	zend_string *lc_extension;
	zend_class_entry *options_ce;

	while (extension > filename && extension[-1] != '.' && extension[-1] != '/' && extension[-1] != '\\') {
		extension--;
	}

	if (extension == filename || extension[-1] != '.' || extension == end) {
		zend_throw_exception_ex(php_gd_codec_exception_ce, 0, "Cannot infer image format from path");
		return NULL;
	}

	lc_extension = zend_string_alloc(end - extension, 0);
	zend_str_tolower_copy(ZSTR_VAL(lc_extension), extension, end - extension);
	options_ce = zend_hash_find_ptr(&php_gd_codec_write_extension_registry, lc_extension);
	zend_string_release_ex(lc_extension, 0);

	if (options_ce == NULL) {
		zend_throw_exception_ex(php_gd_codec_exception_ce, 0, "Cannot infer image format from path");
		return NULL;
	}

	return options_ce;
}

static void php_gd_call_codec_write(zend_function *function, zval *return_value, uint32_t param_count, zval *params)
{
	zend_call_known_function(function, NULL, function->common.scope, return_value, param_count, params, NULL);
}

void php_gd_codec_write_to_file(php_gd_codec_write_entry *entry, zval *return_value, zval *image, zend_string *path, zval *options)
{
	zval params[3];

	ZVAL_COPY(&params[0], image);
	ZVAL_STR_COPY(&params[1], path);
	ZVAL_COPY(&params[2], options);
	php_gd_call_codec_write(entry->to_file, return_value, 3, params);
	zval_ptr_dtor(&params[2]);
	zval_ptr_dtor(&params[1]);
	zval_ptr_dtor(&params[0]);
}

void php_gd_codec_write_to_stream(php_gd_codec_write_entry *entry, zval *return_value, zval *image, zval *stream, zval *options)
{
	zval params[3];

	ZVAL_COPY(&params[0], image);
	ZVAL_COPY(&params[1], stream);
	ZVAL_COPY(&params[2], options);
	php_gd_call_codec_write(entry->to_stream, return_value, 3, params);
	zval_ptr_dtor(&params[2]);
	zval_ptr_dtor(&params[1]);
	zval_ptr_dtor(&params[0]);
}

void php_gd_codec_write_to_string(php_gd_codec_write_entry *entry, zval *return_value, zval *image, zval *options)
{
	zval params[2];

	ZVAL_COPY(&params[0], image);
	ZVAL_COPY(&params[1], options);
	php_gd_call_codec_write(entry->to_string, return_value, 2, params);
	zval_ptr_dtor(&params[1]);
	zval_ptr_dtor(&params[0]);
}
