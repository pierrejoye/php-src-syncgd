/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | SPDX-License-Identifier: BSD-3-Clause                                |
   +----------------------------------------------------------------------+
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "zend_object_handlers.h"

#include "gd_metadata.h"
#include "gd_metadata_arginfo.h"

zend_class_entry *php_gd_metadata_ce;

typedef struct {
	gdImageMetadata *metadata;
	zend_object std;
} php_gd_metadata_object;

typedef struct {
	zend_object_iterator iterator;
	size_t position;
	zval current;
} php_gd_metadata_iterator;

static zend_object_handlers php_gd_metadata_handlers;

static php_gd_metadata_object *php_gd_metadata_from_object(zend_object *object)
{
	return (php_gd_metadata_object *) ((char *) object - offsetof(php_gd_metadata_object, std));
}

static php_gd_metadata_iterator *php_gd_metadata_iterator_from_iterator(zend_object_iterator *iterator)
{
	return (php_gd_metadata_iterator *) iterator;
}

static zend_object *php_gd_metadata_create_object(zend_class_entry *ce)
{
	php_gd_metadata_object *object = zend_object_alloc(sizeof(*object), ce);

	object->metadata = gdImageMetadataCreate();
	zend_object_std_init(&object->std, ce);
	object_properties_init(&object->std, ce);
	object->std.handlers = &php_gd_metadata_handlers;
	return &object->std;
}

static void php_gd_metadata_free_object(zend_object *object)
{
	php_gd_metadata_object *metadata = php_gd_metadata_from_object(object);

	if (metadata->metadata != NULL) {
		gdImageMetadataFree(metadata->metadata);
	}
	zend_object_std_dtor(&metadata->std);
}

gdImageMetadata *php_gd_metadata_from_zval(zval *zv)
{
	return php_gd_metadata_from_object(Z_OBJ_P(zv))->metadata;
}

void php_gd_metadata_create_zval(zval *zv, gdImageMetadata *metadata)
{
	php_gd_metadata_object *object;

	object_init_ex(zv, php_gd_metadata_ce);
	object = php_gd_metadata_from_object(Z_OBJ_P(zv));
	gdImageMetadataFree(object->metadata);
	object->metadata = metadata;
}

static int php_gd_metadata_find(const gdImageMetadata *metadata, const char *key,
		const unsigned char **data, size_t *size)
{
	size_t i;

	for (i = 0; i < gdImageMetadataGetProfileCount(metadata); i++) {
		const char *profile_key;
		const unsigned char *profile_data;
		size_t profile_size;

		if (gdImageMetadataGetProfileAt(metadata, i, &profile_key, &profile_data, &profile_size) == GD_META_OK &&
				strcmp(profile_key, key) == 0) {
			if (data != NULL) {
				*data = profile_data;
			}
			if (size != NULL) {
				*size = profile_size;
			}
			return 1;
		}
	}

	return 0;
}

static bool php_gd_metadata_validate_key(zend_string *key, uint32_t arg_num)
{
	if (ZSTR_LEN(key) == 0 || memchr(ZSTR_VAL(key), '\0', ZSTR_LEN(key)) != NULL) {
		zend_argument_value_error(arg_num, "must be a non-empty string without null bytes");
		return false;
	}
	return true;
}

gdImageMetadata *php_gd_metadata_copy(const gdImageMetadata *source)
{
	gdImageMetadata *copy = gdImageMetadataCreate();
	size_t i;

	if (copy == NULL) {
		return NULL;
	}
	for (i = 0; i < gdImageMetadataGetProfileCount(source); i++) {
		const char *key;
		const unsigned char *data;
		size_t size;

		if (gdImageMetadataGetProfileAt(source, i, &key, &data, &size) != GD_META_OK ||
				gdImageMetadataSetProfile(copy, key, data, size) != GD_META_OK) {
			gdImageMetadataFree(copy);
			return NULL;
		}
	}
	return copy;
}

PHP_METHOD(Gd_Metadata, create)
{
	ZEND_PARSE_PARAMETERS_NONE();
	object_init_ex(return_value, php_gd_metadata_ce);
}

PHP_METHOD(Gd_Metadata, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Metadata, has)
{
	zend_string *key;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(key)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_metadata_validate_key(key, 1)) {
		RETURN_THROWS();
	}
	RETURN_BOOL(php_gd_metadata_find(php_gd_metadata_from_zval(ZEND_THIS), ZSTR_VAL(key), NULL, NULL));
}

PHP_METHOD(Gd_Metadata, get)
{
	zend_string *key;
	const unsigned char *data;
	size_t size;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(key)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_metadata_validate_key(key, 1)) {
		RETURN_THROWS();
	}
	if (!php_gd_metadata_find(php_gd_metadata_from_zval(ZEND_THIS), ZSTR_VAL(key), &data, &size)) {
		RETURN_NULL();
	}
	RETURN_STRINGL((const char *) data, size);
}

PHP_METHOD(Gd_Metadata, keys)
{
	size_t i;
	gdImageMetadata *metadata = php_gd_metadata_from_zval(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();
	array_init_size(return_value, gdImageMetadataGetProfileCount(metadata));
	for (i = 0; i < gdImageMetadataGetProfileCount(metadata); i++) {
		const char *key;

		if (gdImageMetadataGetProfileAt(metadata, i, &key, NULL, NULL) == GD_META_OK) {
			add_next_index_string(return_value, key);
		}
	}
}

PHP_METHOD(Gd_Metadata, count)
{
	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_LONG((zend_long) gdImageMetadataGetProfileCount(php_gd_metadata_from_zval(ZEND_THIS)));
}

PHP_METHOD(Gd_Metadata, getIterator)
{
	ZEND_PARSE_PARAMETERS_NONE();
	zend_create_internal_iterator_zval(return_value, ZEND_THIS);
}

PHP_METHOD(Gd_Metadata, with)
{
	zend_string *key, *data;
	gdImageMetadata *copy;
	int status;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_STR(data)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_metadata_validate_key(key, 1)) {
		RETURN_THROWS();
	}
	copy = php_gd_metadata_copy(php_gd_metadata_from_zval(ZEND_THIS));
	if (copy == NULL) {
		zend_throw_error(NULL, "Failed to copy metadata");
		RETURN_THROWS();
	}
	status = gdImageMetadataSetProfile(copy, ZSTR_VAL(key), (const unsigned char *) ZSTR_VAL(data), ZSTR_LEN(data));
	if (status != GD_META_OK) {
		gdImageMetadataFree(copy);
		if (status == GD_META_ERR_LIMIT) {
			zend_argument_value_error(2, "exceeds the metadata size limit");
		} else {
			zend_throw_error(NULL, "Failed to update metadata");
		}
		RETURN_THROWS();
	}
	php_gd_metadata_create_zval(return_value, copy);
}

PHP_METHOD(Gd_Metadata, without)
{
	zend_string *key;
	gdImageMetadata *copy;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(key)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_metadata_validate_key(key, 1)) {
		RETURN_THROWS();
	}
	copy = php_gd_metadata_copy(php_gd_metadata_from_zval(ZEND_THIS));
	if (copy == NULL || gdImageMetadataRemoveProfile(copy, ZSTR_VAL(key)) != GD_META_OK) {
		gdImageMetadataFree(copy);
		zend_throw_error(NULL, "Failed to copy metadata");
		RETURN_THROWS();
	}
	php_gd_metadata_create_zval(return_value, copy);
}

static void php_gd_metadata_iterator_dtor(zend_object_iterator *iterator)
{
	php_gd_metadata_iterator *iter = php_gd_metadata_iterator_from_iterator(iterator);

	if (!Z_ISUNDEF(iter->current)) {
		zval_ptr_dtor(&iter->current);
	}
	zval_ptr_dtor(&iterator->data);
}

static zend_result php_gd_metadata_iterator_valid(zend_object_iterator *iterator)
{
	php_gd_metadata_iterator *iter = php_gd_metadata_iterator_from_iterator(iterator);
	gdImageMetadata *metadata = php_gd_metadata_from_zval(&iterator->data);

	return iter->position < gdImageMetadataGetProfileCount(metadata) ? SUCCESS : FAILURE;
}

static zval *php_gd_metadata_iterator_get_current_data(zend_object_iterator *iterator)
{
	php_gd_metadata_iterator *iter = php_gd_metadata_iterator_from_iterator(iterator);
	gdImageMetadata *metadata = php_gd_metadata_from_zval(&iterator->data);
	const unsigned char *data;
	size_t size;

	if (!Z_ISUNDEF(iter->current)) {
		return &iter->current;
	}
	if (gdImageMetadataGetProfileAt(metadata, iter->position, NULL, &data, &size) != GD_META_OK) {
		ZVAL_NULL(&iter->current);
	} else {
		ZVAL_STRINGL(&iter->current, (const char *) data, size);
	}
	return &iter->current;
}

static void php_gd_metadata_iterator_get_current_key(zend_object_iterator *iterator, zval *key)
{
	php_gd_metadata_iterator *iter = php_gd_metadata_iterator_from_iterator(iterator);
	gdImageMetadata *metadata = php_gd_metadata_from_zval(&iterator->data);
	const char *profile_key;

	if (gdImageMetadataGetProfileAt(metadata, iter->position, &profile_key, NULL, NULL) != GD_META_OK) {
		ZVAL_NULL(key);
	} else {
		ZVAL_STRING(key, profile_key);
	}
}

static void php_gd_metadata_iterator_move_forward(zend_object_iterator *iterator)
{
	php_gd_metadata_iterator *iter = php_gd_metadata_iterator_from_iterator(iterator);

	if (!Z_ISUNDEF(iter->current)) {
		zval_ptr_dtor(&iter->current);
		ZVAL_UNDEF(&iter->current);
	}
	iter->position++;
}

static void php_gd_metadata_iterator_rewind(zend_object_iterator *iterator)
{
	php_gd_metadata_iterator *iter = php_gd_metadata_iterator_from_iterator(iterator);

	if (!Z_ISUNDEF(iter->current)) {
		zval_ptr_dtor(&iter->current);
		ZVAL_UNDEF(&iter->current);
	}
	iter->position = 0;
}

static const zend_object_iterator_funcs php_gd_metadata_iterator_funcs = {
	php_gd_metadata_iterator_dtor,
	php_gd_metadata_iterator_valid,
	php_gd_metadata_iterator_get_current_data,
	php_gd_metadata_iterator_get_current_key,
	php_gd_metadata_iterator_move_forward,
	php_gd_metadata_iterator_rewind,
	NULL,
	NULL,
};

static zend_object_iterator *php_gd_metadata_get_iterator(zend_class_entry *ce, zval *object, int by_ref)
{
	php_gd_metadata_iterator *iter;

	if (by_ref) {
		zend_throw_exception(NULL, "Cannot iterate metadata by reference", 0);
		return NULL;
	}
	iter = ecalloc(1, sizeof(*iter));
	zend_iterator_init(&iter->iterator);
	iter->iterator.funcs = &php_gd_metadata_iterator_funcs;
	ZVAL_OBJ_COPY(&iter->iterator.data, Z_OBJ_P(object));
	ZVAL_UNDEF(&iter->current);
	iter->position = 0;
	return &iter->iterator;
}

void php_gd_metadata_minit(void)
{
	php_gd_metadata_ce = register_class_Gd_Metadata(zend_ce_countable, zend_ce_aggregate);
	php_gd_metadata_ce->create_object = php_gd_metadata_create_object;
	php_gd_metadata_ce->get_iterator = php_gd_metadata_get_iterator;

	memcpy(&php_gd_metadata_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_metadata_handlers.offset = offsetof(php_gd_metadata_object, std);
	php_gd_metadata_handlers.free_obj = php_gd_metadata_free_object;
	php_gd_metadata_handlers.clone_obj = NULL;
}
