/*
   +----------------------------------------------------------------------+
   | Copyright © The PHP Group and Contributors.                          |
   +----------------------------------------------------------------------+
   | This source file is subject to the Modified BSD License that is      |
   | bundled with this package in the file LICENSE, and is available      |
   | through the World Wide Web at https://www.php.net/license/bsd-3-clause. |
   |                                                                      |
   | SPDX-License-Identifier: BSD-3-Clause                                |
   +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_enum.h"
#include "zend_exceptions.h"
#include "php_gd.h"
#include "gd_codec_write.h"
#include "gd_qoi.h"
#include "gd_metadata.h"
#include "ext/spl/spl_exceptions.h"
#include <limits.h>

#ifdef HAVE_GD_BUNDLED
# include "libgd/gd.h"
# include "libgd/gdhelpers.h"
#else
# include <gd.h>
#endif

#include "gd_qoi_arginfo.h"
#include "gd_qoi_decl.h"

static zend_class_entry *php_gd_codec_exception_ce;

#ifdef HAVE_GD_QOI
static zend_class_entry *php_gd_qoi_colorspace_ce;
static zend_class_entry *php_gd_qoi_read_options_ce;
static zend_class_entry *php_gd_qoi_write_options_ce;
static zend_class_entry *php_gd_qoi_info_ce;
static zend_class_entry *php_gd_qoi_reader_ce;
static zend_object_handlers php_gd_qoi_reader_handlers;

typedef struct {
	zend_string *bytes;
	zval info;
	bool read;
	bool failed;
	zend_object std;
} php_gd_qoi_reader_object;

static php_gd_qoi_reader_object *php_gd_qoi_reader_from_object(zend_object *object)
{
	return (php_gd_qoi_reader_object *) ((char *) object - offsetof(php_gd_qoi_reader_object, std));
}

#define Z_GD_QOI_READER_P(zv) php_gd_qoi_reader_from_object(Z_OBJ_P((zv)))

static int php_gd_qoi_colorspace(zval *colorspace)
{
	ZEND_ASSERT(Z_TYPE_P(colorspace) == IS_OBJECT);

	switch (zend_enum_fetch_case_id(Z_OBJ_P(colorspace))) {
		case ZEND_ENUM_Gd_Qoi_Colorspace_Linear:
			return GD_QOI_LINEAR;
		case ZEND_ENUM_Gd_Qoi_Colorspace_SRGB:
		default:
			return GD_QOI_SRGB;
	}
}

static void php_gd_qoi_throw_open_failure(const char *message)
{
	if (!EG(exception)) {
		zend_throw_exception(php_gd_codec_exception_ce, message, 0);
	}
}

static bool php_gd_qoi_create_info(zval *result, zend_string *bytes)
{
	gdQoiInfo info;

	if (ZSTR_LEN(bytes) > INT_MAX) {
		php_gd_qoi_throw_open_failure("QOI input is too large");
		return false;
	}

	gdQoiInfoInit(&info);
	if (!gdQoiGetInfoPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes), &info)) {
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to read QOI info", 0);
		return false;
	}

	object_init_ex(result, php_gd_qoi_info_ce);
	zval colorspace;
	ZVAL_OBJ(&colorspace, zend_enum_get_case_by_id(php_gd_qoi_colorspace_ce,
		info.colorspace == GD_QOI_LINEAR ? ZEND_ENUM_Gd_Qoi_Colorspace_Linear : ZEND_ENUM_Gd_Qoi_Colorspace_SRGB));
	zend_update_property_long(php_gd_qoi_info_ce, Z_OBJ_P(result), ZEND_STRL("width"), info.width);
	zend_update_property_long(php_gd_qoi_info_ce, Z_OBJ_P(result), ZEND_STRL("height"), info.height);
	zend_update_property_long(php_gd_qoi_info_ce, Z_OBJ_P(result), ZEND_STRL("channels"), info.channels);
	zend_update_property_long(php_gd_qoi_info_ce, Z_OBJ_P(result), ZEND_STRL("colorspaceTag"), info.colorspace);
	zend_update_property(php_gd_qoi_info_ce, Z_OBJ_P(result), ZEND_STRL("colorspace"), &colorspace);
	return true;
}

static zend_string *php_gd_qoi_read_stream(php_stream *stream)
{
	return php_stream_copy_to_mem(stream, PHP_STREAM_COPY_ALL, 0);
}

static bool php_gd_qoi_read_file_bytes(zend_string *path, zend_string **bytes)
{
	php_stream *stream = php_stream_open_wrapper(ZSTR_VAL(path), "rb", REPORT_ERRORS | IGNORE_PATH, NULL);

	if (stream == NULL) {
		php_gd_qoi_throw_open_failure("Failed to open QOI input");
		return false;
	}
	*bytes = php_gd_qoi_read_stream(stream);
	php_stream_close(stream);
	if (*bytes == NULL) {
		php_gd_qoi_throw_open_failure("Failed to read QOI input");
		return false;
	}
	return true;
}

static bool php_gd_qoi_read_stream_bytes(zval *stream_zv, zend_string **bytes)
{
	php_stream *stream;

	if (Z_TYPE_P(stream_zv) != IS_RESOURCE) {
		zend_argument_type_error(1, "must be a valid stream resource");
		return false;
	}
	php_stream_from_zval_no_verify(stream, stream_zv);
	if (stream == NULL) {
		zend_argument_type_error(1, "must be a valid stream resource");
		return false;
	}
	*bytes = php_gd_qoi_read_stream(stream);
	if (*bytes == NULL) {
		php_gd_qoi_throw_open_failure("Failed to read QOI input");
		return false;
	}
	return true;
}

static gdImagePtr php_gd_qoi_decode_bytes(zend_string *bytes)
{
	if (ZSTR_LEN(bytes) > INT_MAX) {
		return NULL;
	}
	return gdImageCreateFromQoiPtr((int) ZSTR_LEN(bytes), ZSTR_VAL(bytes));
}

static zend_object *php_gd_qoi_reader_create(zend_class_entry *class_entry)
{
	php_gd_qoi_reader_object *reader = zend_object_alloc(sizeof(*reader), class_entry);

	reader->bytes = NULL;
	ZVAL_UNDEF(&reader->info);
	reader->read = false;
	reader->failed = false;
	zend_object_std_init(&reader->std, class_entry);
	object_properties_init(&reader->std, class_entry);
	reader->std.handlers = &php_gd_qoi_reader_handlers;
	return &reader->std;
}

static void php_gd_qoi_reader_free(zend_object *object)
{
	php_gd_qoi_reader_object *reader = php_gd_qoi_reader_from_object(object);

	if (reader->bytes != NULL) {
		zend_string_release(reader->bytes);
	}
	if (!Z_ISUNDEF(reader->info)) {
		zval_ptr_dtor(&reader->info);
	}
	zend_object_std_dtor(&reader->std);
}

static bool php_gd_qoi_initialize_reader(zval *result, zend_string *bytes)
{
	php_gd_qoi_reader_object *reader;

	object_init_ex(result, php_gd_qoi_reader_ce);
	reader = Z_GD_QOI_READER_P(result);
	reader->bytes = zend_string_copy(bytes);
	if (!php_gd_qoi_create_info(&reader->info, bytes)) {
		return false;
	}
	return true;
}

static bool php_gd_qoi_reader_read_image(php_gd_qoi_reader_object *reader, zval *return_value)
{
	gdImagePtr im;

	if (reader->failed) {
		php_gd_qoi_throw_open_failure("QOI reader is in a failed state");
		return false;
	}
	if (reader->read) {
		php_gd_qoi_throw_open_failure("QOI image has already been read");
		return false;
	}

	im = php_gd_qoi_decode_bytes(reader->bytes);
	reader->read = true;
	if (im == NULL) {
		reader->failed = true;
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to decode QOI image", 0);
		return false;
	}

	php_gd_assign_libgdimageptr_as_extgdimage(return_value, im);
	return true;
}

static bool php_gd_qoi_decode_reader_bytes(zend_string *bytes, zval *return_value)
{
	zval reader_zv;
	php_gd_qoi_reader_object *reader;
	bool result;

	if (!php_gd_qoi_initialize_reader(&reader_zv, bytes)) {
		zval_ptr_dtor(&reader_zv);
		return false;
	}
	reader = Z_GD_QOI_READER_P(&reader_zv);
	result = php_gd_qoi_reader_read_image(reader, return_value);
	zval_ptr_dtor(&reader_zv);
	return result;
}

PHP_METHOD(Gd_Qoi_Codec, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Qoi_ReadOptions, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Qoi_WriteOptions, __construct)
{
	zval *colorspace_zv = NULL, *metadata = NULL;
	zval default_colorspace;

	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(colorspace_zv, php_gd_qoi_colorspace_ce)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(metadata, php_gd_metadata_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (colorspace_zv == NULL) {
		ZVAL_OBJ(&default_colorspace, zend_enum_get_case_by_id(php_gd_qoi_colorspace_ce, ZEND_ENUM_Gd_Qoi_Colorspace_SRGB));
		colorspace_zv = &default_colorspace;
	}

	zend_update_property(php_gd_qoi_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("colorspace"), colorspace_zv);
	if (metadata != NULL) {
		zend_update_property(php_gd_qoi_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"), metadata);
	} else {
		zend_update_property_null(php_gd_qoi_write_options_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("metadata"));
	}
}

PHP_METHOD(Gd_Qoi_Info, __construct)
{
	zend_long width, height, channels, colorspace_tag;
	zval *colorspace;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_LONG(width)
		Z_PARAM_LONG(height)
		Z_PARAM_LONG(channels)
		Z_PARAM_LONG(colorspace_tag)
		Z_PARAM_OBJECT_OF_CLASS(colorspace, php_gd_qoi_colorspace_ce)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_long(php_gd_qoi_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("width"), width);
	zend_update_property_long(php_gd_qoi_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("height"), height);
	zend_update_property_long(php_gd_qoi_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("channels"), channels);
	zend_update_property_long(php_gd_qoi_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("colorspaceTag"), colorspace_tag);
	zend_update_property(php_gd_qoi_info_ce, Z_OBJ_P(ZEND_THIS), ZEND_STRL("colorspace"), colorspace);
}

PHP_METHOD(Gd_Qoi_Reader, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();
}

PHP_METHOD(Gd_Qoi_Reader, fromString)
{
	zend_string *bytes;
	zval *options = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(bytes)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options, php_gd_qoi_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_qoi_initialize_reader(return_value, bytes)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Qoi_Reader, fromFile)
{
	zend_string *path, *bytes;
	zval *options = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options, php_gd_qoi_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_qoi_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_qoi_initialize_reader(return_value, bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Qoi_Reader, fromStream)
{
	zval *stream_zv;
	zval *options = NULL;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options, php_gd_qoi_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_qoi_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_qoi_initialize_reader(return_value, bytes)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Qoi_Reader, info)
{
	php_gd_qoi_reader_object *reader = Z_GD_QOI_READER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();
	RETURN_COPY(&reader->info);
}

PHP_METHOD(Gd_Qoi_Reader, read)
{
	php_gd_qoi_reader_object *reader = Z_GD_QOI_READER_P(ZEND_THIS);

	ZEND_PARSE_PARAMETERS_NONE();
	if (!php_gd_qoi_reader_read_image(reader, return_value)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Qoi_Codec, fromFile)
{
	zend_string *path, *bytes;
	zval *options = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_PATH_STR(path)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options, php_gd_qoi_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (!php_gd_qoi_read_file_bytes(path, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_qoi_decode_reader_bytes(bytes, return_value)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Qoi_Codec, fromStream)
{
	zval *stream_zv;
	zval *options = NULL;
	zend_string *bytes;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ZVAL(stream_zv)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options, php_gd_qoi_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(stream_zv) != IS_RESOURCE) {
		zend_argument_type_error(1, "must be a valid stream resource");
		RETURN_THROWS();
	}

	if (!php_gd_qoi_read_stream_bytes(stream_zv, &bytes)) {
		RETURN_THROWS();
	}
	if (!php_gd_qoi_decode_reader_bytes(bytes, return_value)) {
		zend_string_release(bytes);
		RETURN_THROWS();
	}
	zend_string_release(bytes);
}

PHP_METHOD(Gd_Qoi_Codec, fromString)
{
	zend_string *bytes;
	zval *options = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(bytes)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options, php_gd_qoi_read_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (ZSTR_LEN(bytes) > ZEND_LONG_MAX) {
		zend_argument_value_error(1, "is too long");
		RETURN_THROWS();
	}

	if (!php_gd_qoi_decode_reader_bytes(bytes, return_value)) {
		RETURN_THROWS();
	}
}

static void php_gd_qoi_write_to_context(INTERNAL_FUNCTION_PARAMETERS, bool require_stream)
{
	zval *image_zv;
	zval *destination;
	zval *options_zv = NULL;
	zval rv, *colorspace_zv;
	gdIOCtx *ctx;

	ZEND_PARSE_PARAMETERS_START(2, 3)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_ZVAL(destination)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_qoi_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	if (require_stream && Z_TYPE_P(destination) != IS_RESOURCE) {
		zend_argument_type_error(2, "must be a valid stream resource");
		RETURN_THROWS();
	}
	ZEND_ASSERT(require_stream || Z_TYPE_P(destination) == IS_STRING);

	ctx = php_gd_create_output_context(destination, 2);
	if (ctx == NULL) {
		php_gd_qoi_throw_open_failure("Failed to open QOI output");
		RETURN_THROWS();
	}
	gdQoiWriteOptions options;
	gdQoiWriteOptionsInit(&options);

	if (options_zv != NULL) {
		colorspace_zv = zend_read_property(php_gd_qoi_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("colorspace"), true, &rv);
		options.colorspace = php_gd_qoi_colorspace(colorspace_zv);
	}

	int result = gdImageQoiCtxWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), ctx, &options);
	ctx->gd_free(ctx);
	if (!result) {
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to encode QOI image", 0);
		RETURN_THROWS();
	}
}

PHP_METHOD(Gd_Qoi_Codec, toFile)
{
	php_gd_qoi_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, false);
}

PHP_METHOD(Gd_Qoi_Codec, toStream)
{
	php_gd_qoi_write_to_context(INTERNAL_FUNCTION_PARAM_PASSTHRU, true);
}

PHP_METHOD(Gd_Qoi_Codec, toString)
{
	zval *image_zv;
	zval *options_zv = NULL;
	zval rv, *colorspace_zv;
	int size = 0;
	void *data;
	gdQoiWriteOptions options;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJECT_OF_CLASS(image_zv, gd_image_ce)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJECT_OF_CLASS(options_zv, php_gd_qoi_write_options_ce)
	ZEND_PARSE_PARAMETERS_END();

	gdQoiWriteOptionsInit(&options);
	if (options_zv != NULL) {
		colorspace_zv = zend_read_property(php_gd_qoi_write_options_ce, Z_OBJ_P(options_zv), ZEND_STRL("colorspace"), true, &rv);
		options.colorspace = php_gd_qoi_colorspace(colorspace_zv);
	}

	data = gdImageQoiPtrWithOptions(php_gd_libgdimageptr_from_zval_p(image_zv), &size, &options);
	if (data == NULL || size < 0) {
		gdFree(data);
		zend_throw_exception(php_gd_codec_exception_ce, "Failed to encode QOI image", 0);
		RETURN_THROWS();
	}

	RETVAL_STRINGL(data, size);
	gdFree(data);
}
#endif

void php_gd_qoi_minit(void)
{
	php_gd_codec_exception_ce = php_gd_get_codec_exception_ce();

#ifdef HAVE_GD_QOI
	zend_class_entry *codec_ce;

	php_gd_qoi_colorspace_ce = register_class_Gd_Qoi_Colorspace();
	php_gd_qoi_read_options_ce = register_class_Gd_Qoi_ReadOptions();
	php_gd_qoi_write_options_ce = register_class_Gd_Qoi_WriteOptions(php_gd_get_codec_write_options_ce());
	php_gd_qoi_info_ce = register_class_Gd_Qoi_Info();
	php_gd_qoi_reader_ce = register_class_Gd_Qoi_Reader();
	php_gd_qoi_reader_ce->create_object = php_gd_qoi_reader_create;

	memcpy(&php_gd_qoi_reader_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	php_gd_qoi_reader_handlers.offset = offsetof(php_gd_qoi_reader_object, std);
	php_gd_qoi_reader_handlers.free_obj = php_gd_qoi_reader_free;
	php_gd_qoi_reader_handlers.clone_obj = NULL;
	codec_ce = register_class_Gd_Qoi_Codec();
	php_gd_register_codec_write(php_gd_qoi_write_options_ce, codec_ce);
	php_gd_register_codec_format("Qoi", php_gd_qoi_write_options_ce);
	php_gd_register_codec_extension("qoi", php_gd_qoi_write_options_ce);
#endif
}
